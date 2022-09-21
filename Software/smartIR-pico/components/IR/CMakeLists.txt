set(IR_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE SOURCES 
        ${IR_ROOT_DIR}/*.c
        ${IR_ROOT_DIR}/decode/src/*.c
        )


idf_component_register(
    SRCS 
        ${SOURCES} 
    INCLUDE_DIRS
        ${IR_ROOT_DIR}
        ${IR_ROOT_DIR}/decode/include
    REQUIRES 
        driver nvs_flash)