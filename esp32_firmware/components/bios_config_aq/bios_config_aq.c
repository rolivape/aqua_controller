/*
 * Aqua Controller BIOS Configuration Component (bios_config_aq)
 */

#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>

#include "bios_config_aq.h"
#include "json_log.h"

static const char *TAG = "bios_config_aq";
static const char *NVS_NAMESPACE = "aquacfg";

esp_err_t bios_config_aq_get_string(const char *key, char *out_value, size_t max_len) {
    if (key == NULL || out_value == NULL || max_len == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        log_json_message(TAG, "ERROR", "Failed to open NVS namespace for reading.");
        strncpy(out_value, "UNKNOWN", max_len - 1);
        out_value[max_len - 1] = '\0';
        return ESP_FAIL;
    }

    size_t required_size = max_len;
    err = nvs_get_str(nvs_handle, key, out_value, &required_size);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        log_json_message(TAG, "WARN", "Key not found in NVS, returning default.");
        strncpy(out_value, "UNKNOWN", max_len - 1);
        out_value[max_len - 1] = '\0';
        err = ESP_OK; // Not a fatal error, fallback is handled.
    } else if (err != ESP_OK) {
        log_json_message(TAG, "ERROR", "Failed to read key from NVS.");
        strncpy(out_value, "UNKNOWN", max_len - 1);
        out_value[max_len - 1] = '\0';
        err = ESP_FAIL;
    }

    nvs_close(nvs_handle);
    return err;
}

esp_err_t bios_config_aq_set_string(const char *key, const char *value) {
    if (key == NULL || value == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        log_json_message(TAG, "ERROR", "Failed to open NVS namespace for writing.");
        return ESP_FAIL;
    }

    err = nvs_set_str(nvs_handle, key, value);
    if (err != ESP_OK) {
        log_json_message(TAG, "ERROR", "Failed to write string to NVS.");
        nvs_close(nvs_handle);
        return ESP_FAIL;
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        log_json_message(TAG, "ERROR", "Failed to commit changes to NVS.");
        nvs_close(nvs_handle);
        return ESP_FAIL;
    }

    log_json_message(TAG, "INFO", "Successfully wrote and committed key to NVS.");
    nvs_close(nvs_handle);
    return ESP_OK;
}