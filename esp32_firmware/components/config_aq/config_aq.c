/*
 * Aqua Controller Configuration Management (config_aq)
 *
 * This version pre-loads critical configuration from NVS at initialization
 * to provide safe, fast, and reliable access for other components. It also
 * handles initial provisioning by writing default values if they don't exist.
 */

#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include <string.h>
#include "config_aq.h"
#include "json_log.h"

static const char *TAG = "config_aq";
static const char *NVS_NAMESPACE = "aquacfg";

// Static storage for pre-loaded configuration
#define CONFIG_AQ_STR_MAX_LEN 32
static char s_usb_manufacturer[CONFIG_AQ_STR_MAX_LEN];
static char s_usb_product[CONFIG_AQ_STR_MAX_LEN];
static char s_usb_serial[CONFIG_AQ_STR_MAX_LEN];

// Internal function to load a string or write and commit a default value.
static esp_err_t load_or_set_default(nvs_handle_t handle, const char* key, char* dest, const char* default_val) {
    size_t required_size = CONFIG_AQ_STR_MAX_LEN;
    esp_err_t err = nvs_get_str(handle, key, dest, &required_size);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        log_json_message(TAG, "WARN", "Key not found, writing default.");
        strncpy(dest, default_val, CONFIG_AQ_STR_MAX_LEN - 1);
        dest[CONFIG_AQ_STR_MAX_LEN - 1] = '\0';
        
        err = nvs_set_str(handle, key, dest);
        if (err != ESP_OK) {
            log_json_message(TAG, "ERROR", "Failed to set default string in NVS.");
        }
        // Return the result of the set operation
        return err;
    } else if (err != ESP_OK) {
        log_json_message(TAG, "ERROR", "NVS read failed, using compiled-in default.");
        strncpy(dest, default_val, CONFIG_AQ_STR_MAX_LEN - 1);
        dest[CONFIG_AQ_STR_MAX_LEN - 1] = '\0';
    }
    return err;
}

esp_err_t config_aq_init(void) {
    log_json_message(TAG, "INFO", "Loading configuration from NVS...");

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        log_json_message(TAG, "FATAL", "Failed to open NVS namespace.");
        return err;
    }

    // Load all required configuration values, setting defaults if necessary
    load_or_set_default(nvs_handle, "usb_manuf", s_usb_manufacturer, "xAI");
    load_or_set_default(nvs_handle, "usb_prod", s_usb_product, "AquaPanel");
    load_or_set_default(nvs_handle, "usb_serial", s_usb_serial, "001");

    // Commit any changes made
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        log_json_message(TAG, "ERROR", "Failed to commit NVS changes.");
    }

    nvs_close(nvs_handle);
    log_json_message(TAG, "INFO", "Configuration loaded.");
    
    return err;
}

// --- Getters for pre-loaded values ---
const char* config_aq_get_usb_manufacturer(void) { return s_usb_manufacturer; }
const char* config_aq_get_usb_product(void) { return s_usb_product; }
const char* config_aq_get_usb_serial(void) { return s_usb_serial; }

// --- Generic NVS functions ---
esp_err_t config_aq_get_string(const char* key, char* out_value, size_t max_len) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) return err;
    err = nvs_get_str(nvs_handle, key, out_value, &max_len);
    nvs_close(nvs_handle);
    return err;
}

esp_err_t config_aq_set_string(const char* key, const char* value) {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) return err;
    err = nvs_set_str(nvs_handle, key, value);
    if (err == ESP_OK) err = nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
    return err;
}
