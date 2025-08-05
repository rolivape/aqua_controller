TAREA
Crear un nuevo componente llamado `usb_descriptors_aq` dentro del proyecto AquaControllerUSB. Este componente debe definir y sobrescribir completamente los descriptores USB necesarios para que el ESP32-S3 funcione correctamente como dispositivo CDC-NCM (Ethernet sobre USB).

📂 CONTEXTO DEL PROYECTO
- Proyecto modular para ESP32-S3 bajo ESP-IDF v5.4.1
- Se usa TinyUSB como backend USB
- No hay descriptores definidos por el usuario aún (solo `tud_descriptor_string_cb()` está implementado en otro archivo)
- El sistema host (una Raspberry Pi) falla con: `cdc_ncm: failed to get mac address`
- Esto indica que falta el campo `.iMACAddress` en el descriptor de red, y que los descriptores default de TinyUSB no son suficientes

🎯 OBJETIVO
Crear el componente `usb_descriptors_aq` con las siguientes funciones y definiciones:

1. `tud_descriptor_device_cb()` → devuelve descriptor de dispositivo (`tusb_desc_device_t`)
2. `tud_network_mac_address()` → devuelve MAC fija en binario: `0x02, 0x00, 0x64, 0x14, 0xcf, 0xac`
3. `tud_network_descriptor` o equivalente → incluir el campo `.iMACAddress = 4`
4. Toda la estructura debe usar nombres con sufijo `_aq` y ser modular
5. No modificar `main.c`
6. Documentar cada parte brevemente

📁 ESTRUCTURA ESPERADA

Componente nuevo: `components/usb_descriptors_aq/`

Debe contener:
- `usb_descriptors_aq.c`
- `usb_descriptors_aq.h`
- `CMakeLists.txt`
- `include/usb_descriptors_aq.h`

Este componente será luego vinculado desde `usb_netif_aq` y `usb_comms_aq`.

🧩 CONDICIONES DE IMPLEMENTACIÓN

- No usar variables globales innecesarias
- Código limpio, comentado y portable
- Incluir `#pragma once` en el encabezado
- No asumir la existencia de macros no definidas, usa valores explícitos
- Agregar en `CMakeLists.txt` el `src` y `include` correspondiente
- El `.iMACAddress` debe ser 4 y coincidir con el string descriptor ya implementado

🧪 VALIDACIÓN ESPERADA

Después de flashear, el host (Raspberry Pi) debe reconocer correctamente:

cdc_ncm: MAC-Address: 02:00:64:14:cf:ac
cdc_ncm: network interface usb0 created



📎 NOTAS

- La dirección MAC puede ser fija por ahora
- El string descriptor index 4 ya está implementado en `tud_descriptor_string_cb()`, y contiene `02006414cfac`

🔚 SALIDA ESPERADA

Entrega completa del componente `usb_descriptors_aq` con todos los archivos mencionados, sin errores de compilación y con comentarios breves.
