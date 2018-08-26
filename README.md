# GLBrebis
A simple GL extension wrangler drop-in generator for OpenGL and OpenGL ES apps.

this program will generate a set of highly portable C99 compliant OpenGL and OpenGL ES function wrangler code with any *prefix* customization that you want. Just generate your wrangler code, drop the two files into your project and start coding! It's as simple as that.

This project replaces the need of using a 3rdparty library to wrangle your GL function pointers (e.g. GLEW). Works pretty much the same way than GLEW, but also adds GLES support, and to some useful utilities, such as `brebisGLSupport`:

```C++
if(brebisGLSupport(GL_VERSION_3_0) || brebisGLSupport(GL_ES_VERSION_3_0))
{
    void *pMapped = glMapBufferRange(target, 0, buffer.size(), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    std::copy(buffer.begin(), buffer.end(), pMapped);
    glUnmapBuffer(target);
}
else if (brebisGLSupport(GL_EXT_map_buffer_range) && <brebisGLSupport(GL_OES_mapbuffer)
{
    void *pMapped = glMapBufferRangeEXT(target, 0, buffer.size(), GL_MAP_WRITE_BIT_EXT | GL_MAP_INVALIDATE_BUFFER_BIT_EXT);
    std::copy(buffer.begin(), buffer.end(), pMapped);
    glUnmapBufferOES(target);
}
else
{
    glBufferData(target, buffer.size(), (const GLvoid *)(&buffer[0]), usage);
}
```

And also comes with this very handy debugging tool that you can wrap around every single GL function calls like so:

```C++
brebisGLCheckErrorDebug(glBindBuffer(handle));
```

`brebisGLCheckErrorDebug` checks the result of `glGetError` for you, parses the result and prints the error in `stderr` should an error be found. `glGetError` check is automatically removed when compiling in release.

# How to build GLBrebis

First of all, you need to install `perl` version 5.24 or better on your system.

GLBrebis uses cmake build system and will automatically download and build software dependencies for you. For windows users, the project can be generated like so:

```
cd desired/project/path
cmake C:/git/GLBrebis -G "Visual Studio 15 2017 Win64"
```

# How to generate my project's drop-in extention wrangler header and source?

```
usage: GLBrebis [-p <prefix>]
-h, --help                                Usage Information
-p<prefix>, --prefix=<prefix>             Project prefix to append to every
                                          generated files and symbols
```

the `-p` parameter is optional. If you don't specify it, it will default to use `Brebis` as the default prefix..

Example for a fictional project called 'Mut' :
```
./GLBrebis -p Mut
```
* The script will generate `mut_gl.h`, including symbols that will automatically be named after `Mut`.  For example, you will see `_MUT_GL_H_` header guards, and utility functions will be named `mutGLInit()`, `mutGLCheckErrorDebug()`, `mutGLShutdown()`, etc.

# How to use the generated extention wrangler code in my project?

1. Drop-in the generated `BrebisGL.h` file into your project
2. Make sure you include `#include "BrebisGL.h"` instead of including the system's GL or GLES header(s).
3. In one of the translation unit of your choice inside your project, either a `.c` or a `.cpp` file, you need to implement the generated `BrebisGL.h` like so:
```C++
#define BREBIS_GL_IMPLEMENTATION
#include <BrebisGL.h>
```
4. Create your window *and* your GL context.
5. Once the gl context successfully created, simply call `bool success = brebisGLInit();`.
6. When shutting down, don't forget to call `brebisGLShutdown();`
