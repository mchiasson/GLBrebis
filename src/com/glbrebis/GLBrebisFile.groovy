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

class GLBrebisFile implements Comparable<GLBrebisFile>
{
    public String name = ""

    public Deque<GLBrebisGroup> groups = new ArrayDeque<GLBrebisGroup>()

    GLBrebisFile(String name)
    {
        this.name = name
    }

    boolean isGL()
    {
        return name.matches("^GL\\/gl(corearb|ext|ext_extra)\\.h\$")
    }

    boolean isGLES()
    {
        return name.matches("^GLES[0-9]+\\/gl([0-9][0-9]?)?(ext|ext_extra|platform)?\\.h\$")
    }

    boolean isEGL()
    {
        return name.matches("^EGL/egl(ext|ext_extra|platform)?\\.h\$")
    }

    boolean isGLX()
    {
        return name.matches("^GL\\/glx(ext|ext_extra)?\\.h\$")
    }

    boolean isWGL()
    {
        return name.matches("^GL\\/wgl(ext|ext_extra)?\\.h\$")
    }

    @Override
    public String toString()
    {
        return "[" + name + "]"
    }

    @Override
    public boolean equals(Object other)
    {
        if (other == null) return false
        if (other.getClass() != getClass()) return false
        GLBrebisFile otherFile = (GLBrebisFile)other
        return name.equals(otherFile.name)
    }

    @Override
    public int hashCode()
    {
        return name.hashCode()
    }

    @Override
    public int compareTo(GLBrebisFile other)
    {
        return name.compareTo(other.name)
    }
}
