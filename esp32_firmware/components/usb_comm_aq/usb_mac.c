/*
 * Copyright (c) 2024, xAI
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "usb_mac.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "bios_config_aq.h"

static const char *TAG = "usb_mac";

// NVS namespace and key for storing the MAC address
#define NVS_NAMESPACE "bios_cfg"
#define NVS_KEY_MAC "usb_mac"

/**
 * @brief Generates and stores a unique MAC address.
 *
 * Reads the base MAC from eFuse, sets the locally administered bit,
 * and saves it to NVS.
 *
 * @param[out] mac A 6-byte array to store the generated MAC address.
 */
static esp_err_t generate_and_store_mac(uint8_t mac[6])
{
    uint8_t base_mac[6];
    esp_err_t err = esp_efuse_mac_get_default(base_mac);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get base MAC from eFuse: %s", esp_err_to_name(err));
        return err;
    }

    // Create a locally administered MAC address
    // Set the second least significant bit of the first octet
    memcpy(mac, base_mac, sizeof(base_mac));
    mac[0] = (base_mac[0] & 0xFC) | 0x02;

    // Store the generated MAC in NVS
    err = bios_config_aq_set_blob(NVS_KEY_MAC, mac, 6);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to store MAC address in NVS: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "Generated and stored new MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    return ESP_OK;
}

void get_usb_ncm_mac(uint8_t mac[6])
{
    size_t required_size = 6;
    esp_err_t err = bios_config_aq_get_blob(NVS_KEY_MAC, mac, &required_size);

    if (err == ESP_OK && required_size == 6) {
        ESP_LOGI(TAG, "Found MAC address in NVS: %02x:%02x:%02x:%02x:%02x:%02x",
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else {
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            ESP_LOGW(TAG, "MAC address not found in NVS, generating a new one.");
        } else {
            ESP_LOGW(TAG, "Failed to read MAC from NVS (%s), generating a new one.", esp_err_to_name(err));
        }
        
        err = generate_and_store_mac(mac);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to generate MAC address, using fallback.");
            // Fallback MAC in case of failure
            uint8_t fallback_mac[6] = {0x02, 0x00, 0x00, 0xDE, 0xAD, 0xBE};
            memcpy(mac, fallback_mac, sizeof(fallback_mac));
        }
    }
}
