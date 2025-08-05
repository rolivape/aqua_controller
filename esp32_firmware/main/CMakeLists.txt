idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES app_logic_aq json_log usb_comm_aq usb_descriptors_aq)