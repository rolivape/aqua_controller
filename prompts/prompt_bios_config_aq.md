# Prompt 1 - Componente bios_config_aq

## ✅ Objetivo

Desarrollar la estructura inicial del componente `bios_config_aq`, encargado de gestionar configuración persistente mediante NVS. Este módulo será utilizado por los módulos ESP32-S3 del sistema AquaControl_USB para leer y escribir parámetros clave como identificador del módulo, tipo, configuración de red, etc.

## 🧠 Contexto

El sistema está estructurado en componentes con sufijo `_aq`. El archivo `main.c` no debe contener lógica de inicialización directa ni procesamiento de configuraciones. Toda configuración debe ser delegada a componentes especializados. El componente `bios_config_aq` será responsable de:

- Inicializar NVS
- Cargar datos persistentes desde NVS (ej. `device_id`, `device_type`)
- Permitir guardar nuevas configuraciones
- Retornar JSONs con el estado de configuración

Este módulo se ejecuta en firmware para ESP32-S3 bajo ESP-IDF 5.4.1.

## 🛠️ Instrucciones para Gemini

1. Crear el componente `bios_config_aq` con la siguiente estructura de archivos:
   ```
   components/
   └── bios_config_aq/
       ├── CMakeLists.txt
       ├── Kconfig
       ├── include/
       │   └── bios_config_aq.h
       └── bios_config_aq.c
   ```

2. En `bios_config_aq.c`:
   - Implementar `bios_config_aq_init()` para inicializar NVS.
   - Implementar `bios_config_aq_load()` que lea desde NVS las claves `device_id` y `device_type`.
   - Implementar `bios_config_aq_save()` que permita guardar un nuevo `device_id`.

3. En `bios_config_aq.h`:
   - Declarar las funciones públicas del componente.
   - Incluir estructuras necesarias para almacenar la configuración cargada.

4. Utilizar las funciones oficiales de ESP-IDF:
   - `nvs_open`, `nvs_get_str`, `nvs_set_str`, `nvs_commit`, `nvs_close`.

5. El componente debe incluir soporte para devolver la configuración como JSON (cadena formateada o `cJSON` si ya está disponible).

## ⚠️ Restricciones

- El código debe estar orientado a producción, no incluir prints innecesarios.
- No debe haber lógica de fallback o defaults codificados por ahora.
- No usar `static` si la función será usada desde `main.c`.
- Usar prefijos consistentes `bios_config_aq_`.

## 📤 Output Esperado

Entrega como un **diff de git** o bloque de código completo para los archivos:

- `bios_config_aq.c`
- `bios_config_aq.h`
- `CMakeLists.txt`
- `Kconfig`
