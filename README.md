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
GLBrebis -p <prefix>

Available options (use -h for help):
 -h,--help           Usage Information
 -p,--prefix <arg>   Project prefix
```
And if everything goes well, you should have `output/src/<prefix>GL.c`, `output/include/<prefix>GL.h` and `output/include/<prefix>GLext.h` on your file system. Take these three files, and drop them in your project, add <prefix>GL.c to your `CMakeLists.txt`

# How to use the generated extention wrangler code in my project?

1. make sure you include `#include "<prefix>GL.h"` instead of including the regular.
2. create your window *and* your GL context.
3. Once the gl context successfully created, simply call `bool success = <prefix>GLInit();`.
3. When shutting down, don't forget to call `<prefix>GLShutdown();`
