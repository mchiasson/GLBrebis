v1.0.4
======

* fix log entry where a random characters appear
* sorting defines by value first, name after.
* fixed hardcoded 'mutGL' copy-paste error back to <%=prefix%>GL, which is what it was supposed to be.
* GLBrebis now generates strict ANSI-C compliant code
* replaced stdbool in an attempt to make GLBrebis ANSI-C compliant
* Using KHRONOS_APIENTRY from khrplatform.h instead of <PREFIX>_STD_CALL

v1.0.3
======

* fixed Qualcomm Adreno 200 bug where it will print an unwrangled error while already being wrangled
* Fix for 'C1026: parser stack overflow, program too complex' error in VS2015 (was working find in VS2013)

v1.0.2
======

* Added mechanism to allow us to enter missing extension manually in xml/extra.xml
* resolves #1 User can now override their generated getProcAddress with something else
* mutGLInit() Time calculation in Debug
* standardized types to khronos type as much as possible

v1.0.1
======

* resolves #8 GLBrebis now available as a stand-alone web service
* Updated documentation for the new -z,--zip command line flag.
* resolves #7 Auto-archive generated code into a zip file via -z,--zip command line flag

v1.0.0
======

* include path now creates a folder for the generated header.
* --help no longer generates prefixless code.
* updated documentation
* fix for Emscripten
* It's working!
* Finished generating GL.c (Untested)
* Finished generating GL.h (untested)
* Downloading from cmake, parsing downloaded file (much faster that way)
* registry XML parser and code generator (WIP)
* Update LICENSE
* Initial commit
