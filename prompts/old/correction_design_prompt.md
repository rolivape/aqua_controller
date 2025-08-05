Lee y sigue estrictamente /mnt/aqua_repo/aqua_controller/docs/Gemini.md como referencia principal.

**Contexto del Proyecto (Breve)**: AquaControllerUSB_v5.2 (ESP-IDF v5.5, ESP32-S3). Sigue documento maestro v5.2, mapa componentes/APIs, Reglas Diseño (Capítulo 9), Anexo 10.5 librerías válidas. Optimiza IRAM <80%, NVS configs, MQTT/NCM, SAFE MODE autónomo. Anti-patrones: No nube/Wi-Fi crítico, no dependencias pesadas.

**Tarea Específica**: Refactoriza para corregir errores de diseño: Mueve componentes a /esp32_firmware/components/, renombra esp32_firmware.c a main.c y hazlo mínimo (solo init y loop, lógica en módulos). Código actual a refactorizar:
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "config_aq.h"
#include "json_log.h"

static const char *TAG = "AQUA_MAIN";

void app_main(void)
{
    json_log(ESP_LOG_INFO, TAG, "Starting AquaControllerUSB v5.2...", NULL);

    nvs_handle_t config_aq_handle;
    esp_err_t err = config_aq_init(&config_aq_handle);

    if (err != ESP_OK) {
        json_log(ESP_LOG_ERROR, TAG, "Failed to initialize config_aq component.", esp_err_to_name(err));
        json_log(ESP_LOG_ERROR, TAG, "Halting application.", NULL);
        while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    } else {
        json_log(ESP_LOG_INFO, TAG, "config_aq component initialized successfully.", NULL);
    }

    char device_role[32];
    err = config_aq_get_role(device_role, sizeof(device_role));

    if (err == ESP_OK) {
        json_log(ESP_LOG_INFO, TAG, "Retrieved device role.", device_role);
    } else {
        json_log(ESP_LOG_ERROR, TAG, "Failed to get device role.", esp_err_to_name(err));
    }
    
    // The handle can be closed here if it's not needed elsewhere
    nvs_close(config_aq_handle);
    json_log(ESP_LOG_INFO, TAG, "config_aq handle closed.", NULL);
}

**Requisitos Clave**:
1. Librerías: nvs_flash nativo, cJSON para JSON.
2. Diseño: Componentes en /esp32_firmware/components/, sufijo _aq, no hardcode, comentarios bloque.
3. Optimizaciones: Bajo impacto IRAM, NVS para roles.
4. APIs: Mantén esp_err_t config_aq_init(nvs_handle_t *handle); esp_err_t config_aq_get_role(char *role, size_t len).
5. Integración/Testing: Llama desde main.c; stubs Unity para NVS read; mueve lógica a módulos.

**Auto-Validación Requerida**:
- Confirma cumplimiento IRAM, v5.5 compatibilidad, anti-patrones.
- Simula output idf.py size (conceptual).

**Salida Esperada**:
- Código C completo para main.c mínimo (#includes, funciones, error handling).
- Modificaciones en módulos.
- Comentarios.
- Verificación final.
- Compila con `idf.py build` en /esp32_firmware/.
- Lista errores si falla (detalle: archivo, línea, mensaje).

**Referencias**: https://docs.espressif.com/projects/esp-idf/en/v5.5/esp32s3/api-reference/storage/nvs_flash.html.

Genera y confirma.