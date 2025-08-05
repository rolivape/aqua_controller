/*
 * Copyright (c) 2024, xAI
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef USB_MAC_H_
#define USB_MAC_H_

#include <stdint.h>
#include "esp_err.h"

/**
 * @brief Gets the device-specific USB NCM MAC address.
 *
 * This function provides a unique MAC address for the USB network interface.
 * If the key `aq_mac` is not found in NVS, it generates a new Locally
 * Administered Address (LAA) based on the device's eFuse MAC, stores it
 * in NVS, and then returns it. Subsequent calls will read the address from NVS.
 *
 * @param[out] mac A 6-byte array to store the MAC address.
 *
 * @return
 *     - ESP_OK on success.
 *     - ESP_FAIL on fatal error.
 */
esp_err_t usb_mac_get(uint8_t mac[6]);

#endif /* USB_MAC_H_ */