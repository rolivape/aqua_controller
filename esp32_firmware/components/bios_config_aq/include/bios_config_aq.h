/*
 * Aqua Controller BIOS Configuration Component (bios_config_aq)
 *
 * This component is responsible for loading the device's persistent
 * configuration from Non-Volatile Storage (NVS) at boot. It provides
 * a structured way to access core settings like the device role.
 */

#ifndef BIOS_CONFIG_AQ_H
#define BIOS_CONFIG_AQ_H

#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Maximum length for string fields in the BIOS configuration
#define BIOS_CONFIG_AQ_STR_MAX_LEN 32

/**
 * @brief Holds the persistent BIOS configuration of the device.
 *
 * This structure is serialized to and from NVS to maintain core
 * device settings across reboots.
 */
typedef struct {
    /** @brief The role of the device (e.g., "master", "sump_sensor", "display"). */
    char device_role[BIOS_CONFIG_AQ_STR_MAX_LEN];

    /** @brief A unique identifier for the panel or device. */
    char panel_id[BIOS_CONFIG_AQ_STR_MAX_LEN];

    // Future settings can be added here, e.g.:
    // uint32_t safe_mode_flags;
    // char mqtt_broker_uri[128];

} bios_config_t;

/**
 * @brief Loads the BIOS configuration from NVS into the provided struct.
 *
 * This function attempts to read the configuration blob from the 'aquacfg'
 * NVS namespace. If the configuration is not found (e.g., on first boot),
 * it initializes the struct with default values, writes them to NVS, and
 * commits the changes.
 *
 * @param[out] config Pointer to a `bios_config_t` struct to be filled with the loaded configuration.
 *
 * @return
 *     - ESP_OK: On successful load or successful creation of a default config.
 *     - ESP_FAIL: If a fatal error occurs during NVS access or deserialization.
 *     - ESP_ERR_INVALID_ARG: If the `config` pointer is NULL.
 */
esp_err_t bios_config_aq_load(bios_config_t *config);

#ifdef __cplusplus
}
#endif

#endif // BIOS_CONFIG_AQ_H
