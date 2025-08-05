# Component for USB descriptors
idf_component_register(SRCS "usb_descriptors_aq.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "esp_tinyusb")
