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


import java.util.Deque

class GLBrebisGroup implements Comparable<GLBrebisGroup>
{
    String name = ""      // example: GL_ES_VERSION_2_0
    GLBrebisFile file = null
    int startLineNumber = -1
    int endLineNumber = -1
    Deque<GLBrebisDeclaration> declarations = new ArrayDeque<GLBrebisDeclaration>()
    Deque<GLBrebisFunction> functions = new ArrayDeque<GLBrebisFunction>()

    public boolean isVersion()
    {
        return name.matches("^(EGL|GL|GL_ES|GLX|WGL)_VERSION_[0-9]+_[0-9]+\$")
    }

    public boolean isExtension()
    {
        return !isVersion()
    }

    @Override
    public String toString()
    {
        if (file == null)
        {
            "[null][" + name + "]"
        }
        else
        {
            return file.toString() + "[" + name + "]"
        }
    }

    @Override
    public boolean equals(Object other)
    {
        if (other == null) return false
        if (other.getClass() != getClass()) return false
        GLBrebisGroup otherGroup = (GLBrebisGroup)other
        return name.equals(otherGroup.name)
    }

    @Override
    public int hashCode() {
        return name.hashCode()
    }

    @Override
    public int compareTo(GLBrebisGroup other)
    {
        return name.compareTo(other.name)
    }
}
