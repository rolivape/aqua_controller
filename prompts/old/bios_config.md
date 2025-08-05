# Prompt Gemini: Componente `bios_config_aq`

## Objetivo
Implementar el componente `bios_config_aq` para inicializar la configuración del dispositivo desde NVS. Este componente se usará en el arranque para leer parámetros persistentes, como el rol del dispositivo, y exponerlos de manera estructurada al resto del firmware.

## Contexto Técnico
- Sistema embebido basado en ESP32-S3.
- Proyecto modular con componentes tipo `*_aq`.
- La configuración se guarda en NVS y se serializa/deserializa desde estructuras C.
- El flujo requiere una función pública `bios_config_aq_load()` que devuelva una estructura tipo `bios_config_t` y maneje errores de forma robusta.

## Reglas y convenciones
- `main.c` debe permanecer limpio; este componente será llamado desde `app_logic_aq`.
- El archivo fuente será `bios_config_aq.c`, con su correspondiente `bios_config_aq.h`.
- El namespace de funciones públicas será `bios_config_aq_*`.
- Logging debe hacerse mediante `json_log_aq`.
- En caso de error en la carga de NVS, se debe reportar con severidad `ERROR` y abortar la inicialización.
- Se permite definir configuraciones por defecto si no existe entrada previa en NVS.

## Ejemplo de uso esperado

```c
bios_config_t config;
esp_err_t err = bios_config_aq_load(&config);
if (err != ESP_OK) {
    json_log_aq_error("APP_LOGIC_AQ", "Failed to load BIOS configuration.");
    return;
}
```

## Archivos esperados
- `components/bios_config_aq/bios_config_aq.c`
- `components/bios_config_aq/include/bios_config_aq.h`

## Extras
- Documentar la estructura `bios_config_t` con comentarios en el header.
- Usar `nvs_flash.h` y `nvs.h` del ESP-IDF.
- Modularizar si se anticipan futuras claves o esquemas JSON.

## Estilo
- C (ESP-IDF v5.5).
- Compatible con ESP32-S3.
- Uso explícito de `esp_err_t` para propagación de errores.
