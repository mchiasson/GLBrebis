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

import java.io.BufferedReader
import java.io.IOException
import java.io.InputStream
import java.io.InputStreamReader
import java.io.PrintWriter
import java.net.URL
import java.net.URLConnection
import java.nio.file.Files
import java.nio.file.Path
import java.nio.file.Paths
import static java.nio.file.StandardCopyOption.*;
import java.util.Map
import java.util.regex.Matcher
import java.util.regex.Pattern
import java.util.zip.GZIPInputStream

class GLBrebisUtilities
{
    public static String download(String url, String fileName)
    {
        print("Downloading " + url + " ... ")
        def out = new BufferedOutputStream(new FileOutputStream(fileName))
        out << new URL(url).openStream()
        out.close()
        println("Done!")
        return readFile(fileName)
    }

    public static String writeFile(String fileName, String content)
    {
        File targetFile = new File(fileName)
        if (!targetFile.exists())
        {
            File parent = targetFile.getParentFile()
            if (!parent.exists())
            {
                parent.mkdirs()
            }
        }

        PrintWriter writer = new PrintWriter(fileName, "UTF-8")
        writer.print(content)
        writer.close()
    }

    public static String readFile(String fileName)
    {
        Path path = Paths.get(fileName)
        if (Files.exists(path))
        {
            byte[] encoded = Files.readAllBytes(path)
            return new String(encoded, "UTF-8")
        }
        System.err.println(fileName + " does not exist!")
        return ""
    }

    public static String capitalizeFirstChar(String s)
    {
        return (s.length() == 1 ? s.toUpperCase() : s[0].toUpperCase() + s[1..-1])
    }
}