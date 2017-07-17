/*******************************************************************************
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
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
    char* infoLog = NULL;
    GLuint shader = mutGLCheckErrorDebug(glCreateShader(type));
    if(shader) {
        mutGLCheckErrorDebug(glShaderSource(shader, 1, (const GLchar **)&shaderSrc, (GLint*)&shaderSrcLen));
        mutGLCheckErrorDebug(glCompileShader(shader));
        mutGLCheckErrorDebug(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));
        if(!compiled) {
            mutGLCheckErrorDebug(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen));
            if(infoLen > 1) {
                infoLog = (char*)malloc(sizeof(char) * infoLen);
                mutGLCheckErrorDebug(glGetShaderInfoLog(shader, infoLen, NULL, infoLog));
                std::cerr << "Error compiling " << shaderProgramTypeToStr(type) << " shader: " << infoLog << std::endl;
                free(infoLog);
            }
            mutGLCheckErrorDebug(glDeleteShader(shader));
            shader = 0;
        }
    }
    return shader;
}


GLuint MutShaderProgram::link(GLuint vertexShader, GLuint fragmentShader) {
    GLint linked = 0, infoLen = 0;
    GLuint shaderProgram = mutGLCheckErrorDebug(glCreateProgram());
    char* infoLog = NULL;
    if (shaderProgram) {
        mutGLCheckErrorDebug(glAttachShader(shaderProgram, vertexShader));
        mutGLCheckErrorDebug(glAttachShader(shaderProgram, fragmentShader));
        mutGLCheckErrorDebug(glLinkProgram(shaderProgram));
        mutGLCheckErrorDebug(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked));
        if(!linked) {
            mutGLCheckErrorDebug(glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLen));
            if(infoLen > 1) {
                infoLog = (char*)malloc(sizeof(char) * infoLen);
                mutGLCheckErrorDebug(glGetProgramInfoLog(shaderProgram, infoLen, NULL, infoLog));
                std::cerr << "Error linking program: " << infoLog << std::endl;
                free(infoLog);
            }
            mutGLCheckErrorDebug(glDeleteProgram(shaderProgram));
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
        mutGLCheckErrorDebug(glDeleteShader(vertex));
    }
    if (fragment) {
        mutGLCheckErrorDebug(glDeleteShader(fragment));
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
