cmake_minimum_required(VERSION 3.7)

include(cmake/HunterGate.cmake)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.34.tar.gz"
    SHA1 "9d0944566336eb6e4e26b4a709e1f17923a9aaa8"
    LOCAL # <----- Use local config.cmake
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

if (NOT EXISTS ${GLBrebis_BINARY_DIR}/gl.xml)
    file(DOWNLOAD https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/master/xml/gl.xml ${GLBrebis_BINARY_DIR}/gl.xml SHOW_PROGRESS)
endif()
configure_file(${GLBrebis_SOURCE_DIR}/template/GL.h ${GLBrebis_BINARY_DIR}/template/GL.h COPYONLY)
configure_file(${GLBrebis_SOURCE_DIR}/template/GL.c ${GLBrebis_BINARY_DIR}/template/GL.c COPYONLY)

add_executable(GLBrebis
    src/GLBrebisApp.cpp
    src/GLBrebisCodeGenerator.cpp
    src/GLBrebisData.cpp
    src/GLBrebisParser.cpp
    src/GLBrebisUtilities.cpp
)

target_link_libraries(GLBrebis
    RapidXML::RapidXML
    Poco::NetSSL
    Poco::Net
    Poco::Foundation
    OpenSSL::SSL
    OpenSSL::Crypto
)
