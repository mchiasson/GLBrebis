################################################################################
# MIT License
#
# Copyright (c) 2017 Mathieu-André Chiasson
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

if (NOT DEFINED resource)
    message(FATAL_ERROR "Missing -Dresource=<RESOURCE_FILE>")
endif()

##
# Create our variables
##
get_filename_component(resource_name ${resource} NAME)
string(MAKE_C_IDENTIFIER ${resource_name} resource_id)
string(TOUPPER ${resource_id} resource_id_upper)

##
# Open and load the resource file to embed in hexadecimal
##
file(READ ${resource} resource_data HEX)
string(LENGTH ${resource_data} resource_data_size)

if (resource_data_size EQUAL 0)
    message(FATAL_ERROR "Could not load '${resource}'. Does the file exist?")
endif()

##
# 1 byte in hex is represented with 2 characters, where in reality, it's 1
# charcacter (1 byte) based on how the C compiler will interpret this data.
##
math(EXPR resource_data_size "${resource_data_size} / 2")

##
# Convert the hexadecimal values to C hexadecimal string representation. For
# example:
#
#     '...3a4c8f...' will become '...0x3a,0x4c,0x8f,...'
##
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," resource_data ${resource_data})

##
# And then split it to have 16 hex values per line for helping most IDEs or text
# editor when opening a file with such a long string of data.
#
# Unfortunately, cmake regular expression is a bit limited: We cannot do {16},
# which is why I am doing it the hard way.
##
string(REGEX REPLACE
    "(0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],0x[0-9a-f][0-9a-f],)"
    "\\1\n    " resource_data ${resource_data})

##
# Create the embedded source file
##
file(WRITE ${resource_name}.c "")
file(APPEND ${resource_name}.c "#include \"${resource_name}.h\"\n")
file(APPEND ${resource_name}.c "\n")
file(APPEND ${resource_name}.c "const unsigned char ${resource_id}[${resource_data_size}] = {\n")
file(APPEND ${resource_name}.c "    ${resource_data}\n")
file(APPEND ${resource_name}.c "};\n")

##
# Create the embedded header file
##
file(WRITE ${resource_name}.h "")
file(APPEND ${resource_name}.h "#ifndef _${resource_id_upper}_H_\n")
file(APPEND ${resource_name}.h "#define _${resource_id_upper}_H_\n")
file(APPEND ${resource_name}.h "\n")
file(APPEND ${resource_name}.h "#ifdef __cplusplus\n")
file(APPEND ${resource_name}.h "extern \"C\" {\n")
file(APPEND ${resource_name}.h "#endif\n")
file(APPEND ${resource_name}.h "\n")
file(APPEND ${resource_name}.h "extern const unsigned char ${resource_id}[${resource_data_size}];\n")
file(APPEND ${resource_name}.h "\n")
file(APPEND ${resource_name}.h "#ifdef __cplusplus\n")
file(APPEND ${resource_name}.h "}\n")
file(APPEND ${resource_name}.h "#endif\n")
file(APPEND ${resource_name}.h "\n")
file(APPEND ${resource_name}.h "#endif /* _${resource_id_upper}_H_ */\n")
