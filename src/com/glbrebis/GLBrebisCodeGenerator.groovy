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

import java.util.Map;
import java.util.regex.Matcher
import java.util.regex.Pattern;

class GLBrebisCodeGenerator
{
    public GLBrebisCodeGenerator(String Prefix, String srcPath, String headerPath, String includePrefix, GLBrebisParserResult result)
    {
        def year = Calendar.getInstance().get(Calendar.YEAR);
        def engine = new groovy.text.GStringTemplateEngine()
        def binding = [
            now           : def now = new Date().format("yyyy-MM-dd HH:mm:ssZ"),
            year          : "2015-" + String.valueOf(year),
            result        : result,
            Prefix        : Prefix,
            prefix        : Prefix.toLowerCase(),
            PREFIX        : Prefix.toUpperCase(),
            includePrefix : includePrefix,
            generator     : this
        ]

        def source
        def template
        def writer

        new File(srcPath).mkdirs();
        new File(headerPath).mkdirs();

        System.out.print("Generating " + Prefix + "GL.c ... ")
        source = new File('templates/GL.c.in').getText("UTF-8")
        template = engine.createTemplate(source).make(binding)
        writer = new PrintWriter(new File(srcPath + Prefix + "GL.c"), "UTF-8")
        writer.print(template.toString())
        writer.close()
        System.out.println("Done!")

        System.out.print("Generating " + Prefix + "GL.h ... ")
        source = new File('templates/GL.h.in').getText("UTF-8")
        template = engine.createTemplate(source).make(binding)
        writer = new PrintWriter(new File(headerPath + Prefix + "GL.h"), "UTF-8")
        writer.print(template.toString())
        writer.close()
        System.out.println("Done!")
    }
}
