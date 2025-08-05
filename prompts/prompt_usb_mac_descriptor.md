# Prompt: Insertar String Descriptor para MAC en USB-NCM

## Objetivo:
Corregir el error `cdc_ncm: failed to get mac address` detectado en la RPi cuando se conecta el ESP32-S3 vía USB-NCM, asegurando que el índice `iMacAddress` en el descriptor USB retorne un string válido desde `tud_descriptor_string_cb`.

## Contexto:
Estamos desarrollando un firmware modular para ESP32-S3 que expone una interfaz USB tipo CDC-NCM (Ethernet over USB) usando TinyUSB. El identificador de dispositivo reportado es:

- `idVendor=0x303a`
- `idProduct=0x4008`
- Producto: `AquaPanel`

Al cargar el firmware, la salida de `lsusb -v -d 303a:4008` en la RPi muestra:
```
CDC Ethernet:
  iMacAddress                      5 (??)
```
Y el kernel lanza:
```
cdc_ncm 1-2:1.0: failed to get mac address
cdc_ncm 1-2:1.0: bind() failure
```

Esto indica que aunque se definió `iMacAddress = 5` en el descriptor, no se está devolviendo correctamente el string en `tud_descriptor_string_cb()`.

## Tarea:
Modificar el archivo donde se implementa el callback `tud_descriptor_string_cb()` (normalmente en `usb_desc.c` o `usb_netif_aq`) para que cuando `index == 5`, se devuelva un descriptor UTF-16 válido con el valor hexadecimal de la MAC address.

## Restricciones:
- Utilizar un string de 12 caracteres hexadecimales, sin `:` ni espacios. Ejemplo: `"02006414CFAC"`.
- El string debe codificarse en UTF-16 como lo exige TinyUSB.
- Se debe retornar un puntero válido a un `uint16_t desc_str[]` como respuesta al callback.
- No deben romperse los índices existentes para iProduct, iManufacturer, etc.
- El código debe compilar sin warnings en ESP-IDF 5.5 para target `esp32s3`.

## Ejemplo esperado:
```c
case 5: { // iMacAddress string descriptor
  const char* mac_str = "02006414CFAC";
  chr_count = strlen(mac_str);
  for (uint8_t i = 0; i < chr_count; i++) {
    desc_str[1 + i] = mac_str[i];
  }
  desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2 * chr_count + 2);
  return desc_str;
}
```

## Archivos involucrados:
- `components/usb_netif_aq/usb_desc.c` (o ubicación equivalente en el proyecto)
- Asegurar que esta implementación no interfiera con la configuración dinámica de MAC en `bios_config_aq` si se usa más adelante.

## Formato de salida esperado:
- Código completo y limpio del callback `tud_descriptor_string_cb()`, incluyendo la nueva entrada para `index == 5`.
- Confirmación de que el código sigue la convención UTF-16 requerida por TinyUSB.
- Breve resumen de validación y cómo verificarlo con `lsusb -v` y `dmesg`.
