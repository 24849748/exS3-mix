idf_component_register(SRCS "mNVS.c" "sd_card.c"
                        INCLUDE_DIRS "."
                        REQUIRES driver fatfs nvs_flash)