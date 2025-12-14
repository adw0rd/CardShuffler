cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT on)

include(pico_sdk_import.cmake)

project(card_shuffler C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(card_shuffler
    src/main.c
    src/display/st7735.c
    src/display/graphics.c
    src/display/icons.c
    src/ui/menu.c
    src/input/encoder.c
    src/motor/motor.c
    src/motor/strategies.c
)

target_include_directories(card_shuffler PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
)

target_link_libraries(card_shuffler
    pico_stdlib
    hardware_spi
    hardware_pwm
    hardware_gpio
)

pico_enable_stdio_usb(card_shuffler 1)
pico_enable_stdio_uart(card_shuffler 0)

pico_add_extra_outputs(card_shuffler)
