cmake_minimum_required(VERSION 3.16)

project(lebo_drivers LANGUAGES CXX)

include(GNUInstallDirs)
# Include emulator configs
include(${CMAKE_CURRENT_LIST_DIR}/cmake/emu.cmake)
include(${DRV_EMU_DIR}/cmake/ctf.cmake)

set_stdcxx()

set(DEFAULT_EMU_DRVS
    dtm
    EtherCat
    koganei
    lebo_gpio
    plc
    SFC8000
    TEC
)
if(DEFINED EMU_DRVS)
    string(REPLACE "," ";" EMU_DRV_LIST "${EMU_DRVS}")
else()
    set(EMU_DRV_LIST ${DEFAULT_EMU_DRVS})
endif()

set(CTF_LIBRARIES 
    iosvc
    ctf_util
    ctf_log
)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

# Set the policy CMP0135 to NEW
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Enable AddressSanitizer for Debug builds
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Enabling AddressSanitizer for Debug build")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    endif()

    add_compile_options(-Wall)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_definitions(__GNU_C__)
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_SYSTEM_NAME STREQUAL "MSYS")
    set(CMAKE_GENERATOR_PLATFORM x64)
    add_compile_definitions(WIN32 _CRT_SECURE_NO_WARNINGS)

    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        add_compile_options(/W4)
        set(tmp_list ${CTF_LIBRARIES})
        set(CTF_LIBRARIES)
        foreach(ctf_libname IN LISTS tmp_list)
            find_library(${ctf_libname}_LIB NAMES ${ctf_libname} PATHS ${CTF_LIBDIR})
            list(APPEND CTF_LIBRARIES ${${ctf_libname}_LIB})
            message(STATUS "Appending Library: ${${ctf_libname}_LIB}")
        endforeach()
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compile_options(-Wall)
    endif()
endif()

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
file(GLOB SRC_FILES ${SRC_DIR}/*.cc)
foreach(drvname IN LISTS EMU_DRV_LIST)
    set(CONFIG_H_CONTENT "// Auto-generated config.h for ${drvname}\n")
    set(CONFIG_H_CONTENT "${CONFIG_H_CONTENT}#define EMU_DRV_NAME \"${drvname}\"\n")
    file(WRITE "${PROJECT_BINARY_DIR}/include/${drvname}_config.h" "${CONFIG_H_CONTENT}")

    add_library(${drvname} SHARED ${SRC_FILES})
    target_include_directories(${drvname} PRIVATE 
        ${SRC_DIR}/internal
        ${PROJECT_BINARY_DIR}/include
        ${ctf_path}/include
        ${DRV_DEPS_INCLUDE}
    )
    target_link_directories(${drvname} PRIVATE ${CTF_LIBDIR})
    target_link_libraries(${drvname} PRIVATE ${CTF_LIBRARIES})

    target_compile_definitions(${drvname} PRIVATE CONFIG_H=\"${PROJECT_BINARY_DIR}/include/${drvname}_config.h\")

    install(TARGETS ${drvname}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endforeach()
