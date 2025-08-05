#include "app_logic_aq.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "json_log.h"
#include "bios_config_aq.h" // Use the correct config component

static const char *TAG = "APP_LOGIC_AQ";

esp_err_t app_logic_aq_init(void) {
    log_json_message(TAG, "INFO", "Initializing application logic...");

    char device_role[BIOS_CONFIG_AQ_STR_MAX_LEN];
    esp_err_t err = bios_config_aq_get_string("bios.role", device_role, sizeof(device_role));

    if (err != ESP_OK) {
        // This path is now only taken on fatal NVS errors, not on key-not-found.
        log_json_message(TAG, "ERROR", "Failed to get device role due to NVS error.");
        return err;
    }

    // Log the retrieved role. It will be "UNKNOWN" on first boot.
    char log_msg[128];
    snprintf(log_msg, sizeof(log_msg), "Device role loaded: %s", device_role);
    log_json_message(TAG, "INFO", log_msg);

    if (strcmp(device_role, "UNKNOWN") == 0) {
        log_json_message(TAG, "WARN", "Device role is not set. Waiting for configuration.");
    }

    log_json_message(TAG, "INFO", "Application logic initialized successfully.");
    return ESP_OK;
}