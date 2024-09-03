cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(seatcontrol)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(seatcontrol
    src/main.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(seatcontrol pico_stdlib hardware_i2c)

pico_enable_stdio_usb(seatcontrol 1)
pico_enable_stdio_uart(seatcontrol 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(seatcontrol)
