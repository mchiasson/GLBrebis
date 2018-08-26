/*******************************************************************************
 * MIT License
 *
 * Copyright (c) 2017-2018 Mathieu-André Chiasson
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

GLBrebisApp::GLBrebisApp() :
    m_prefix("Brebis")
{
    setUnixOptions(true);
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
                .required(false)
                .repeatable(false)
                .argument("<prefix>")
                .callback(Poco::Util::OptionCallback<GLBrebisApp>(this, &GLBrebisApp::handlePrefix)));
}

int GLBrebisApp::main(const std::vector<std::string>& args)
{
    GLBrebisParser parser;

    parser.parse(gl_xml, sizeof(gl_xml));
    parser.parse(extra_xml, sizeof(extra_xml));

    Poco::Logger &logger = Poco::Logger::get("GLBrebisApp");

    logger.information("Generating %sGL.h ...", m_prefix);
    std::ofstream header(m_prefix + "GL.h");
    GLBrebisCodeGenerator::generateHeader(m_prefix, parser.getResult(), header);
    logger.information("Done!");

    return EXIT_OK;
}

void GLBrebisApp::handleHelp(const std::string& name, const std::string& value)
{
    Poco::Util::HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("-p <prefix>");
    helpFormatter.format(std::cout);
    stopOptionsProcessing();
    exit(EXIT_USAGE);
}

void GLBrebisApp::handlePrefix(const std::string& name, const std::string& value)
{
    m_prefix = value;
}
