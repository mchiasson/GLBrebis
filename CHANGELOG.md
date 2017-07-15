v1.0.2 / 2017-07-15
===================

  * fixed shader major/minor detection bug where it sometimes shows as 3.30 instead of 330
  * fixed missing GL function pointers in windows. wglGetProcAddress doesn't work for GL 1.1 functions.
  * fixed compile error in windows, because 'near' and 'far' are reserved keywords in MSVC
  * fixed glGetProgramPipelineivEXT extension, which has a variation between GL and GLES (the only one so far, knock on wood)
  * Reduce LOC count by making single-line function calls
  * Merged GlExt into GL generated header. It's still under 20KLOC.
  * changed the default ouptut folder. instead of output/src and output/include, they all go in the same output/
  * reinforced the parser result to make it resilient against extensions functions appearing as regular GL functions
  * Added some missing unwrangled extra extensions that are not part of Khronos's official headers.

v1.0.1 / 2017-07-12
===================

  * New feature: ability to set destination src and header path, and an include prefix for `#include` lines.
  * Merge branch 'master' of github.com:mchiasson/GLBrebis
  * Moving things around a bit
  * Update README.md
    fixed a header
  * Added better documentation

v1.0.0 / 2017-07-11
===================

  * GLBrebis v1.0.0
  * Changed the description of the project
  * Adding required CDT libraries to initiate the project
  * Update LICENSE
  * Initial commit
