Lee y sigue estrictamente /mnt/aqua_repo/aqua_controller/docs/Gemini.md y /mnt/aqua_repo/aqua_controller/docs/Librerias_Valida.md como referencias principales.

**Contexto del Proyecto (Ampliado)**: AquaControllerUSB_v5.2 es local-first para acuarios, MASTER RPi5, paneles ESP32-S3. ESP-IDF v5.5, IRAM <80% con PSRAM, NVS configs (roles, IPs), MQTT sobre USB-NCM (latencia <50ms, reconexión <5s), SAFE MODE <3s. Sigue documento maestro v5.2 (local-first, migración NCM, anti-patrones no nube/Wi-Fi, visión IA local). Mapa: main.c mínimo, módulos _aq (usb_comm_aq, rules_engine_aq). Lecciones (Anexo 10.5): esp_tinyusb + esp_netif para NCM (no tusb_cdc_ncm.h, fixes v5.5). Inspiración: Cheguewara/esp-usb-net y esp32-open-source/usb-netif-example para wrapper simple (descriptores NCM, VID/PID personalizados, subclass 0x0D, callbacks).

**Tarea Específica**: Genera usb_comm_aq.c para inicialización USB-NCM robusta, con logs ESP_LOGI para puntos de control/debugging (init, connect, errors).

**Requisitos Clave**:
1. Librerías: esp_tinyusb, esp_netif nativos.
2. Diseño: /esp32_firmware/components/usb_comm_aq/, sufijo _aq, no hardcode, comentarios bloque, logs ESP_LOGI en puntos clave.
3. Optimizaciones: Bajo impacto IRAM, reconexión automática <5s, buffers en PSRAM.
4. APIs: esp_err_t usb_comm_aq_init(const char *panel_id, const char *ip_addr); callback for USB events.
5. Integración/Testing: Llama desde main.c; stubs Unity for NCM init; test reconexión <5s.

**Auto-Validación Requerida**:
- Confirma cumplimiento IRAM, v5.5 compatibilidad, anti-patrones.
- Simula output idf.py size (conceptual).

**Salida Esperada**:
- Código C completo (#includes, funciones, error handling).
- Comentarios.
- Verificación final.
- Compila con `idf.py build` en /esp32_firmware/.
- Lista errores si falla (detalle: archivo, línea, mensaje).

**Referencias**: https://docs.espressif.com/projects/esp-idf/en/v5.5/esp32s3/api-reference/peripherals/usb.html; https://github.com/chegewara/esp-usb-net; https://github.com/esp32-open-source/usb-netif-example.

Genera y confirma.