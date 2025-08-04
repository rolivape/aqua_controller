/*
 * Aqua Controller BIOS Configuration Component (bios_config_aq)
 */

#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>

#include "bios_config_aq.h"
#include "json_log.h" // Assuming a project-specific JSON logger

static const char *TAG = "bios_config_aq";
static const char *NVS_NAMESPACE = "aquacfg";
static const char *NVS_KEY = "bios_cfg";

/**
 * @brief Sets the default BIOS configuration.
 *
 * @param[out] config Pointer to the config structure to be filled.
 */
static void set_default_config(bios_config_t *config) {
    strncpy(config->device_role, "master", sizeof(config->device_role) - 1);
    config->device_role[sizeof(config->device_role) - 1] = '\0';

    strncpy(config->panel_id, "panel-01", sizeof(config->panel_id) - 1);
    config->panel_id[sizeof(config->panel_id) - 1] = '\0';
}

/**
 * @brief Public API to load the BIOS configuration.
 */
esp_err_t bios_config_aq_load(bios_config_t *config) {
    if (config == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t nvs_handle;
    esp_err_t err;

    // Open NVS with read-write access
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        log_json_message(TAG, "ERROR", "Failed to open NVS namespace.");
        return ESP_FAIL;
    }

    // Try to read the configuration blob
    size_t required_size = sizeof(bios_config_t);
    err = nvs_get_blob(nvs_handle, NVS_KEY, config, &required_size);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        log_json_message(TAG, "WARN", "BIOS config not found in NVS. Creating default.");
        
        // Set default values
        set_default_config(config);

        // Write the new default configuration to NVS
        err = nvs_set_blob(nvs_handle, NVS_KEY, config, sizeof(bios_config_t));
        if (err != ESP_OK) {
            log_json_message(TAG, "ERROR", "Failed to write default config to NVS.");
            nvs_close(nvs_handle);
            return ESP_FAIL;
        }

        // Commit the changes
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            log_json_message(TAG, "ERROR", "Failed to commit default config to NVS.");
            nvs_close(nvs_handle);
            return ESP_FAIL;
        }
        log_json_message(TAG, "INFO", "Default BIOS config written and committed to NVS.");

    } else if (err != ESP_OK) {
        log_json_message(TAG, "ERROR", "Failed to read BIOS config from NVS.");
        nvs_close(nvs_handle);
        return ESP_FAIL;
    } else if (required_size != sizeof(bios_config_t)) {
        log_json_message(TAG, "ERROR", "NVS blob size mismatch. Expected vs. actual.");
        // This could indicate a struct version mismatch. For now, we fail.
        // A more robust solution might involve migration logic.
        nvs_close(nvs_handle);
        return ESP_FAIL;
    } else {
        log_json_message(TAG, "INFO", "Successfully loaded BIOS config from NVS.");
    }

    // Close the NVS handle and return
    nvs_close(nvs_handle);
    return ESP_OK;
}
