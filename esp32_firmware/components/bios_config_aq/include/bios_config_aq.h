/*
 * Aqua Controller BIOS Configuration Component (bios_config_aq)
 *
 * Manages persistent device configuration using individual key-value
 * pairs in Non-Volatile Storage (NVS).
 */

#ifndef BIOS_CONFIG_AQ_H
#define BIOS_CONFIG_AQ_H

#include "esp_err.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BIOS_CONFIG_AQ_STR_MAX_LEN 32

/**
 * @brief Retrieves a configuration string from NVS.
 *
 * If the requested key is not found in NVS, this function will return a
 * default value ("UNKNOWN") and log a warning. It does not return an error
 * in this case, ensuring a graceful fallback.
 *
 * @param[in]  key       The NVS key for the desired string (e.g., "bios.role").
 * @param[out] out_value Buffer to store the retrieved string.
 * @param[in]  max_len   The maximum size of the out_value buffer.
 *
 * @return
 *     - ESP_OK: If the value was retrieved successfully or a default was applied.
 *     - ESP_FAIL: On a fatal NVS error.
 *     - ESP_ERR_INVALID_ARG: If input pointers are NULL or max_len is 0.
 */
esp_err_t bios_config_aq_get_string(const char *key, char *out_value, size_t max_len);

/**
 * @brief Stores a configuration string in NVS.
 *
 * This function allows for runtime updates of configuration values, such as
 * setting the device role via a command from the host.
 *
 * @param[in] key   The NVS key for the value to be stored (e.g., "bios.role").
 * @param[in] value The string value to store.
 *
 * @return
 *     - ESP_OK: On success.
 *     - ESP_FAIL: On a fatal NVS error.
 *     - ESP_ERR_INVALID_ARG: If key or value is NULL.
 */
esp_err_t bios_config_aq_set_string(const char *key, const char *value);

/**
 * @brief Retrieves a binary data blob from NVS.
 *
 * @param[in]      key       The NVS key for the blob.
 * @param[out]     out_value Buffer to store the retrieved blob.
 * @param[in,out]  length    On input, the size of the out_value buffer.
 *                           On output, the actual size of the retrieved blob.
 *
 * @return
 *     - ESP_OK: On success.
 *     - ESP_ERR_NVS_NOT_FOUND: If the key does not exist.
 *     - Other NVS errors on failure.
 */
esp_err_t bios_config_aq_get_blob(const char* key, void* out_value, size_t* length);

/**
 * @brief Stores a binary data blob in NVS.
 *
 * @param[in] key   The NVS key for the blob.
 * @param[in] value Pointer to the data to store.
 * @param[in] length The length of the data in bytes.
 *
 * @return
 *     - ESP_OK: On success.
 *     - Other NVS errors on failure.
 */
esp_err_t bios_config_aq_set_blob(const char* key, const void* value, size_t length);


#ifdef __cplusplus
}
#endif

#endif // BIOS_CONFIG_AQ_H