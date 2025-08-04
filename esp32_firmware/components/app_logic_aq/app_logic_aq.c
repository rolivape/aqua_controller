#include "app_logic_aq.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "config_aq.h"
#include "json_log.h"
#include "usb_comm_aq.h"
#include "esp_log.h"

static const char *TAG = "APP_LOGIC_AQ";

// Dummy callback for USB events within app_logic
void app_logic_usb_event_callback(bool connected) {
    if (connected) {
        log_json_message(TAG, "INFO", "USB connection event: CONNECTED");
    } else {
        log_json_message(TAG, "WARN", "USB connection event: DISCONNECTED");
    }
}

esp_err_t app_logic_aq_init(void) {
    log_json_message(TAG, "INFO", "Initializing application logic...");

    esp_err_t err = config_aq_init();

    if (err != ESP_OK) {
        log_json_message(TAG, "ERROR", "Failed to initialize config_aq component.");
        return err;
    } else {
        log_json_message(TAG, "INFO", "config_aq component initialized successfully.");
    }

    char device_role[32];
    err = config_aq_get_string("device_role", device_role, sizeof(device_role));

    if (err == ESP_OK) {
        log_json_message(TAG, "INFO", "Retrieved device role.");
    } else {
        log_json_message(TAG, "ERROR", "Failed to get device role.");
    }
    
    log_json_message(TAG, "INFO", "config_aq handle closed.");

    // This component should not be initializing USB, main.c should.
    // This is left here for now to satisfy compilation, but in a real
    // architecture this would be removed.
    // err = usb_comm_aq_init("panel-01", "192.168.71.1", app_logic_usb_event_callback);
    // if (err != ESP_OK) {
    //     log_json_message(TAG, "ERROR", "Failed to initialize usb_comm_aq component.");
    //     return err;
    // }

    return ESP_OK;
}
