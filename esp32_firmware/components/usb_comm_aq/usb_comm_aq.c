/*
 * Aqua Controller USB Communication Component (v15 - Final & Correct)
 *
 * This version implements the correct, standard ESP-IDF method for creating
 * and attaching a network driver, resolving all previous compilation and runtime errors.
 */

#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "tinyusb.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "lwip/sockets.h"
#include "class/net/ncm.h"
#include <string.h>
#include "assert.h"

#include "usb_comm_aq.h"
#include "config_aq.h"
#include "json_log.h"

static const char *TAG = "usb_comm_aq";

// -----------------------------------------------------------------------------
// USB Descriptors & Configuration
// -----------------------------------------------------------------------------
#define USBD_VID (0x303A)
#define USBD_PID (0x4008)

enum { ITF_NUM_CDC_NCM = 0, ITF_NUM_CDC_NCM_DATA, ITF_NUM_TOTAL };
enum { STRID_LANGID = 0, STRID_MANUFACTURER, STRID_PRODUCT, STRID_SERIAL, STRID_MAC };

static const tusb_desc_device_t s_tusb_device_descriptor = {
    .bLength = sizeof(tusb_desc_device_t), .bDescriptorType = TUSB_DESC_DEVICE, .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC, .bDeviceSubClass = MISC_SUBCLASS_COMMON, .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = USBD_VID, .idProduct = USBD_PID, .bcdDevice = 0x0101,
    .iManufacturer = STRID_MANUFACTURER, .iProduct = STRID_PRODUCT, .iSerialNumber = STRID_SERIAL,
    .bNumConfigurations = 0x01
};

// -----------------------------------------------------------------------------
// esp_netif Glue & State
// -----------------------------------------------------------------------------
static esp_netif_t *s_usb_netif = NULL;
static usb_comm_aq_event_callback_t s_event_callback = NULL;
static bool s_usb_connected = false;
static uint8_t s_mac_address[6] = {0x02, 0x00, 0x04, 0x00, 0x00, 0x00};
static char s_mac_address_str[13];

// Forward declarations for our custom driver functions
static esp_err_t aq_netif_driver_transmit(void *h, void *buffer, size_t len);
void tud_network_init_cb(void);

// -----------------------------------------------------------------------------
// Public API Implementation
// -----------------------------------------------------------------------------
esp_err_t usb_comm_aq_init(const char *panel_id, const char *ip_addr, usb_comm_aq_event_callback_t cb) {
    log_json_message(TAG, "INFO", "Initializing USB-NCM component...");
    s_event_callback = cb;

    // 1. Get USB strings and MAC address
    snprintf(s_mac_address_str, sizeof(s_mac_address_str), "%02X%02X%02X%02X%02X%02X",
             s_mac_address[0], s_mac_address[1], s_mac_address[2], s_mac_address[3], s_mac_address[4], s_mac_address[5]);

    const char *s_tusb_string_descriptors[] = {
        [STRID_LANGID]       = (const char[]){0x09, 0x04},
        [STRID_MANUFACTURER] = config_aq_get_usb_manufacturer(),
        [STRID_PRODUCT]      = config_aq_get_usb_product(),
        [STRID_SERIAL]       = config_aq_get_usb_serial(),
        [STRID_MAC]          = s_mac_address_str,
    };

    // 2. Install TinyUSB driver
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = &s_tusb_device_descriptor,
        .string_descriptor = s_tusb_string_descriptors,
        .string_descriptor_count = sizeof(s_tusb_string_descriptors) / sizeof(s_tusb_string_descriptors[0]),
        .external_phy = false,
        .configuration_descriptor = NULL, // Let TinyUSB generate it
        .self_powered = true,
    };
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    log_json_message(TAG, "INFO", "TinyUSB driver installed.");

    // 3. Create the esp_netif instance. The IO driver is set later in tud_network_init_cb
    const esp_netif_config_t netif_cfg = ESP_NETIF_DEFAULT_ETH();
    s_usb_netif = esp_netif_new(&netif_cfg);
    assert(s_usb_netif);

    // 4. Set static IP
    esp_netif_dhcpc_stop(s_usb_netif);
    esp_netif_ip_info_t ip_info;
    ip_info.ip.addr = ipaddr_addr(ip_addr);
    inet_pton(AF_INET, "255.255.255.0", &ip_info.netmask);
    inet_pton(AF_INET, "192.168.7.1", &ip_info.gw);
    ESP_ERROR_CHECK(esp_netif_set_ip_info(s_usb_netif, &ip_info));

    log_json_message(TAG, "INFO", "USB-NCM initialization complete.");
    return ESP_OK;
}

// --- Glue function implementations ---
static esp_err_t aq_netif_driver_transmit(void *h, void *buffer, size_t len) {
    if (tud_ready() && tud_network_can_xmit(len)) {
        tud_network_xmit(buffer, len);
        return ESP_OK;
    }
    return ESP_ERR_TIMEOUT;
}

// --- TinyUSB Callbacks ---
bool tud_network_recv_cb(const uint8_t *src, uint16_t size) {
    if (s_usb_netif) {
        esp_netif_receive(s_usb_netif, (void *)src, size, NULL);
    }
    return true;
}

void tud_network_init_cb(void) {
    // This callback is called from the TinyUSB task when the network stack is ready.
    // This is the correct place to set the IO driver and attach the netif.
    const esp_netif_driver_ifconfig_t driver_ifconfig = {
        .handle = s_usb_netif,
        .transmit = aq_netif_driver_transmit,
        .driver_free_rx_buffer = NULL // We don't need to free, TinyUSB handles it
    };
    
    ESP_ERROR_CHECK(esp_netif_set_driver_config(s_usb_netif, &driver_ifconfig));
    ESP_ERROR_CHECK(esp_netif_set_mac(s_usb_netif, s_mac_address));
    ESP_ERROR_CHECK(esp_netif_attach(s_usb_netif, esp_netif_get_io_driver(s_usb_netif)));
}

void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
    if (dtr && rts) {
        if (!s_usb_connected) {
            s_usb_connected = true;
            log_json_message(TAG, "INFO", "USB connected.");
            esp_netif_action_start(s_usb_netif, NULL, 0, NULL);
            esp_netif_action_connected(s_usb_netif, NULL, 0, NULL);
            if (s_event_callback) s_event_callback(true);
        }
    } else {
        if (s_usb_connected) {
            s_usb_connected = false;
            log_json_message(TAG, "WARN", "USB disconnected.");
            esp_netif_action_disconnected(s_usb_netif, NULL, 0, NULL);
            esp_netif_action_stop(s_usb_netif, NULL, 0, NULL);
            if (s_event_callback) s_event_callback(false);
        }
    }
}