# cmake/emu.cmake
cmake_minimum_required(VERSION 3.16)

# Define the macro for enabling or disabling emulator
option(ENABLE_EMU "Enable emulator mode" OFF)

# Add a definition based on the option
if(ENABLE_EMU)
    add_definitions(-DDRV_EMU)
    message(STATUS "Enabling definition flag: DRV_EMU")
endif()

set(DRV_DEPS_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/../include)
message(STATUS "Using drivers deps. include: ${DRV_DEPS_INCLUDE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

