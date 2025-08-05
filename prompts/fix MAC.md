# 🚀 Tarea — USB-NCM MAC Address Fix

## 1. Contexto clave
- **Proyecto**: Aqua Controller USB.  
- **Plataforma**: ESP32-S3 · ESP-IDF v5.5 · TinyUSB (modo NCM).  
- **Estado actual**: El stack TinyUSB reporta _“failed to get mac address / bind() failure”_ y el núcleo Linux no crea `usb0`.  
- **Reglas fundacionales**:  
  - Un único firmware para AC/DC/IO → el *rol* del panel se graba en NVS al primer arranque.  
  - Todo módulo debe arrancar autónomamente y “local-first” sin depender de la nube.  
- **Doc referencial**: [`docs/Aqua_Controller_USB_v5.2.md`](../Aqua_Controller_USB_v5.2.md) — arquitectura completa y lineamientos versión 5.2.

## 2. Objetivo
Implementar una **asignación determinística de MAC address** para la interfaz USB-NCM y exponerla en el descriptor TinyUSB, garantizando que:
1. Cada panel tenga una MAC única, reproducible a partir de su chip-ID (o eFuse).  
2. Sea una dirección **locally-administered** (bit 1 del primer octeto = 1, bit 0 = 0).  
3. No incremente el consumo de RAM ni ROM de forma significativa (< 256 B).  
4. Se conserve la lógica de rol y la lectura de configuración vía `bios_config` (NVS).  

## 3. Entregables
| Ítem | Descripción |
|------|-------------|
| **Código fuente** | Nuevo módulo `usb_mac.c/.h` dentro de `components/usb_comm_aq/` + patch en `tusb_descriptors.c`. |
| **Tests** | Prueba unitaria (Unity) que valide el algoritmo de MAC vs. `esp_efuse_mac_get_default()`. |
| **Docs** | Actualizar `docs/firmware_arch.md` sección “USB-NCM Init”. |
| **CI** | Añadir job a GitHub Actions para compilar con `idf.py build` en modo `esp32s3`. |

## 4. Restricciones técnicas
- **C** puro (no C++).  
- Sin asignaciones dinámicas (`malloc`).  
- Mantener compatibilidad con `idf.py menuconfig > Component config > Aqua → Panel role`.  
- El algoritmo debe funcionar aun si NVS está virgen (primer arranque).  
- Cumplir MISRA-C-2012 esencial (máx. 10 violaciones justificadas).

## 5. Pasos sugeridos (Gemini)
1. Leer `components/usb_comm_aq/tusb_descriptors.c` y detectar el placeholder MAC.  
2. Crear helper `make_panel_mac(uint8_t mac[6])` que:  
   - Obtiene el OUI base _“xAI”_ (`02:03:3A`) o genera un OUI `0x02,0xXX,0xYY` derivado del chip-ID.  
   - Usa el chip-ID (o CRC16) para los tres bytes bajos.  
3. Incluir llamada en `usb_comm_aq_init()` antes de `tusb_init()`.  
4. Persistir la MAC en NVS (namespace `bios_cfg`, key `usb_mac`) tras la primera generación.  
5. Documentar cómo flashear y verificar con `lsusb`, `dmesg`, `ip link`.

## 6. Criterios de aceptación
- `idf.py flash monitor` muestra la MAC calculada al arrancar.  
- En host Linux aparece interfaz `usb0` y responde a `ping` desde `192.168.88.2` (config estática).  
- Cobertura de tests > 90 % para las funciones nuevas.  
- Footprint:<br>  Flash ≤ +2 KiB ·  RAM ≤ +256 B.

## 7. Formato de salida esperado de Gemini CLI
```diff
+++ components/usb_comm_aq/usb_mac.c
+++ components/usb_comm_aq/usb_mac.h
+++ components/usb_comm_aq/tusb_descriptors.c
+++ test/test_usb_mac.c
