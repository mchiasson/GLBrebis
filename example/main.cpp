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
#include <SDL2/SDL.h>
#include <MutEngine.h>
#include <glm/gtc/matrix_transform.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


static SDL_Window *window = nullptr;
static MutEngine *engine = nullptr;

/* Global */
static int last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
static int arcball_on = 0;
static int screen_width = 720;
static int screen_height = 720;
static float fovy = glm::radians(45.0f);
static float aspect = static_cast<float>(screen_width) / static_cast<float>(screen_height);
static float zNear = 3.0;
static float zFar = 7.0;

static bool running;

void renderFrame();

int main(int, char *[])
{
    SDL_LogSetPriority(0, SDL_LOG_PRIORITY_VERBOSE);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogCritical(0, "Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow("GLBrebis Example",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen_width,
                              screen_height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_LogCritical(0, "Window could not be created: %s\n", SDL_GetError() );
        SDL_Quit();
        return 2;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if(!context) {
        SDL_LogCritical(0, "GL Context could not be created: %s\n", SDL_GetError() );
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 3;
    }

    // Now that our GL context is created, we need to initialize our generated
    // GL wranger. Make sure it returns true!
    if (brebisGLInit() != KHRONOS_TRUE)
    {
        SDL_LogCritical(0, "Unable to initialize GLBrebis Wranger.\n");
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 4;
    }

    engine = new MutEngine();

    engine->projection = glm::perspective(fovy, aspect, zNear, zFar);
    engine->model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));


    running = true;

#ifdef __EMSCRIPTEN__
    int fps = 0;
    int simulate_infinite_loop = 1;
    emscripten_set_main_loop(renderFrame, fps, simulate_infinite_loop);
#else
    while(running)
    {
        renderFrame();
    }
#endif

    delete engine;
    engine = nullptr;

    // Don't forget to call this when you're done.
    brebisGLShutdown();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void onMouseDown(int x, int y)
{
    if (arcball_on == 0)
    {
        last_mx = cur_mx = x;
        last_my = cur_my = y;
    }
    ++arcball_on;
}

void onMouseMove(int x, int y)
{
    if (arcball_on) {
        cur_mx = x;
        cur_my = y;
    }
}

void onMouseRelease(int, int)
{
    --arcball_on;
}

glm::vec3 get_arcball_vector(int x, int y) {
    glm::vec3 P = glm::vec3(2.0*x/screen_width - 1.0,
                            2.0*y/screen_height - 1.0,
                            0);
    float OP_squared = P.x * P.x + P.y * P.y;
    P.y = -P.y;
    if (OP_squared <= 1) {
        P.z = sqrt(1 - OP_squared);  // Pythagore
    } else {
        P = glm::normalize(P);  // nearest point
    }
    return P;
}


void renderFrame()
{
    SDL_Event e;
    while( SDL_PollEvent( &e ) != 0 )
    {
        switch(e.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            onMouseDown(e.button.x, e.button.y);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseRelease(e.button.x, e.button.y);
            break;
        case SDL_MOUSEMOTION:
            onMouseMove(e.button.x, e.button.y);
            break;
        case SDL_FINGERDOWN:
            onMouseDown(static_cast<int>(e.tfinger.x*screen_width),
                        static_cast<int>(e.tfinger.y*screen_height));
            break;
        case SDL_FINGERUP:
            onMouseRelease(static_cast<int>(e.tfinger.x*screen_width),
                           static_cast<int>(e.tfinger.y*screen_height));
            break;
        case SDL_FINGERMOTION:
            onMouseMove(static_cast<int>(e.tfinger.x*screen_width),
                        static_cast<int>(e.tfinger.y*screen_height));
            break;
        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                screen_width = e.window.data1;
                screen_height = e.window.data2;
                break;
            case SDL_WINDOWEVENT_CLOSE: running = false; break;
            default: break;
            }
            break;
        case SDL_QUIT: running = false; break;
        default: break;
        }
    }

    if (running)
    {
        if (cur_mx != last_mx || cur_my != last_my) {
            glm::vec3 va = get_arcball_vector(last_mx, last_my);
            glm::vec3 vb = get_arcball_vector( cur_mx,  cur_my);
            float angle = glm::acos(glm::min(1.0f, glm::dot(va, vb)));
            glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
            glm::mat3 camera2object = glm::inverse(glm::mat3(engine->projection) * glm::mat3(engine->model));
            glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
            engine->model = glm::rotate(engine->model, angle, axis_in_object_coord);
            last_mx = cur_mx;
            last_my = cur_my;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        engine->drawCube();
        SDL_GL_SwapWindow(window);
    }
}
