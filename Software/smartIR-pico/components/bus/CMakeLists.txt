idf_component_register(SRCS "i2c_app.c" "i2c_bus.c"
                        INCLUDE_DIRS "."
                        REQUIRES driver axp173 mpu6050)