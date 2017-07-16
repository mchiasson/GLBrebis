/*******************************************************************************
 * Collection of GL extension that are unavailable in neither GLES2/gl2ext.h nor
 * GL/glext.h
 *
 * This header is fake and shouldn't be used directly on your project.
 *
 * I manually gather these missing extensions in this file to the best of my
 * knowledge, but these extensions are most likely untested and might not even
 * work. Use at your own risk!
 *
 * https://github.com/mchiasson/GLBrebis
 *
 *******************************************************************************
 * GLBrebis License (MIT)
 *******************************************************************************
 * Copyright (c) 2015-2017 Mathieu-André Chiasson
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
#ifndef __glext_extra_h_
#define __glext_extra_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

// https://github.com/nigels-com/glew/blob/master/auto/core/gl/GL_EXT_Cg_shader
#ifndef GL_EXT_Cg_shader
#define GL_EXT_Cg_shader 1
#define GL_CG_VERTEX_SHADER_EXT 0x890E
#define GL_CG_FRAGMENT_SHADER_EXT 0x890F
#endif /* GL_EXT_Cg_shader */

// ???
// https://www.opengl.org/discussion_boards/showthread.php/156754-OpenGL-extension-rumor?p=1122374&viewfull=1#post1122374
#ifndef GL_EXTX_framebuffer_mixed_formats
#define GL_EXTX_framebuffer_mixed_formats 1
#endif /* GL_EXTX_framebuffer_mixed_formats */

// defined from GLES2/gl2ext.h
#ifndef GL_EXT_texture_compression_dxt1
#define GL_EXT_texture_compression_dxt1 1
#endif /* GL_EXT_texture_compression_dxt1 */

// https://github.com/nigels-com/glew/blob/master/auto/core/gl/GL_EXT_texture_edge_clamp
#ifndef GL_EXT_texture_edge_clamp
#define GL_EXT_texture_edge_clamp 1
#define GL_CLAMP_TO_EDGE_EXT 0x812F
#endif /* GL_EXT_texture_edge_clamp */

// defined from GLES2/gl2ext.h
#ifndef GL_EXT_texture_storage
#define GL_EXT_texture_storage 1
#endif /* GL_EXT_texture_storage */

// http://developer.download.nvidia.com/opengl/specs/GL_EXT_import_sync_object.txt
#ifndef GL_EXT_import_sync_object
#define GL_EXT_import_sync_object 1
#endif

// https://github.com/nigels-com/glew/blob/master/auto/core/gl/GL_KTX_buffer_region
#ifndef GL_KTX_buffer_region
#define GL_KTX_buffer_region 1
#define GL_KTX_FRONT_REGION 0x0
#define GL_KTX_BACK_REGION 0x1
#define GL_KTX_Z_REGION 0x2
#define GL_KTX_STENCIL_REGION 0x3
GLuint glBufferRegionEnabled(void);
GLuint glNewBufferRegion(GLenum region);
void glDeleteBufferRegion(GLenum region);
void glReadBufferRegion(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height);
void glDrawBufferRegion(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);
#endif

// ???
#ifndef GL_NV_ES1_1_compatibility
#define GL_NV_ES1_1_compatibility 1
#endif

#ifdef __cplusplus
}
#endif

#endif
