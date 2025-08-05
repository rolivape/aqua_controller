**Contexto del Proyecto (Breve)**: AquaControllerUSB_v5.2 (ESP-IDF v5.5, ESP32-S3). Sigue documento maestro v5.2, mapa componentes/APIs, Reglas Diseño (Capítulo 9), Anexo 10.5 librerías válidas. Optimiza IRAM <80%, NVS configs, MQTT/NCM, SAFE MODE autónomo. Anti-patrones: No nube/Wi-Fi crítico, no dependencias pesadas.

**Tarea Específica**: Genera config_aq.c para inicialización NVS y asignación de roles.

**Requisitos Clave**:
1. Librerías: nvs_flash nativo.
2. Diseño: /components/config_aq/, sufijo _aq, no hardcode, comentarios bloque.
3. Optimizaciones: Bajo impacto IRAM, NVS para roles.
4. APIs: esp_err_t config_aq_init(nvs_handle_t *handle); esp_err_t config_aq_get_role(char *role, size_t len).
5. Integración/Testing: Llama desde main.c; stubs Unity para NVS read.

**Auto-Validación Requerida**:
- Confirma cumplimiento IRAM, v5.5 compatibilidad, anti-patrones.
- Simula output idf.py size (conceptual).

**Salida Esperada**:
- Código C completo (#includes, funciones, error handling).
- Comentarios.
- Verificación final.
- Compila el código generado con `idf.py build` en /mnt/aqua_repo/aqua_controller/esp32_firmware/.
- Lista errores encontrados si la compilación falla (detalle: archivo, línea, mensaje).

**Referencias**: https://docs.espressif.com/projects/esp-idf/en/v5.5/esp32s3/api-reference/storage/nvs_flash.html.