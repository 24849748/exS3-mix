set(NET_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE SOURCES 
        ${NET_ROOT_DIR}/wifi/*.c
        ${NET_ROOT_DIR}/time/*.c
        )


idf_component_register(
    SRCS 
        ${SOURCES} 
    INCLUDE_DIRS
        ${NET_ROOT_DIR}/wifi
        ${NET_ROOT_DIR}/time
    REQUIRES 
        nvs_flash)