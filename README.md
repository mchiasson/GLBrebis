# GLBrebis
A simple GL extension wrangler drop-in generator script for C/C++ OpenGL and OpenGL ES apps.

this script will generate a set of highly portable C99 compliant GL function wrangler code with any `<prefix>` customization that you want. This script will customize the generated code with any given `<prefix>`. Just generate your wrangler code, drop the two files into your project and start coding! It's as simple as that.

This project replaces the need of using a 3rdparty library to wrangle your GL function pointers (e.g. GLEW). Works pretty much the same way than GLEW, but also adds GLES support, and to some useful utilities, such as `<prefix>GLSupport`:
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
If you’re on Windows, you can use the NSIS Windows installer:
https://dl.bintray.com/groovy/Distributions/groovy-2.5.0-beta-1-installer.exe

# How to generate my project's drop-in extention wrangler header and source?
```
GLBrebis -p <prefix> [-s /path/to/src] [-H /path/to/include] [-i includePrefix] [-f]

Available options (use -h for help):
 -f,--force                 Force a fresh re-download of Khronos GL
                            headers.
 -h,--help                  Usage Information
 -H,--headerPath <arg>      path to dump the generated headers into.
                            Default : 'output'
 -i,--includePrefix <arg>   prefix path for the generated source to
                            properly include the headers. Default : ''
 -p,--prefix <arg>          Project prefix to append to every generated
                            files and symbols
 -s,--srcPath <arg>         path to dump the generated source into.
                            Default : 'output'

```
Example for a fictional project called 'Mut' :
```
./GLBrebis.sh --prefix Mut --srcPath ~/git/mut/src --headerPath ~/git/mut/include/mut --includePrefix mut
```
* `-f,--force` is optional. It simply forces a full re-download of every Khronos GL headers, overwriting the ones that were previously downloaded from the last run.
* `-H,--headerPath` is optional. This is where `MutGL.h` will be dumped into. If not specified, it will be dumped in a new `output`  folder relative to where the script is launched from.
* `-i,--includePrefix` is optional. It's mostly used by `MutGL.c` to do `#include <mut/MutGL.h>` instead of just `#include <MutGL.h>` to help you match your application's file layout requirement.
* `-p,--prefix` is mandatory. If you do `--prefix Mut`, the script will generate `MutGL.c`, and `MutGL.h`, including symbols that will automatically be named after `Mut`.  For example, you will see `_MUT_GL_H_` header guards, and utility functions will be named `mutGLInit()` and `mutGLShutdown()`, etc.
* `-s,--srcPath` is optional. This is where `MutGL.c` will be dumped into. If not specified, it will be dumped in a new `output` folder relative to where the script is launched from.

And if everything goes well, you should have `~/git/mut/src/MutGL.c` and `~/git/mut/include/mut/MutGL.h` on your file system

# How to use the generated extention wrangler code in my project?

1. Drop-in the three generated files into your project
2. Add `<prefix>GL.c` to your CMakeLists.txt/Makefile/VCProj/etc
3. make sure you include `#include "<prefix>GL.h"` instead of including the regular.
4. create your window *and* your GL context.
5. Once the gl context successfully created, simply call `bool success = <prefix>GLInit();`.
6. When shutting down, don't forget to call `<prefix>GLShutdown();`

