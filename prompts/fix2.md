# Prompt para Corrección de Crash en esp_netif_attach para USB-NCM en AquaControllerUSB_v5.2

**Contexto del Proyecto (Breve)**:  
AquaControllerUSB_v5.2 (ESP-IDF v5.5, ESP32-S3). Firmware con USB-NCM para MQTT, configs/roles/strings en NVS, SAFE MODE autónomo. Optimiza IRAM <80%, evita anti-patrones (no nube/Wi-Fi crítico, no hardcode). El monitor muestra crash (Guru Meditation LoadProhibited) en esp_netif_attach después de TinyUSB Driver installed, probable por esp_netif_t * nulo o glue incorrecto. NVS ya carga OK, pero secuencia de netif/init/glue errónea.

**Tarea Específica**:  
Modificar `main.c` para llamar esp_netif_init() y esp_event_loop_create_default() temprano. Actualizar `usb_comm_aq.c` para crear esp_netif_new, tinyusb_driver_install, tinyusb_net_init, esp_netif_attach con glue correcto (tinyusb_net_get_lwip_netif_glue), manejar NULL/errors. Usar MAC/strings de NVS/defaults cortos.

**Requisitos Clave**:  
1. **Librerías**: esp_netif, esp_event, esp_tinyusb, nvs_flash (nativos).  
2. **Diseño**: /main/main.c mínimo (inits secuenciales), /components/usb_comm_aq/usb_comm_aq.c (init NCM seguro, sufijo _aq). Defaults: manufacturer "xAI", product "AquaPanel", serial "001", MAC {0x02,0x00,0x04,0x00,0x00,0x00}. Limitar strings a 3-4.  
3. **Optimizaciones**: IRAM <80% (idf.py size). Reconexión <5s. Logs JSON para errors. PSRAM si buffers grandes.  
4. **APIs**: usb_comm_aq_init() (esp_netif_new/check NULL, tinyusb_driver_install, tinyusb_net_init con descriptor/MAC/netif, esp_netif_attach con tinyusb_net_get_lwip_netif_glue(netif)).  
5. **Integración/Testing**: Secuencia en main.c: nvs_flash_init → config_aq_init → esp_netif_init → esp_event_loop_create_default → usb_comm_aq_init. Unity stubs para test netif create/attach/error. Validar idf.py monitor sin crash (ver "USB-NCM connected" o similar).

**Auto-Validación Requerida**:  
- Confirmar esp_netif_init y glue resuelven LoadProhibited (no null en attach).  
- Verificar netif_new != NULL, MAC/strings cargados.  
- Simula idf.py size (IRAM <80%).  
- Cumplimiento anti-patrones: configs NVS, no hardcode, autónomo.  
- Simula log: Netif created, attached OK, no crash.

**Salida Esperada**:  
- Código C completo para `main.c` (#includes, app_main con secuencia, error handling).  
- Modificaciones a `usb_comm_aq.c` (define esp_netif_config_t = ESP_NETIF_DEFAULT_NETIF(), netif_new, tinyusb_net_init, attach con glue, free en deinit).  
- Comentarios.  
- Verificación final: Explica cumplimiento (secuencia ejemplo tusb_ncm evita null, strings limitados).

**Referencias**:  
- [USB Device API](https://docs.espressif.com/projects/esp-idf/en/v5.5/esp32s3/api-reference/peripherals/usb_device.html).  
- [ESP Netif API](https://docs.espressif.com/projects/esp-idf/en/v5.5/esp32s3/api-reference/network/esp_netif.html).  
- [TUSB NCM Example](https://github.com/espressif/esp-idf/tree/v5.5/examples/peripherals/usb/device/tusb_ncm) (secuencia: netif_init, event_loop, netif_new, driver_install, net_init, attach con glue).  
- [Error Codes](https://docs.espressif.com/projects/esp-idf/en/v5.5/esp32s3/api-reference/error-codes.html).

Genera y confirma.