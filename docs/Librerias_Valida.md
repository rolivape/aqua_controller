# Librerías Válidas, Componentes, Headers y Funciones para AquaControllerUSB_v5.2

**Versión: 1.0**
**Fecha: 28 de julio de 2025**
**Autor: Chief Programmer & PMO**
**Propósito**: Listado exhaustivo de librerías nativas ESP-IDF v5.5, componentes, headers y funciones válidas para el proyecto, para evitar errores recurrentes (e.g., headers no existentes como tusb_cdc_ncm.h). Basado en docs.espressif.com y ejemplos válidos.

## 1. Librerías y Componentes Válidos
- **esp_tinyusb**: Para USB-NCM (nativo, add-dependency esp_tinyusb).
  - Uso: Inicialización USB, descriptores NCM.
  - Componentes: tinyusb_driver_install, tinyusb_config_t.
- **esp_netif**: Para IP estática y attach a USB-NCM.
  - Uso: esp_netif_init, esp_netif_new, esp_netif_attach.
- **esp_mqtt**: Para MQTT (nativo).
  - Uso: esp_mqtt_client_init, esp_mqtt_client_publish.
- **nvs_flash**: Para configs (nativo).
  - Uso: nvs_flash_init, nvs_open, nvs_get_str.
- **cJSON**: Para JSON (submodule Git).
  - Uso: cJSON_Parse, cJSON_GetObjectItem.
- **esp_task_wdt / esp_rtc_wdt**: Para watchdog (nativo).
  - Uso: esp_task_wdt_init, esp_task_wdt_add.
- **gpio / ledc**: Para I/O paneles (nativo).
  - Uso: gpio_set_direction, ledc_timer_config.

## 2. Headers Válidos
- **USB-NCM**: tinyusb.h, class/net/net_device.h, esp_netif.h (no tusb_cdc_ncm.h, no existe).
- **MQTT**: mqtt_client.h.
- **NVS**: nvs_flash.h.
- **cJSON**: cJSON.h.
- **Watchdog**: wdts.h.
- **GPIO**: gpio.h.

## 3. Funciones y Clases Válidas
- **USB-NCM**:
  - tinyusb_driver_install(tinyusb_config_t *config)
  - esp_netif_new(esp_netif_config_t *config)
  - esp_netif_attach(esp_netif_t *netif, void *glue_handle)
- **MQTT**:
  - esp_mqtt_client_init(esp_mqtt_client_config_t *config)
  - esp_mqtt_client_publish(esp_mqtt_client_handle_t client, const char *topic, const char *data, int len, int qos, int retain)
- **NVS**:
  - nvs_flash_init()
  - nvs_open(const char *namespace, nvs_open_mode_t mode, nvs_handle_t *handle)
  - nvs_get_str(nvs_handle_t handle, const char *key, char *out, size_t *len)
- **cJSON**:
  - cJSON_Parse(const char *value)
  - cJSON_GetObjectItem(cJSON *object, const char *string)
- **Watchdog**:
  - esp_task_wdt_init(esp_task_wdt_config_t *config)
  - esp_task_wdt_add(void *task_handle)
- **GPIO**:
  - gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode)
  - ledc_timer_config(ledc_timer_config_t *config)

## 4. Estrategia para USB-NCM
- Wrapper: tinyusb_config_t con NCM descriptor (VID/PID personalizados, subclass 0x0D).
- Flow: Init driver, attach netif, set IP, callbacks for events.
- Robustez: Reconexión <5s, SAFE MODE <3s, logs ESP_LOGI.
- Inspiración: Chegewara/esp-usb-net (wrapper simple).

## 5. Trazabilidad
- Changelog: Registra todos los cambios en docs/changelog.md.
