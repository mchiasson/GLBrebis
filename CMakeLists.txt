################################################################################
# MIT License
#
# Copyright (c) 2017-2018 Mathieu-André Chiasson
# All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# 1. The above copyright notice and this permission notice shall be included in
#    all copies or substantial portions of the Software.
#
# Disclaimer:
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
################################################################################

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

include(cmake/HunterGate.cmake)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.11.tar.gz"
    SHA1 "b732407d782459f89950ab52bc0ff9612f3e4be2"
    LOCAL # <----- Use cmake/Hunter/config.cmake
)

project(GLBrebis)

################################################################################
# OpenSSL / Crypto Libraries
################################################################################
hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)

################################################################################
# Poco C++ Libraries
################################################################################
hunter_add_package(PocoCpp)
find_package(Poco REQUIRED Foundation JSON Net NetSSL Crypto Util Zip CONFIG)

################################################################################
# RapidXML
################################################################################
hunter_add_package(RapidXML)
find_package(RapidXML REQUIRED CONFIG)

################################################################################
# GLBrebis Console App
################################################################################

# Generate a 'download.cmake' helper that will be used at build-time.
file(WRITE ${PROJECT_BINARY_DIR}/download.cmake "# Generated built-time download helper.\n")
file(APPEND ${PROJECT_BINARY_DIR}/download.cmake "message(STATUS \"Downloading xml/gl.xml ...\")\n")
file(APPEND ${PROJECT_BINARY_DIR}/download.cmake "file(DOWNLOAD https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/master/xml/gl.xml xml/gl.xml)\n")
file(APPEND ${PROJECT_BINARY_DIR}/download.cmake "message(STATUS \"Downloading KHR/khrplatform.h ...\")\n")
file(APPEND ${PROJECT_BINARY_DIR}/download.cmake "file(DOWNLOAD https://raw.githubusercontent.com/KhronosGroup/EGL-Registry/master/api/KHR/khrplatform.h ${PROJECT_BINARY_DIR}/KHR/khrplatform.h)\n")

add_custom_command(
    OUTPUT
        ${PROJECT_BINARY_DIR}/KHR/khrplatform.h
        ${PROJECT_BINARY_DIR}/xml/gl.xml
    COMMAND
        ${CMAKE_COMMAND} -P ${PROJECT_BINARY_DIR}/download.cmake
    WORKING_DIRECTORY
        ${PROJECT_BINARY_DIR}
    COMMENT
        "Downloading Dependencies"
)
set_source_files_properties(${PROJECT_BINARY_DIR}/KHR/khrplatform.h PROPERTIES GENERATED TRUE)
set_source_files_properties(${PROJECT_BINARY_DIR}/xml/gl.xmx        PROPERTIES GENERATED TRUE)

configure_file(${PROJECT_SOURCE_DIR}/template/GL.h.template ${PROJECT_BINARY_DIR}/template/GL.h.template COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/xml/extra.xml          ${PROJECT_BINARY_DIR}/xml/extra.xml          COPYONLY)

function(EmbedResource RESOURCE)
    get_filename_component(RESOURCE_WORKSPACE ${RESOURCE} DIRECTORY)
    add_custom_command(
        OUTPUT
            ${RESOURCE}.c
            ${RESOURCE}.h
        COMMAND
            ${CMAKE_COMMAND} -Dresource=${RESOURCE} -P ${PROJECT_SOURCE_DIR}/cmake/EmbedResource.cmake
        WORKING_DIRECTORY
            ${RESOURCE_WORKSPACE}
        MAIN_DEPENDENCY
            ${RESOURCE}
        DEPENDS
            ${RESOURCE}
    )
    set_source_files_properties(${RESOURCE}.c PROPERTIES GENERATED TRUE)
    set_source_files_properties(${RESOURCE}.h PROPERTIES GENERATED TRUE)
endfunction()

EmbedResource(${PROJECT_BINARY_DIR}/KHR/khrplatform.h)
EmbedResource(${PROJECT_BINARY_DIR}/template/GL.h.template)
EmbedResource(${PROJECT_BINARY_DIR}/xml/extra.xml)
EmbedResource(${PROJECT_BINARY_DIR}/xml/gl.xml)

add_library(GLBrebisLib STATIC
    src/GLBrebisCodeGenerator.cpp
    src/GLBrebisData.cpp
    src/GLBrebisParser.cpp
    src/GLBrebisUtilities.cpp

    #generated by EmbedResource.cmake
    ${PROJECT_BINARY_DIR}/KHR/khrplatform.h.c
    ${PROJECT_BINARY_DIR}/KHR/khrplatform.h.h
    ${PROJECT_BINARY_DIR}/xml/gl.xml.c
    ${PROJECT_BINARY_DIR}/xml/gl.xml.h
    ${PROJECT_BINARY_DIR}/xml/extra.xml.c
    ${PROJECT_BINARY_DIR}/xml/extra.xml.h
    ${PROJECT_BINARY_DIR}/template/GL.h.template.c
    ${PROJECT_BINARY_DIR}/template/GL.h.template.h
)

target_include_directories(GLBrebisLib PUBLIC
    # to access generated headers created by EmbedResource.cmake
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(GLBrebisLib
    PUBLIC
    RapidXML::RapidXML
    Poco::NetSSL
    Poco::Net
    Poco::Zip
    Poco::Foundation
    OpenSSL::SSL
    OpenSSL::Crypto
)

add_executable(GLBrebis src/GLBrebisApp.cpp src/GLBrebisApp.h)
target_link_libraries(GLBrebis GLBrebisLib)

add_custom_command(
    TARGET
        GLBrebis POST_BUILD
    COMMAND
        ${PROJECT_BINARY_DIR}/GLBrebis
    WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}/example
)

################################################################################
# GLBrebis Example App
################################################################################
add_subdirectory(example)

# To insure that GLBrebis is built first because GLBrebis generates the
# example/BrebisGL.h using the latest downloaded khronos API schematics, which
# will always upgrade example/BrebisGL.h to the latest. Any example/BrebisGL.h
# deltas should be commited to git for convenience.
add_dependencies(MutCube GLBrebis)
