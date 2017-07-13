/*******************************************************************************
 * MIT License
 * 
 * Copyright (c) 2017 Mathieu-André Chiasson
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
package com.glbrebis

class GLBrebis
{
    static main(args)
    {
        def cli = new CliBuilder(
            usage: 'GLBrebis -p <prefix> [-s /path/to/src] [-H /path/to/include] [-i includePrefix]',
            header: '\nAvailable options (use -h for help):\n')

        cli.with {
            h(longOpt: 'help', 'Usage Information', required: false)
            p(longOpt: 'prefix', 'Project prefix to append to every generated files and symbols',                                args: 1, required: true)
            H(longOpt: 'headerPath', 'path to dump the generated headers into. Default : \'output/include\' ',                   args: 1, required: false)
            s(longOpt: 'srcPath', 'path to dump the generated source into. Default : \'output/src\' ',                           args: 1, required: false)
            i(longOpt: 'includePrefix', 'prefix path for the generated source to properly include the headers. Default : \'\' ', args: 1, required: false)
        }

        def opt = cli.parse(args)
        if (!opt) return
        if (opt.h) cli.usage()
        
        def prefix = opt.p

        def srcPath = "output/src/"
        if (opt.s) {
            srcPath = opt.s
            if (!srcPath.endsWith('/')) {
                srcPath += '/'
            }
        }

        def headerPath = "output/include/"
        if (opt.H) {
            headerPath = opt.H
            if (!headerPath.endsWith('/')) {
                headerPath += '/'
            }
        }

        def includePrefix = ""
        if (opt.i) {
            includePrefix = opt.i
            if (!includePrefix.endsWith('/')) {
                includePrefix += '/'
            }
        }

        GLBrebisASTParser parser = new GLBrebisASTParser()
        parser.parse("https://www.khronos.org/registry/gles/api/GLES3/gl32.h", "GLES3/gl32.h")
        parser.parse("https://www.khronos.org/registry/gles/api/GLES2/gl2ext.h", "GLES2/gl2ext.h")
        parser.parse("https://www.opengl.org/registry/api/GL/glcorearb.h", "GL/glcorearb.h")
        parser.parse("https://www.opengl.org/registry/api/GL/glext.h", "GL/glext.h")
        
        new GLBrebisCodeGenerator(prefix, srcPath, headerPath, includePrefix, parser.getResults())
    }
}