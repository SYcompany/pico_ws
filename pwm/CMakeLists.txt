cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(PROJECT_NAME pwm)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJECT_NAME})



pico_sdk_init()


add_executable(${PROJECT_NAME}
    src/main.c
)


pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/pwm.pio)

target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_pio)

pico_add_extra_outputs(${PROJECT_NAME})