/*
 * Aqua Controller USB Communication Component
 *
 * Manages USB-NCM communication for ESP32-S3 panels, providing a stable
 * network interface over USB for local-first operation.
 */

#ifndef USB_COMM_AQ_H
#define USB_COMM_AQ_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback function type for USB communication events.
 *
 * @param connected True if the USB is connected, false otherwise.
 */
typedef void (*usb_comm_aq_event_callback_t)(bool connected);

/**
 * @brief Initializes the USB-NCM communication stack.
 *
 * This function sets up the esp_netif, configures TinyUSB with NCM descriptors,
 * and starts the USB driver. It establishes a network interface over USB,
 * allowing the device to communicate with a host (e.g., Raspberry Pi).
 *
 * @param panel_id A unique identifier for the panel, used for logging and potentially
 *                 for generating USB descriptors in more advanced scenarios.
 * @param ip_addr The static IP address to assign to the USB network interface
 *                (e.g., "192.168.7.2").
 * @param cb The callback function to be invoked upon USB connection or disconnection events.
 *
 * @return
 *     - ESP_OK: On successful initialization.
 *     - ESP_FAIL: If any critical step fails (e.g., netif creation, driver installation).
 *     - ESP_ERR_INVALID_ARG: If input parameters are invalid.
 */
esp_err_t usb_comm_aq_init(const char *panel_id, const char *ip_addr, usb_comm_aq_event_callback_t cb);

#ifdef __cplusplus
}
#endif

#endif // USB_COMM_AQ_H