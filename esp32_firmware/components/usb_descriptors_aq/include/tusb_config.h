#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------------
// COMMON CONFIGURATION
//--------------------------------------------------------------------
#define CFG_TUSB_MCU                OPT_MCU_ESP32S3
#define CFG_TUSB_OS                 OPT_OS_FREERTOS
#define CFG_TUSB_DEBUG              0
#define CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_ALIGN          __attribute__((aligned(4)))

//--------------------------------------------------------------------
// DEVICE CONFIGURATION
//--------------------------------------------------------------------
#define CFG_TUD_ENABLED             1
#define CFG_TUD_MAX_SPEED           TUSB_SPEED_HIGH
#define CFG_TUD_ENDPOINT0_SIZE      64

//------------- CLASS-SPECIFIC -------------//
#define CFG_TUD_CDC                 1
#define CFG_TUD_NCM                 1
#define CFG_TUD_NET_MTU             1514

#ifdef __cplusplus
 }
#endif

#endif /* _TUSB_CONFIG_H_ */
