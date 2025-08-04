# Contexto General para Gemini CLI - AquaControllerUSB_v5.2

## Proyecto
- Nombre: AquaControllerUSB_v5.2
- Plataforma: ESP-IDF v5.5, ESP32-S3
- Comunicación: MQTT sobre USB-NCM
- Configuraciones: NVS
- Consola: Flask en RPi5
- Optimizaciones: IRAM <80%, PSRAM buffers
- Anti-patrones: No nube/Wi-Fi crítico, no propietario, no dependencias pesadas (e.g., no esp_console)

## Reglas de Diseño (Capítulo 9)
- main.c mínimo, lógica en módulos con sufijo _aq
- No hardcode, configs en NVS
- Modularidad: /components/, /include/, /tests/
- Clang-Format: Estilo LLVM, IndentWidth 4
- Tests: Unity/CMock, Pytest
- Logs JSON

## Librerías Válidas (Anexo 10.5)
- nvs_flash, esp_tinyusb (NCM), esp-mqtt, cJSON

## Instrucciones
- Ejecuta prompts desde /mnt/aqua_repo/aqua_controller/prompts/
- Usa sintaxis: gemini @/mnt/aqua_repo/aqua_controller/prompts/<nombre_prompt>.md
- Salida: Código C en /components/<nombre>_aq/, con verificación IRAM
- Compila el código generado con `idf.py build` en /mnt/aqua_repo/aqua_controller/esp32_firmware/
- Lista errores encontrados si la compilación falla (detalle: archivo, línea, mensaje)

## Referencias
- Documento Maestro: docs/Aqua_Controller_USB_v5.2.md
- Mapa Componentes: docs/mapa_componentes.md (crear si no existe)
