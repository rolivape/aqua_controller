#pragma once

#include "tusb.h"

#ifdef __cplusplus
extern "C" {
#endif

// Main callbacks invoked by TinyUSB stack
uint8_t const * tud_descriptor_device_cb(void);
uint8_t const * tud_descriptor_configuration_cb(uint8_t index);
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid);

// NCM specific callbacks
void tud_network_mac_address_cb(uint8_t mac_addr[6]);

#ifdef __cplusplus
}
#endif
