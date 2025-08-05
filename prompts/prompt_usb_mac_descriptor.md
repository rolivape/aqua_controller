Título: Implementar descriptores USB funcionales para CDC-NCM (basado en ejemplo de Chegewara)


🔧 TAREA
Corregir e implementar los descriptores USB necesarios en el componente `usb_descriptors_aq` del proyecto AquaControllerUSB, asegurando compatibilidad total con CDC-NCM en ESP32-S3 y detección funcional en el host (RPi). La implementación debe estar inspirada directamente en el ejemplo `chegewara/usb-netif`.

📂 CONTEXTO DEL PROYECTO
- Proyecto modular en ESP-IDF v5.4.1 con ESP32-S3
- TinyUSB usado como backend USB
- El dispositivo debe funcionar como interfaz de red USB usando CDC-NCM
- Ya existe el componente `usb_descriptors_aq`, pero la enumeración falla con:
  ```log
  cdc_ncm: failed to get mac address
  cdc_ncm: bind() failure
El string descriptor de idioma (index 0) está mal formado (copiado como ASCII en lugar de UTF-16)

El campo .iMACAddress = 4 ya está presente en TUD_CDC_NCM_DESCRIPTOR

📎 REFERENCIA
Tomar como guía el componente publicado en components.espressif.com/components/chegewara/usb-netif. Este ejemplo ya resuelve el problema correctamente.

🎯 OBJETIVO
Modificar y completar el componente usb_descriptors_aq para cumplir con lo siguiente:

Implementar tud_descriptor_device_cb() y tud_descriptor_configuration_cb() con los mismos valores y estructura que el ejemplo usb-netif

Implementar correctamente tud_descriptor_string_cb(), codificando UTF-16 válidos, especialmente para index 0 (idioma = 0x0409)

Asegurarse de que el string descriptor de MAC está en el índice 4 y definido como "02006414CFAC"

Implementar tud_network_mac_address_cb() para retornar [0x02, 0x00, 0x64, 0x14, 0xcf, 0xac]

Usar sufijos _aq en funciones y archivos según convención del proyecto

Modularizar el código en:

usb_descriptors_aq.c

usb_descriptors_aq.h

include/usb_descriptors_aq.h

CMakeLists.txt

📁 UBICACIÓN
Todos los archivos deben ubicarse dentro de:


esp32_firmware/components/usb_descriptors_aq/
🧩 CONDICIONES

No modificar main.c

No implementar lógica de red ni MQTT

Mantener el uso de CFG_TUD_NET_MTU como macro

Agregar comentarios breves sobre cada descriptor

🧪 VALIDACIÓN ESPERADA
Al flashear y conectar el ESP32-S3 al host (RPi), el log de dmesg debe mostrar:


cdc_ncm: MAC-Address: 02:00:64:14:cf:ac
cdc_ncm: network interface usb0 created
📎 NOTAS EXTRA

No hace falta definir múltiples idiomas, solo soportar 0x0409 (inglés US)

No utilizar memcpy ni cadenas ASCII para index 0

No usar string_desc_arr[0] como string, definir idioma explícitamente como UTF-16

🔚 SALIDA ESPERADA
Entrega completa de:

Código funcional del componente usb_descriptors_aq

Archivos listos para compilar

Comentarios explicativos

Enumeración CDC-NCM 100% operativa en host