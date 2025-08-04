#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs_flash.h"

#include "app_logic_aq.h"
#include "json_log.h"
#include "usb_comm_aq.h"
#include "config_aq.h"

static const char *TAG = "AQUA_MAIN";

void usb_event_callback(bool connected) {
    if (connected) {
        log_json_message(TAG, "INFO", "USB connected.");
    } else {
        log_json_message(TAG, "WARN", "USB disconnected.");
    }
}

void app_main(void)
{
    // Step 1: Initialize NVS flash. This is a prerequisite for all other components.
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      log_json_message(TAG, "WARN", "NVS partition corrupt or full, erasing.");
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Step 2: Initialize the underlying TCP/IP stack. Required for esp_netif.
    ESP_ERROR_CHECK(esp_netif_init());

    // Step 3: Create the default event loop.
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Step 4: Initialize our custom configuration component.
    if (config_aq_init() != ESP_OK) {
        log_json_message(TAG, "FATAL", "Failed to initialize configuration. Halting.");
        while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }

    log_json_message(TAG, "INFO", "Starting AquaControllerUSB v5.2...");

    // Step 5: Initialize the USB communication stack.
    if (usb_comm_aq_init("main-panel", "192.168.7.10", usb_event_callback) != ESP_OK) {
        log_json_message(TAG, "ERROR", "USB Comm initialization failed. Entering SAFE MODE.");
        while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }

    // Step 6: Initialize the main application logic.
    if (app_logic_aq_init() != ESP_OK) {
        log_json_message(TAG, "ERROR", "Application logic initialization failed. Halting.");
        while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }

    log_json_message(TAG, "INFO", "Initialization complete. Entering main loop.");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
