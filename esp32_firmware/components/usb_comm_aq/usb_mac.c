/*
 * Copyright (c) 2024, xAI
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "usb_mac.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "usb_mac";

/**
 * @brief Provides the static MAC address for the USB NCM interface.
 *
 * This function returns the hardcoded MAC address required for the AquaController.
 * The address is 02:00:64:14:CF:AC.
 *
 * @param[out] mac A 6-byte array to store the MAC address.
 *
 * @return
 *     - ESP_OK on success.
 */
esp_err_t usb_mac_get(uint8_t mac[6])
{
    static const uint8_t static_mac[6] = {0x02, 0x00, 0x64, 0x14, 0xcf, 0xac};
    memcpy(mac, static_mac, sizeof(static_mac));
    ESP_LOGI(TAG, "Providing static MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return ESP_OK;
}
