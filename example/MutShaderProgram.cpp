/*******************************************************************************
 * Author: Mathieu-André Chiasson
 * Date: 2017-07-17
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or distribute
 * this software, either in source code form or as a compiled binary, for any
 * purpose, commercial or non-commercial, and by any means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors of this
 * software dedicate any and all copyright interest in the software to the
 * public domain. We make this dedication for the benefit of the public at large
 * and to the detriment of our heirs and successors. We intend this dedication
 * to be an overt act of relinquishment in perpetuity of all present and future
 * rights to this software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 ******************************************************************************/

#include <MutShaderProgram.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

const char * shaderProgramTypeToStr(GLenum type) {
    switch (type) {
        case GL_VERTEX_SHADER : return "vertex";
        case GL_FRAGMENT_SHADER: return "fragment";
        default: return "???";
    }
}


uint32_t MutShaderProgram::compile(const uint8_t *shaderSrc, size_t shaderSrcLen, GLenum type) {
    GLint compiled = 0, infoLen = 0;
    char* infoLog = nullptr;
    GLuint shader = brebisGLCheckErrorDebug(glCreateShader(type));
    if(shader) {
        brebisGLCheckErrorDebug(glShaderSource(shader, 1, (const GLchar **)&shaderSrc, (GLint*)&shaderSrcLen));
        brebisGLCheckErrorDebug(glCompileShader(shader));
        brebisGLCheckErrorDebug(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));
        if(!compiled) {
            brebisGLCheckErrorDebug(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen));
            if(infoLen > 1) {
                infoLog = (char*)malloc(sizeof(char) * infoLen);
                brebisGLCheckErrorDebug(glGetShaderInfoLog(shader, infoLen, NULL, infoLog));
                std::cerr << "Error compiling " << shaderProgramTypeToStr(type) << " shader: " << infoLog << std::endl;
                free(infoLog);
            }
            brebisGLCheckErrorDebug(glDeleteShader(shader));
            shader = 0;
        }
    }
    return shader;
}


GLuint MutShaderProgram::link(GLuint vertexShader, GLuint fragmentShader) {
    GLint linked = 0, infoLen = 0;
    GLuint shaderProgram = brebisGLCheckErrorDebug(glCreateProgram());
    char* infoLog = nullptr;
    if (shaderProgram) {
        brebisGLCheckErrorDebug(glAttachShader(shaderProgram, vertexShader));
        brebisGLCheckErrorDebug(glAttachShader(shaderProgram, fragmentShader));
        brebisGLCheckErrorDebug(glLinkProgram(shaderProgram));
        brebisGLCheckErrorDebug(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked));
        if(!linked) {
            brebisGLCheckErrorDebug(glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLen));
            if(infoLen > 1) {
                infoLog = (char*)malloc(sizeof(char) * infoLen);
                brebisGLCheckErrorDebug(glGetProgramInfoLog(shaderProgram, infoLen, NULL, infoLog));
                std::cerr << "Error linking program: " << infoLog << std::endl;
                free(infoLog);
            }
            brebisGLCheckErrorDebug(glDeleteProgram(shaderProgram));
            shaderProgram = 0;
        }
    }
    return shaderProgram;
}

GLuint MutShaderProgram::compileAndLink(const uint8_t *vertShaderSrc,
                                        size_t vertShaderSrcLen,
                                        const uint8_t *fragShaderSrc,
                                        size_t fragShaderSrcLen) {
    GLuint program = 0, vertex = 0, fragment = 0;
    vertex = compile(vertShaderSrc, vertShaderSrcLen, GL_VERTEX_SHADER);
    if (!vertex) return program;
    fragment = compile(fragShaderSrc, fragShaderSrcLen, GL_FRAGMENT_SHADER);
    if (vertex && fragment) {
        program = link(vertex, fragment);
    }
    if (vertex) {
        brebisGLCheckErrorDebug(glDeleteShader(vertex));
    }
    if (fragment) {
        brebisGLCheckErrorDebug(glDeleteShader(fragment));
    }
    return program;
}

GLuint MutShaderProgram::compileAndLink(const char *vertShaderFilePath, const char *fragShaderFilePath)
{
    std::stringstream vcontent;
    std::stringstream fcontent;

    std::ifstream vifs(vertShaderFilePath);
    if (!vifs.is_open()) {
        std::cerr << "Unable to open " << vertShaderFilePath << std::endl;
        return 0;
    }

    std::ifstream fifs(fragShaderFilePath);
    if (!fifs.is_open()) {
        std::cerr << "Unable to open " << fragShaderFilePath << std::endl;
        return 0;
    }

    std::string line;

    while ( getline(vifs,line) ) {
        vcontent << line << '\n';
    }

    while ( getline(fifs,line) ) {
        fcontent << line << '\n';
    }

    std::string vertShader = vcontent.str();
    std::string fragShader = fcontent.str();

    return compileAndLink((uint8_t*)vertShader.c_str(), vertShader.size(), (uint8_t*)fragShader.c_str(), fragShader.length());
}
