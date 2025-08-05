/*
 * SPDX-FileCopyrightText: 2024 Aqua Controller
 * SPDX-License-Identifier: Apache-2.0
 *
 * Based on the Chegewara `usb-netif` component, which is a stripped-down version
 * of the Espressif `tinyusb_net` example.
 */

#include "tusb.h"

//--------------------------------------------------------------------+
//- ENUMERATION
//--------------------------------------------------------------------+
enum {
    ITF_NUM_CDC_NCM = 0,
    ITF_NUM_CDC_NCM_DATA,
    ITF_NUM_TOTAL
};

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device_aq = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x303A, // Espressif Inc.
    .idProduct = 0x4002,
    .bcdDevice = 0x0100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01
};

uint8_t const *tud_descriptor_device_cb(void)
{
    return (uint8_t const *)&desc_device_aq;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+
#define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_CDC_NCM_DESC_LEN)

uint8_t const desc_configuration_aq[] = {
    // Config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, TUSB_DESC_TOTAL_LEN, 0, 100),

    // Interface number, description string index, MAC address string index, EP notification address and size, EP data address (out, in), and size, max segment size.
    TUD_CDC_NCM_DESCRIPTOR(ITF_NUM_CDC_NCM, 0, 4, 0x82, 16, 0x01, 0x81, 64, CFG_TUD_NET_MTU),
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index;
    return desc_configuration_aq;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+
char const *string_desc_arr_aq[] = {
    (char[]){0x09, 0x04}, // 0: English (US)
    "AquaController",     // 1: Manufacturer
    "CDC-NCM Device",     // 2: Product
    "123456",             // 3: Serials
    "02006414CFAC",       // 4: MAC
};

static uint16_t _desc_str[32];

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void)langid;
    uint8_t chr_count;

    if (index == 0) {
        _desc_str[1] = 0x0409; // English (US)
        chr_count = 1;
    } else {
        if (!(index < sizeof(string_desc_arr_aq) / sizeof(string_desc_arr_aq[0]))) {
            return NULL;
        }
        const char *str = string_desc_arr_aq[index];
        chr_count = strlen(str);
        if (chr_count > 31) chr_count = 31;
        for (uint8_t i = 0; i < chr_count; i++) {
            _desc_str[1 + i] = str[i];
        }
    }

    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);
    return _desc_str;
}

//- MAC ADDRESS CALLBACK
uint8_t *tud_network_mac_address_cb(void)
{
    static uint8_t mac[] = {0x02, 0x00, 0x64, 0x14, 0xcf, 0xac};
    return mac;
}
