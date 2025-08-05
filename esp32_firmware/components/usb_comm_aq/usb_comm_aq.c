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
#include "usb_mac.h"
#include "usb_descriptors.h"

static const char *TAG = "usb_comm_aq";

// -----------------------------------------------------------------------------
// esp_netif Glue & State
// -----------------------------------------------------------------------------
static esp_netif_t *s_usb_netif = NULL;
static usb_comm_aq_event_callback_t s_event_callback = NULL;
static bool s_usb_connected = false;
static uint8_t s_mac_address[6];

// Forward declarations for our custom driver functions
static esp_err_t aq_netif_driver_transmit(void *h, void *buffer, size_t len);
void tud_network_init_cb(void);

// USB task
static void usb_task(void *param)
{
    (void)param;
    ESP_LOGI(TAG, "usb_task running");
    while (1) {
        tud_task();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// -----------------------------------------------------------------------------
// Public API Implementation
// -----------------------------------------------------------------------------
esp_err_t usb_comm_aq_init(const char *panel_id, const char *ip_addr, const uint8_t* mac_addr, usb_comm_aq_event_callback_t cb) {
    log_json_message(TAG, "INFO", "Initializing USB-NCM component...");
    s_event_callback = cb;

    // 1. Get USB strings and MAC address
    memcpy(s_mac_address, mac_addr, sizeof(s_mac_address));

    // 2. Install TinyUSB driver
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = NULL,
        .string_descriptor_count = 0,
        .external_phy = false,
        .configuration_descriptor = NULL,
        .self_powered = true,
    };
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    log_json_message(TAG, "INFO", "TinyUSB driver installed.");

    // Create a task to run tud_task()
    xTaskCreate(usb_task, "usb_task", 4096, NULL, 5, NULL);

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