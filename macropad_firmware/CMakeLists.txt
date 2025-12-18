cmake_minimum_required(VERSION 3.13)

# Pull in SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(macropad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(macropad
    src/main.c
    src/matrix.c
    src/encoder.c
    src/oled.c
    src/usb_hid.c
    src/usb_descriptors.c
)

target_include_directories(macropad PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# Enable USB output, disable UART
pico_enable_stdio_usb(macropad 0)
pico_enable_stdio_uart(macropad 0)

# Link libraries
target_link_libraries(macropad
    pico_stdlib
    hardware_gpio
    hardware_irq
    tinyusb_device
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(macropad)
