# 🛠️ TAREA: Corregir error "Failed to get device role" al iniciar APP_LOGIC_AQ

## 🎯 OBJETIVO
Corregir el error reportado por el componente `APP_LOGIC_AQ`:  
```json
{"timestamp": 340194, "severity": "ERROR", "tag": "APP_LOGIC_AQ", "message": "Failed to get device role."}
```
El objetivo es garantizar que `bios_config_aq` entregue correctamente el valor del campo `"bios.role"` al módulo `app_logic_aq` en tiempo de ejecución.

---

## 🧩 CONTEXTO
El componente `bios_config_aq` administra la configuración persistente usando NVS. Una de sus claves críticas es:

- `"bios.role"`: debe contener el rol del dispositivo actual (`"MASTER"`, `"PANEL_DC"`, etc.)

Actualmente, la carga de este valor falla. Se sospecha que:

1. La clave `"bios.role"` no existe aún en NVS.
2. El código no maneja fallback ni retorno de error correctamente.
3. `app_logic_aq` no valida si el resultado de `bios_get_config("bios.role")` es válido.

---

## ✅ CRITERIOS DE ACEPTACIÓN

- Si `"bios.role"` no está configurado, el sistema debe:
  - Retornar `"UNKNOWN"` como valor por defecto **o** fallar con log claro.
  - Permitir inicializar el sistema (modo seguro o solo log).
- El error debe desaparecer del log y ser reemplazado por un mensaje informativo o warning controlado.
- La lógica debe permitir luego establecer el valor real desde la RPi vía JSON CDC/NCM.

---

## 🧪 PRUEBAS ESPERADAS
- Simular inicio sin valor `"bios.role"` y verificar que el sistema entra en fallback controlado.
- Establecer el valor desde un mensaje CDC JSON:
```json
{ "command": "bios.set", "key": "bios.role", "value": "PANEL_DC" }
```
y confirmar que se guarda en NVS.
- Reiniciar y validar que se carga correctamente el nuevo rol desde `bios_config_aq`.

---

## 💡 SUGERENCIAS
- Usar `bios_get_config("bios.role", &out)` y verificar retorno (`ESP_OK` vs `ESP_ERR_NVS_NOT_FOUND`).
- Implementar `bios_get_device_role()` como wrapper con fallback a `"UNKNOWN"` o `"UNSET"`.
- Añadir logs informativos (`ESP_LOGW`) si el rol no está definido aún.

---

## 🧩 ARCHIVOS INVOLUCRADOS
- `components/bios_config_aq/bios_config_aq.c`
- `components/app_logic_aq/app_logic_aq.c`
