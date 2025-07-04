cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Name project
SET(tarefa-freertos-1-RoogerMelo rp2040-freertos-template)

# Set any variables required for importing libraries
SET(FREERTOS_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(${FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(tarefa-freertos-1-RoogerMelo C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(tarefa-freertos-1-RoogerMelo src/main.c )

pico_set_program_name(tarefa-freertos-1-RoogerMelo "tarefa-freertos-1-RoogerMelo")
pico_set_program_version(tarefa-freertos-1-RoogerMelo "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(tarefa-freertos-1-RoogerMelo 0)
pico_enable_stdio_usb(tarefa-freertos-1-RoogerMelo 0)

# Add the standard library to the build
target_link_libraries(tarefa-freertos-1-RoogerMelo
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(tarefa-freertos-1-RoogerMelo PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
   ${CMAKE_CURRENT_LIST_DIR}/include
   ${CMAKE_CURRENT_LIST_DIR}/src
)

# Add any user requested libraries
target_link_libraries(tarefa-freertos-1-RoogerMelo 
   pico_stdlib
   FreeRTOS-Kernel-Heap4
   hardware_pwm
   hardware_i2c        
        )

pico_add_extra_outputs(tarefa-freertos-1-RoogerMelo)