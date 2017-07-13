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


class GLBrebisFunction implements Comparable<GLBrebisFunction>
{
    GLBrebisGroup group = null
    String returnParam = ""      // example: void
    String name = ""             // example: glClearColor
    String pointerName = ""      // example: PFNGLCLEARCOLORPROC
    String paramDeclaration = "" // example: GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha
    String paramCall = ""        // example: red, green, blue, alpha
    String signature = ""
    int lineNumber = -1

    @Override
    public String toString()
    {
        return group.toString() + "[" + name + "]"
    }

    @Override
    public boolean equals(Object other)
    {
        if (other == null) return false
        if (other.getClass() != getClass()) return false
        GLBrebisFunction otherFunction = (GLBrebisFunction)other
        return name.equals(otherFunction.name)
    }

    @Override
    public int hashCode() {
        return name.hashCode()
    }

    @Override
    public int compareTo(GLBrebisFunction other)
    {
        return name.compareTo(other.name)
    }
}
