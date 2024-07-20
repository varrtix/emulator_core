# cmake/ctf.cmake
cmake_minimum_required(VERSION 3.16)

set(CTF_RPATH ${ctf_path} CACHE STRING "Specific CTF realpath")
if(NOT CTF_RPATH STREQUAL ctf_path)
    set(ctf_path ${CTF_RPATH})
    message(STATUS "Using specific ctf_path: ${CTF_RPATH}")
endif()

set(CTF_LIBDIR ${ctf_path}/bin/Debug)