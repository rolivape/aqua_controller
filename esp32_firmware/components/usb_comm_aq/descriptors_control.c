#include "tusb.h"
#include "usb_descriptors.h"
#include "sdkconfig.h"

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void) langid;
    static uint16_t desc_str[32];
    uint8_t chr_count = 0;

    // Assign the MAC address string here.
    const char* mac_str = "02006414CFAC";

    switch (index) {
        case STRID_LANGID:
            desc_str[1] = 0x0409; // English (US)
            chr_count = 1;
            break;
        case STRID_MANUFACTURER:
            chr_count = strlen(CONFIG_TINYUSB_DESC_MANUFACTURER_STRING);
            for(uint8_t i=0; i<chr_count; i++) desc_str[1+i] = CONFIG_TINYUSB_DESC_MANUFACTURER_STRING[i];
            break;
        case STRID_PRODUCT:
            chr_count = strlen(CONFIG_TINYUSB_DESC_PRODUCT_STRING);
            for(uint8_t i=0; i<chr_count; i++) desc_str[1+i] = CONFIG_TINYUSB_DESC_PRODUCT_STRING[i];
            break;
        case STRID_SERIAL:
            chr_count = strlen(CONFIG_TINYUSB_DESC_SERIAL_STRING);
            for(uint8_t i=0; i<chr_count; i++) desc_str[1+i] = CONFIG_TINYUSB_DESC_SERIAL_STRING[i];
            break;
        case 4: // iMacAddress
            chr_count = strlen(mac_str);
            for (uint8_t i = 0; i < chr_count; i++) {
                desc_str[1 + i] = mac_str[i];
            }
            break;
        default:
            return NULL;
    }

    // first byte is length (including header), second byte is string type
    desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

    return desc_str;
}
