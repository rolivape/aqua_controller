/*
 * Copyright (c) 2024, xAI
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef USB_MAC_H_
#define USB_MAC_H_

#include <stdint.h>

/**
 * @brief Gets the device-specific USB NCM MAC address.
 *
 * This function provides a unique MAC address for the USB network interface.
 * On the first call, it generates the MAC address based on the device's
 * base MAC address (from eFuse), sets it to be a locally-administered
 * address, and stores it in NVS for persistence. Subsequent calls will
 * read the address from NVS.
 *
 * @param[out] mac A 6-byte array to store the MAC address.
 */
void get_usb_ncm_mac(uint8_t mac[6]);

#endif /* USB_MAC_H_ */
