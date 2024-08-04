cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(vl53lox)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(vl53lox
    src/main.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(vl53lox pico_stdlib hardware_i2c)

pico_enable_stdio_usb(vl53lox 1)
pico_enable_stdio_uart(vl53lox 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(vl53lox)
