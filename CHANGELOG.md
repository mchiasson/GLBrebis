2018-08-29
==========

  * fixed the inability to use GLU

2018-08-25
==========

  * corrected CMAKE_C_STANDARD
  * Updated changelog
  * update_changelog.sh is now executable
  * downloading khronos api at runtime now (slower but better)
  * changelog update helper script
  * Update README.md
  * downloading khronos api at build time now.
  * changed default prefix to 'Brebis'

2018-08-24
==========

  * removed unecessary stdlib.h include from template header
  * fixed more mistakes in the gl template file
  * fixed some typos in the header template
  * Update README.md
  * Updated changelog
  * fixed generator build issue on Xcode
  * changed license and docs for preparation for stb
  * embedded KHR header into generated header.
  * Removed the generated .c file in favor of STB-style
  * Fixed build issue in linux, and removed the unecessary macros

2018-08-10
==========

  * windows.h->Windows.h case warning fix

2018-08-04
==========

  * Fixed black screen in the sample
    Was introduced on the latest glm where glm::mat4 don't default to
    uninitialized anymore as an optimization strategy.
  * fixed windows build, upgraded copyright year, updated documentation

2018-04-30
==========

  * fixed emscripten build

2018-03-01
==========

  * Update README.md

2018-02-21
==========

  * fixed not defined warning

2018-02-15
==========

  * marked GL_NV_ES3_1_compatibility as known, since it doesn't do anything
  * attempt to fix OSX where it wouldn't be able to load any GL symbols from a null handle
  * fixed the hunter build

2017-08-14
==========

  * removed duplicated emscripten_set_main_loop

2017-07-28
==========

  * replaced {0} by memset since zeroizing struct from initializer was introduced in C99.

2017-07-27
==========

  * changelog
  * fix log entry where a random characters appear
  * sorting defines by value first, name after.
  * fixed hardcoded 'mutGL' copy-paste error back to <%=prefix%>GL, which is what it was supposed to be.

2017-07-26
==========

  * GLBrebis now generates strict ANSI-C compliant code
  * replaced stdbool in an attempt to make GLBrebis ANSI-C compliant

2017-07-25
==========

  * Using KHRONOS_APIENTRY from khrplatform.h instead of <PREFIX>_STD_CALL
  * changelog
  * fixed Qualcomm Adreno 200 bug where it will print an unwrangled error while already being wrangled

2017-07-23
==========

  * Fix for 'C1026: parser stack overflow, program too complex' error in
    VS2015 (was working find in VS2013)
    Looks like it is a bug with VS2015's compiler:
    https://connect.microsoft.com/VisualStudio/feedback/details/1572286/program-too-complex-used-to-compile-with-vs-2013-but-not-anymore
    Still going ahead with this even if Microsoft promised that it is going to
    be fixed on the next version of VS2015: nobody likes to upgrade visual
    studio because it is painfully slow.

2017-07-22
==========

  * changelog
  * Added mechanism to allow us to enter missing extension manually in xml/extra.xml
  * resolves [#1](https://github.com/mchiasson/GLBrebis/issues/1) User can now override their generated getProcAddress with something else
  * mutGLInit() Time calculation in Debug
  * standardized types to khronos type as much as possible
  * changelog + docs
  * resolves [#8](https://github.com/mchiasson/GLBrebis/issues/8) GLBrebis now available as a stand-alone web service
  * Updated documentation for the new -z,--zip command line flag.
  * resolves [#7](https://github.com/mchiasson/GLBrebis/issues/7) Auto-archive generated code into a zip file via -z,--zip command line flag
  * Changelog and license update
  * include path now creates a folder for the generated header.
  * --help no longer generates prefixless code.
  * updated documentation
  * fix for Emscripten

2017-07-21
==========

  * It's working!

2017-07-20
==========

  * Finished generating GL.c (Untested)

2017-07-19
==========

  * Finished generating GL.h (untested)
  * Downloading from cmake, parsing downloaded file (much faster that way)
  * registry XML parser and code generator (WIP)

2017-07-11
==========

  * Update LICENSE
  * Initial commit
