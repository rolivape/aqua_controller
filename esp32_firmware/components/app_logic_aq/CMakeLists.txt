idf_component_register(SRCS "app_logic_aq.c"
                    INCLUDE_DIRS "include"
                    REQUIRES config_aq json_log usb_comm_aq)
