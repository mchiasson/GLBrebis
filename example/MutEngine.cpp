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
#include <MutEngine.h>
#include <MutShaderProgram.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define MUT_ARRAY_BUFFER 0
#define MUT_ELEMENT_ARRAY_BUFFER 1

struct VertexData
{
    glm::vec3 position;
    glm::vec2 texCoord;
};


MutEngine::MutEngine()
{
    VertexData vertices[] = {
        {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.5f)},
        {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.33333f, 0.5f)},
        {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.33333f, 0.0f)},

        {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2( 0.0f, 1.0f)},
        {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.33333f, 1.0f)},
        {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.5f)},
        {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.33333f, 0.5f)},

        {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.66666f, 1.0f)},
        {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.66666f, 0.5f)},
        {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(1.0f, 0.5f)},

        {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.66666f, 0.5f)},
        {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(1.0f, 0.5f)},
        {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.66666f, 0.0f)},
        {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(1.0f, 0.0f)},

        {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.33333f, 0.5f)},
        {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.66666f, 0.5f)},
        {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.33333f, 0.0f)},
        {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.66666f, 0.0f)},

        {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.33333f, 1.0f)},
        {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.66666f, 1.0f)},
        {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.33333f, 0.5f)},
        {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.66666f, 0.5f)}
    };

    uint8_t indices[] = {
         0,  1,  2,  3,  3,
         4,  4,  5,  6,  7,  7,
         8,  8,  9, 10, 11, 11,
        12, 12, 13, 14, 15, 15,
        16, 16, 17, 18, 19, 19,
        20, 20, 21, 22, 23
    };

    program = MutShaderProgram::compileAndLink("assets/shader.vert", "assets/shader.frag");

    mutGLCheckErrorDebug ( glUseProgram(program) );

    positionLoc = mutGLCheckErrorDebug(glGetAttribLocation(program, "a_position"));
    texCoordLoc = mutGLCheckErrorDebug(glGetAttribLocation(program, "a_texcoord"));
    mvpLoc = mutGLCheckErrorDebug(glGetUniformLocation(program, "u_mvp"));
    texture0Loc = mutGLCheckErrorDebug(glGetUniformLocation(program, "u_texture0"));
    cubeIndexCount = sizeof(indices);
    cubeBufferId[MUT_ARRAY_BUFFER] = 0;
    cubeBufferId[MUT_ELEMENT_ARRAY_BUFFER] = 0;
    mutGLCheckErrorDebug ( glGenBuffers(2, cubeBufferId) );

    mutGLCheckErrorDebug ( glBindBuffer(GL_ARRAY_BUFFER, cubeBufferId[MUT_ARRAY_BUFFER]) );
    mutGLCheckErrorDebug ( glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW) );

    mutGLCheckErrorDebug ( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeBufferId[MUT_ELEMENT_ARRAY_BUFFER]) );
    mutGLCheckErrorDebug ( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW) );

    int w;
    int h;
    int comp;
    unsigned char* pixels = stbi_load("assets/texture.png", &w, &h, &comp, STBI_rgb);
    mutGLCheckErrorDebug ( glGenTextures(1, &textureId) );
    mutGLCheckErrorDebug ( glActiveTexture(GL_TEXTURE0) );
    mutGLCheckErrorDebug ( glBindTexture(GL_TEXTURE_2D, textureId) );
    mutGLCheckErrorDebug ( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
    mutGLCheckErrorDebug ( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
    mutGLCheckErrorDebug ( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
    mutGLCheckErrorDebug ( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
    mutGLCheckErrorDebug ( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels) );
    stbi_image_free(pixels);

    mutGLCheckErrorDebug ( glEnable(GL_DEPTH_TEST) );
    mutGLCheckErrorDebug ( glEnable(GL_CULL_FACE) );

    mutGLCheckErrorDebug ( glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)0) );
    mutGLCheckErrorDebug ( glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)sizeof(glm::vec3)) );
    mutGLCheckErrorDebug ( glEnableVertexAttribArray(positionLoc) );
    mutGLCheckErrorDebug ( glEnableVertexAttribArray(texCoordLoc) );
    mutGLCheckErrorDebug ( glUniform1i(texture0Loc, 0) );
}

MutEngine::~MutEngine()
{
    mutGLCheckErrorDebug(glDeleteProgram(program));
    program = 0;
    positionLoc = 0;
    texCoordLoc = 0;

    glDeleteBuffers(2, cubeBufferId);
    cubeBufferId[MUT_ARRAY_BUFFER] = 0;
    cubeBufferId[MUT_ELEMENT_ARRAY_BUFFER] = 0;
}

void MutEngine::drawCube()
{
    glm::mat4 mvp = projection * model;
    mutGLCheckErrorDebug ( glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp)) );
    glDrawElements(GL_TRIANGLE_STRIP, cubeIndexCount, GL_UNSIGNED_BYTE, 0);
}
