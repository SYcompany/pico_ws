cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(encoder)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(encoder
    src/main.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(encoder pico_stdlib hardware_pwm)

pico_enable_stdio_usb(encoder 1)
pico_enable_stdio_uart(encoder 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(encoder)
