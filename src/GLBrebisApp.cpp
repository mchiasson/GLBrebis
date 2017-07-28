/*******************************************************************************
 * MIT License
 *
 * Copyright (c) 2017 Mathieu-André Chiasson
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *
 * Disclaimer:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#include "GLBrebisApp.h"

#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>
#include <Poco/File.h>
#include <Poco/Zip/Compress.h>

#include <cctype>
#include <sstream>
#include <iostream>
#include <fstream>

#include "GLBrebisParser.h"
#include "GLBrebisCodeGenerator.h"

#include "xml/gl.xml.h"
#include "xml/extra.xml.h"
#include "KHR/khrplatform.h.h"

#define PARSE_METHOD_DOWNLOAD 1
#define PARSE_METHOD_FILE 2
#define PARSE_METHOD_EMBEDDED 3

#define PARSE_METHOD PARSE_METHOD_EMBEDDED

GLBrebisApp::GLBrebisApp() :
    m_zip(false)
{
}

void GLBrebisApp::initialize(Application& self)
{
    Application::initialize(self);
}

void GLBrebisApp::reinitialize()
{

}

void GLBrebisApp::uninitialize()
{

}

void GLBrebisApp::defineOptions(Poco::Util::OptionSet &options)
{
    Application::defineOptions(options);

    options.addOption(Poco::Util::Option("help", "h", "Usage Information")
                .required(false)
                .repeatable(false)
                .callback(Poco::Util::OptionCallback<GLBrebisApp>(this, &GLBrebisApp::handleHelp)));

    options.addOption(Poco::Util::Option("prefix", "p", "Project prefix to append to every generated files and symbols")
                .required(true)
                .repeatable(false)
                .argument("<prefix>")
                .callback(Poco::Util::OptionCallback<GLBrebisApp>(this, &GLBrebisApp::handlePrefix)));

    options.addOption(Poco::Util::Option("include", "i", "include path prefix to use in the generated GL.c")
                .required(false)
                .repeatable(false)
                .argument("<includepath>")
                .callback(Poco::Util::OptionCallback<GLBrebisApp>(this, &GLBrebisApp::handleInclude)));

    options.addOption(Poco::Util::Option("zip", "z", "Create a zip archive containing all of the generated content.")
                .required(false)
                .repeatable(false)
                .callback(Poco::Util::OptionCallback<GLBrebisApp>(this, &GLBrebisApp::handleZip)));

}

int GLBrebisApp::main(const std::vector<std::string>& args)
{
    GLBrebisParser parser;

#if PARSE_METHOD == PARSE_METHOD_DOWNLOAD

    parser.parse(Poco::URI("https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/master/xml/gl.xml"));
    parser.parse("xml/extra.xml");

#elif PARSE_METHOD == PARSE_METHOD_FILE

    parser.parse("xml/gl.xml");
    parser.parse("xml/extra.xml");

#elif PARSE_METHOD == PARSE_METHOD_EMBEDDED

    parser.parse(gl_xml, sizeof(gl_xml));
    parser.parse(extra_xml, sizeof(extra_xml));

#endif
    Poco::Logger &logger = Poco::Logger::get("GLBrebisApp");

    {
        logger.information("Generating %sGL.c and %sGL.h ...", m_prefix, m_includePath + m_prefix);
        std::ofstream source(m_prefix + "GL.c");
        std::ofstream header(m_includePath + m_prefix + "GL.h");
        GLBrebisCodeGenerator::generateGL(m_prefix, m_includePath, m_zip, parser.getResult(), source, header);
    }

    {
        logger.information("Generating KHR/khrplatform.h ...");
        Poco::File("KHR").createDirectories();
        std::ofstream khrHeader("KHR/khrplatform.h");
        khrHeader << std::string((char*)khrplatform_h, sizeof(khrplatform_h));
    }

    if (m_zip)
    {
        logger.information("Archiving into %sGL.zip ...", m_prefix);
        std::ofstream out(m_prefix + "GL.zip", std::ios::binary);
        Poco::Zip::Compress zipFile(out, true);

        Poco::Path header(m_includePath + m_prefix + "GL.h");
        Poco::Path source(m_prefix + "GL.c");
        Poco::Path khronos("KHR/khrplatform.h");

        zipFile.addFile(header, header);
        zipFile.addFile(source, source);
        zipFile.addFile(khronos, khronos);

        std::stringstream comment;
        Poco::DateTime now;
        std::string nowStr = Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::RFC1123_FORMAT);
        std::string year = "2017"; if (now.year() > 2017) { year += "-" + std::to_string(now.year());}
        comment << "This archive was generated by GLBrebis on " << nowStr << std::endl;
        comment << "https://github.com/mchiasson/GLBrebis" << std::endl;
        comment << "Copyright (c) " << year << " Mathieu-André Chiasson" << std::endl;
        comment << "All rights reserved." << std::endl;
        zipFile.setZipComment(comment.str());

        zipFile.close();
        out.close();

        fflush(NULL);
    }

    logger.information("Done!");


    return EXIT_OK;
}

void GLBrebisApp::handleHelp(const std::string& name, const std::string& value)
{
    Poco::Util::HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("-p <prefix> [-i includepath] [-z]");
    helpFormatter.format(std::cout);
    stopOptionsProcessing();
    exit(EXIT_USAGE);
}

void GLBrebisApp::handlePrefix(const std::string& name, const std::string& value)
{
    m_prefix = value;
}

void GLBrebisApp::handleInclude(const std::string& name, const std::string& value)
{
    m_includePath = value;
    if (m_includePath.length() > 0)
    {
        size_t lastCharPos = m_includePath.length()-1;
        if (m_includePath[lastCharPos] == '\\')
        {
            m_includePath[lastCharPos] = '/';
        }
        else if(m_includePath[lastCharPos] != '/')
        {
            m_includePath += '/';
        }

        Poco::File(m_includePath).createDirectories();
    }
}

void GLBrebisApp::handleZip(const std::string& name, const std::string& value)
{
    m_zip = true;
}
