set(WIDGET_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE SOURCES 
        ${WIDGET_ROOT_DIR}/*.c
        ${WIDGET_ROOT_DIR}/led/*.c
        ${WIDGET_ROOT_DIR}/motor/*.c
        ${WIDGET_ROOT_DIR}/encoder/*.c
        )


idf_component_register(
    SRCS 
        ${SOURCES} 
    INCLUDE_DIRS
        ${WIDGET_ROOT_DIR}
        ${WIDGET_ROOT_DIR}/led
        ${WIDGET_ROOT_DIR}/motor
        ${WIDGET_ROOT_DIR}/encoder
    REQUIRES 
        driver lvgl)