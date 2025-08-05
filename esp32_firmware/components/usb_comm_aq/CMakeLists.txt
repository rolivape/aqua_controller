# Component build configuration for usb_comm_aq
idf_component_register(SRCS "usb_mac.c" "usb_comm_aq.c" "descriptors_control.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_netif esp_tinyusb nvs_flash config_aq json_log
                    PRIV_REQUIRES espressif__tinyusb bios_config_aq)

set_property(TARGET ${COMPONENT_LIB} APPEND PROPERTY COMPONENT_OVERWRITE_SRCS "descriptors_control.c")

