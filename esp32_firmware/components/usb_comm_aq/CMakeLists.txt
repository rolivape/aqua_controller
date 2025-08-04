# Component build configuration for usb_comm_aq
idf_component_register(SRCS "usb_comm_aq.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_netif esp_tinyusb nvs_flash config_aq json_log
                    PRIV_REQUIRES espressif__tinyusb)