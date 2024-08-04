cmake_minimum_required(VERSION 3.13)



set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(PROJECT_NAME dual_core)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJECT_NAME})

#set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})


pico_sdk_init()

add_executable(${PROJECT_NAME}
    src/main.c
)

target_link_libraries(${PROJECT_NAME} pico_stdlib pico_multicore)

pico_add_extra_outputs(${PROJECT_NAME})