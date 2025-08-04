Contexto del Proyecto (Breve):
AquaControllerUSB_v5.2 utiliza ESP-IDF v5.5 y ESP32-S3 para control de acuarios marinos. La comunicación se basa en USB-NCM (librería esp_tinyusb) para MQTT, con configuraciones en NVS y SAFE MODE autónomo. Se optimiza IRAM (<80%), evita Wi-Fi crítico y dependencias pesadas, y sigue las Reglas de Diseño de Software (Capítulo 9, AquaController_v52.md) y Anexo 10.5 para librerías válidas. El programa compila, pero el monitor reporta los siguientes errores:





E (341) tusb_desc: tinyusb_set_descriptors(238): String descriptors exceed limit



E (341) TinyUSB: tinyusb_driver_install(90): Descriptors config failed

Tarea Específica:
Corregir los errores de configuración de descriptores USB en el componente usb_comm_aq para esp_tinyusb (NCM), ajustando la inicialización de descriptores y asegurando compatibilidad con ESP-IDF v5.5 y ESP32-S3. Generar o modificar el archivo usb_comm_aq.c con las correcciones necesarias.

Requisitos Clave:





Librerías: Usar solo esp_tinyusb (nativo en ESP-IDF v5.5, ref: [docs.espressif.com/projects/esp-idf/en/v5.5/esp32s3/api-reference/peripherals/usb_device.html]). Evitar dependencias externas o forks de TinyUSB.



Diseño:





Código en /components/usb_comm_aq/usb_comm_aq.c.



Sufijo _aq en funciones y archivos (e.g., usb_comm_aq_init()).



Configuraciones (e.g., strings USB) en NVS, no hardcodeadas.



Optimizaciones:





Minimizar uso de IRAM (validar con idf.py size).



Asegurar reconexión USB-NCM en <5s tras fallo.



Usar PSRAM para buffers si es necesario.



APIs:





usb_comm_aq_init(): Inicializa TinyUSB con descriptores corregidos.



usb_comm_aq_get_status(): Reporta estado de conexión NCM.



usb_comm_aq_deinit(): Libera recursos USB.



Integración/Testing:





Llamar usb_comm_aq_init() desde main.c.



Implementar stubs para pruebas unitarias con Unity/CMock.



Validar conexión NCM con tres módulos (latencia MQTT <50ms).

Auto-Validación Requerida:





Confirmar que los errores de descriptores (tusb_desc y tinyusb_driver_install) están resueltos.



Verificar compatibilidad con ESP-IDF v5.5 y ESP32-S3 (GPIOs 19/20 para USB D-/D+).



Simular output de idf.py size (IRAM <80%, conceptual).



Asegurar cumplimiento de anti-patrones: sin nube, sin dependencias pesadas, sin hardcodeo.



Validar estabilidad de USB-NCM con reconexión automática (esp_netif reinit).

Salida Esperada:





Código C completo para /components/usb_comm_aq/usb_comm_aq.c con:





Includes necesarios (e.g., esp_tinyusb.h, nvs_flash.h).



Corrección de descriptores USB (limitando strings a especificaciones válidas).



Manejo de errores robusto (logs JSON, reintentos).



Comentarios claros explicando cada sección.



Código de prueba en /tests/test_usb_comm_aq.c para Unity/CMock.



Verificación final: Explicar cómo se resuelve el error de descriptores y se valida IRAM.

Referencias:





ESP-IDF v5.5 USB Device



ESP-IDF TinyUSB NCM Example



TinyUSB Descriptor Configuration



NVS Flash API



Detalles del Problema y Solución Esperada

Errores Reportados





String descriptors exceed limit: Indica que los descriptores de cadena USB (e.g., manufacturer, product, serial number) superan el límite permitido por TinyUSB o no están bien formados.



Descriptors config failed: Sugiere un problema en la configuración de tinyusb_config_t o en la llamada a tinyusb_driver_install(), posiblemente por descriptores mal definidos o falta de inicialización.

Solución Propuesta





Revisar descriptores USB: Asegurar que las cadenas (manufacturer, product, serial) sean cortas (<64 caracteres cada una, per TinyUSB) y estén correctamente definidas en tusb_config_descriptor_t.



Usar NVS para configuraciones: Almacenar strings USB en NVS (e.g., usb_manufacturer, usb_product) para evitar hardcodeo y permitir personalización por panel.



Inicialización robusta: Configurar tinyusb_config_t con valores mínimos y validados, usando tusb_ncm como referencia (ver ejemplo ESP-IDF).



Manejo de errores: Implementar reintentos en tinyusb_driver_install() y logs JSON para depuración.



Optimización IRAM: Asegurar que las funciones críticas (e.g., callbacks USB) no consuman IRAM excesivo; usar CONFIG_SPI_FLASH_AUTO_SUSPEND si aplica.

Pasos para Gemini CLI





Analizar el ejemplo tusb_ncm de ESP-IDF v5.5 para estructura de descriptores.



Generar usb_comm_aq.c con inicialización de TinyUSB, limitando strings a especificaciones válidas.



Incluir lectura de NVS para strings USB (e.g., nvs_get_str()).



Implementar manejo de errores con logs JSON (esp_log_write() con formato { "timestamp": "...", "severity": "ERROR", "message": "..." }).



Crear pruebas unitarias en test_usb_comm_aq.c para validar inicialización y estado.



Verificar IRAM con simulación conceptual de idf.py size (estimar <80% uso).

Observaciones de Chief Programmer & PMO:





Priorizar solución ligera y alineada con esp_tinyusb.



Evitar modificaciones a esp_tinyusb interno; usar solo APIs públicas.



Validar en ESP32-S3 con USB-NCM activo (IP estática 192.168.1.x).



Usar clang-format para consistencia de estilo.



Confirmar que el código generado respeta las Reglas de Diseño (Capítulo 9) y evita anti-patrones (sin hardcodeo, sin dependencias externas).

Instrucción Final:
Gemini CLI, genera el código para usb_comm_aq.c y test_usb_comm_aq.c, asegurando que resuelve los errores de descriptores USB, optimiza IRAM, y cumple con los requisitos del proyecto. Incluye comentarios claros y verifica cumplimiento con ESP-IDF v5.5. Simula el output de idf.py size conceptualmente y explica cómo se resuelve cada error.