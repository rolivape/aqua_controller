/*
 * Aqua Controller Configuration Management (config_aq)
 *
 * Provides a simplified interface for NVS (Non-Volatile Storage) operations,
 * ensuring consistent key naming and error handling for configuration data.
 */

#ifndef CONFIG_AQ_H
#define CONFIG_AQ_H

#include "esp_err.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the NVS partition and loads all configuration data into memory.
 *
 * This function must be called once at startup before any other component
 * requires configuration. It initializes NVS, handles potential corruption,
 * and pre-loads all necessary configuration values (like USB strings) into
 * static memory, applying safe defaults if values are not found.
 *
 * @return
 *     - ESP_OK: On successful initialization and loading.
 *     - ESP_FAIL: If a fatal error occurs that cannot be recovered.
 */
esp_err_t config_aq_init(void);

/**
 * @brief Retrieves the pre-loaded USB manufacturer string.
 * @return A constant pointer to the null-terminated manufacturer string.
 */
const char* config_aq_get_usb_manufacturer(void);

/**
 * @brief Retrieves the pre-loaded USB product string.
 * @return A constant pointer to the null-terminated product string.
 */
const char* config_aq_get_usb_product(void);

/**
 * @brief Retrieves the pre-loaded USB serial number string.
 * @return A constant pointer to the null-terminated serial number string.
 */
const char* config_aq_get_usb_serial(void);

/**
 * @brief Retrieves a generic string value from NVS.
 *
 * @param key The key for the NVS entry.
 * @param out_value Buffer to store the retrieved string.
 * @param max_len The maximum size of the out_value buffer, including null terminator.
 *
 * @return
 *     - ESP_OK: If the key was found and the value was retrieved successfully.
 *     - ESP_ERR_NVS_NOT_FOUND: If the key does not exist in NVS.
 *     - Other NVS error codes on failure.
 */
esp_err_t config_aq_get_string(const char* key, char* out_value, size_t max_len);

/**
 * @brief Stores a string value in NVS.
 *
 * @param key The key for the NVS entry.
 * @param value The string value to store.
 *
 * @return
 *     - ESP_OK: On success.
 *     - Other NVS error codes on failure.
 */
esp_err_t config_aq_set_string(const char* key, const char* value);

#ifdef __cplusplus
}
#endif

#endif // CONFIG_AQ_H
