# GLBrebis
A simple GL extension wrangler drop-in generator script for C/C++ OpenGL and OpenGL ES apps.

It will generate a set of highly portable C99 compliant files with any `<prefix>` that you want, and the script will customize the generated code with any given prefix name.

This project replaces the need of using a 3rdparty library to wrangle your GL function pointers (e.g. GLEW). Works pretty much the same way than GLEW, with GLES support, in addition to some useful utilities, such as `<prefix>GLSupport`:
```C++
if(<prefix>GLSupport(GL_VERSION_3_0) || <prefix>GLSupport(GL_ES_VERSION_3_0))
{
    void *pMapped = glMapBufferRange(target, 0, buffer.size(), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    std::copy(buffer.begin(), buffer.end(), pMapped);
    glUnmapBuffer(target);
}
else if (<prefix>GLSupport(GL_EXT_map_buffer_range) && <prefix>GLSupport(GL_OES_mapbuffer)
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
And also comes with this very handy debugging tool that you can wrap around eveyr single GL function calls like so:
```C++
<prefix>GLCheckErrorDebug(glBindBuffer(handle));
```
`<prefix>GLCheckErrorDebug` checks the result of `glGetError` for you, parses the result and prints the error in `stderr` should an error be found. `glGetError` check is automatically removed when compiling in release.

# Requirement
First of all, you're going to need to install 'groovy' since GLBrebis is written in Groovy.

## Mac OSX
If you’re on MacOS and have MacPorts installed, you can run:
```
sudo port install groovy
```

If you’re on MacOS and have Homebrew installed, you can run:
```
brew install groovy
```

## Linux
If you're on Debian, or Debian-family distro (Ubuntu, Mint, etc), you can run:
```
sudo apt-get install groovy
```
If you're on RedHat Enterprise, or CentOS, you can run:
```
sudo yum install groovy
```

## Microsoft Windows
If you’re on Windows, you can also use the NSIS Windows installer:
https://dl.bintray.com/groovy/Distributions/groovy-2.5.0-beta-1-installer.exe

# How to generate my project's drop-in extention wrangler header and source?
```
GLBrebis -p <prefix> [-s /path/to/src] [-H /path/to/include] [-i includePrefix] [-f]

Available options (use -h for help):
 -f,--force                 Force a fresh re-download of Khronos GL
                            headers.
 -h,--help                  Usage Information
 -H,--headerPath <arg>      path to dump the generated headers into.
                            Default : 'output/include'
 -i,--includePrefix <arg>   prefix path for the generated source to
                            properly include the headers. Default : ''
 -p,--prefix <arg>          Project prefix to append to every generated
                            files and symbols
 -s,--srcPath <arg>         path to dump the generated source into.
                            Default : 'output/src'

```
Example:
```
./GLBrebis.sh --prefix Onut --srcPath ~/git/onut/src --headerPath ~/git/onut/include/onut --includePrefix onut
```
* `-p,--prefix` is mandatory. If you do `--prefix Onut`, the script will generate `OnutGL.c`, `OnutGL.h` and `OnutGLExt.h`, and symbols will automatically be named after `Onut`.  For example, you will see `_ONUT_GL_H_` header guards, and utility functions will be named `onutGLInit()` and `onutGLShutdown()`, etc.
*  `-s,--srcPath` is optional. This is where `OnutGL.c` will be dumped into. If not specified, it will be dumped in `output/src`
*  `-H,--headerPath` is optional. This is where `OnutGL.h` and `OnutGLExt.h` will be dumped into. If not specified, it will be dumped in `output/include`
* `-i,--includePrefix` is optional. It's mostly used by `OnutGL.c` to generate a proper `#include <onut/OnutGL.h>` line to help you match your application's source layout requirement.
* `-f,--force` is optional. It simply forces a full re-download of Khronos GL header, ignoring the ones that were previously downloaded from the last run.

And if everything goes well, you should have `output/src/<prefix>GL.c`, `output/include/<prefix>GL.h` and `output/include/<prefix>GLext.h` on your file system

# How to use the generated extention wrangler code in my project?

1. Drop-in the three generated files into your project
2. Add `<prefix>GL.c` to your CMakeLists.txt/Makefile/VCProj/etc
3. make sure you include `#include "<prefix>GL.h"` instead of including the regular.
4. create your window *and* your GL context.
5. Once the gl context successfully created, simply call `bool success = <prefix>GLInit();`.
6. When shutting down, don't forget to call `<prefix>GLShutdown();`

