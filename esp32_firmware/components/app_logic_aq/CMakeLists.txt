idf_component_register(SRCS "app_logic_aq.c"
                    INCLUDE_DIRS "include"
                    REQUIRES bios_config_aq json_log usb_comm_aq)
