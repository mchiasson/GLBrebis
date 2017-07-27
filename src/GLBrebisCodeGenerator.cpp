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

#include "GLBrebisCodeGenerator.h"
#include "GLBrebisUtilities.h"

#include "template/GL.c.template.h"
#include "template/GL.h.template.h"

#include <string>
#include <cctype>
#include <cstdio>
#include <sstream>
#include <fstream>

#include <Poco/Logger.h>
#include <Poco/String.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>

void GLBrebisCodeGenerator::generateGL(const std::string &inPrefix,
                                       const std::string &includePath,
                                       bool zip,
                                       const GLBrebisData &result,
                                       std::ostream &sourceOut,
                                       std::ostream &headerOut)
{
    std::string Prefix = inPrefix; std::toupper(Prefix[0]);
    std::string prefix = Poco::toLower(inPrefix);
    std::string PREFIX = Poco::toUpper(inPrefix);

    Poco::DateTime now;
    std::string nowStr = Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::RFC1123_FORMAT);
    std::string year = "2017"; if (now.year() > 2017) { year += "-" + std::to_string(now.year());}

    std::vector<GLBrebisData::Enum> uniqueDefines = result.getAllUniqueDefines();
    std::stringstream defineBlock;
    for(size_t i = 0; i < uniqueDefines.size(); ++i)
    {
        defineBlock << "#define " << uniqueDefines[i].name;
        /*if (uniqueDefines[i].alias.length() > 0)
        {
            defineBlock << " " << uniqueDefines[i].alias;
        }
        else */if (uniqueDefines[i].value.length() > 0)
        {
            defineBlock << " " << uniqueDefines[i].value;
        }

        if (uniqueDefines[i].api.length() > 0)
        {
            defineBlock << " /* for " << uniqueDefines[i].api << " */";
        }

        defineBlock  << std::endl;
    }

    int IdCount = 1;

    std::stringstream versionBlock;
    for(size_t i = 0; i < result.registry.features.size(); ++i)
{
        const GLBrebisData::Feature &feature = result.registry.features[i];
        versionBlock << "#undef " << feature.name << std::endl;
        versionBlock << "#define " << feature.name << " " << IdCount++ << std::endl;
    }

    std::stringstream extensionBlock;
    for(size_t i = 0; i < result.registry.extensions.size(); ++i)
    {
        const GLBrebisData::Extension &extension = result.registry.extensions[i];
        extensionBlock << "#undef " << extension.name << std::endl;
        extensionBlock << "#define " << extension.name << " " << IdCount++ << std::endl;
    }

    std::vector<GLBrebisData::Type> uniqueTypes = result.getAllUniqueTypes();
    std::stringstream typeBlock;
    for(size_t i = 0; i < uniqueTypes.size(); ++i) {
        const GLBrebisData::Type &type = uniqueTypes[i];
        typeBlock << type.signature << std::endl;
    }

    std::stringstream funcPtrBlock;
    std::stringstream funcImplBlock;
    std::stringstream funcDefineBlock;
    std::vector<GLBrebisData::Command> uniqueCommands = result.getAllUniqueCommands();
    for(size_t i = 0; i < uniqueCommands.size(); ++i)
    {
        const GLBrebisData::Command &command = uniqueCommands[i];

        /////////////// funcPtrBlock ////////////////////

        funcPtrBlock << "    " << command.proto.signature << "(KHRONOS_APIENTRY *" << &command.proto.name[2] << ")(";
        if (command.params.size() == 0)
        {
            funcPtrBlock << "void"; // for strict ANSI-C compliance
        }
        else
        {
            for (size_t j = 0; j < command.params.size(); ++j)
            {
                const GLBrebisData::Param &param = command.params[j];
                if (j > 0) funcPtrBlock << ", ";
                funcPtrBlock << param.signature;
            }
        }
        funcPtrBlock << ");" << std::endl;

        /////////////// funcImplBlock ////////////////////

        funcImplBlock << PREFIX << "_FORCE_INLINE " << command.proto.signature << " " << prefix << &command.proto.name[2] << "(";
        if (command.params.size() == 0)
        {
            funcImplBlock << "void"; // for strict ANSI-C compliance
        }
        else
        {
            for (size_t j = 0; j < command.params.size(); ++j)
            {
                const GLBrebisData::Param &param = command.params[j];
                if (j > 0) funcImplBlock << ", ";
                funcImplBlock << param.signatureFull;
            }
        }
        funcImplBlock << ") { ";
        if (command.proto.name == "glGetProgramPipelineivEXT")
        {
            funcImplBlock << "if (" << prefix << "GLSupport(GL_ES_VERSION_2_0) && (pname == GL_CURRENT_PROGRAM)) { pname = GL_ACTIVE_PROGRAM; } else if (" << prefix << "GLSupport(GL_VERSION_1_0) && (pname == GL_ACTIVE_PROGRAM)) { pname = GL_CURRENT_PROGRAM; } ";
        }
        if (command.proto.signature.compare(0,6,"void *") == 0 || command.proto.signature.compare(0,5,"void ") != 0)
        {
            funcImplBlock << "return ";
        }
        funcImplBlock << prefix << "GL." << &command.proto.name[2] << "(";
        for (size_t j = 0; j < command.params.size(); ++j)
        {
            const GLBrebisData::Param &param = command.params[j];
            if (j > 0) funcImplBlock << ", ";
            funcImplBlock << param.name;
        }
        funcImplBlock << "); }" << std::endl;

        /////////////// funcDefineBlock ////////////////////

        funcDefineBlock << "#define " << command.proto.name << " " << prefix << &command.proto.name[2] << std::endl;

    }

    std::stringstream glesAddExtensionBlock;
    std::vector<std::string> uniqueGLESExtensions = result.getAllUniqueGLESExtensions();
    for (size_t i = 0; i < uniqueGLESExtensions.size(); ++i)
    {
        const std::string &extension = uniqueGLESExtensions[i];
        glesAddExtensionBlock << "        if (!" << prefix << "GL.support[" << extension <<  "] && (extensionStrLength == " << extension.length() << ") && strncmp(extensionStr, \"" << extension <<  "\", extensionStrLength) == 0) { " << prefix << "GL.support[" << extension <<  "] = KHRONOS_TRUE; return; }" << std::endl;
    }

    std::stringstream glAddExtensionBlock;
    std::vector<std::string> uniqueGLExtensions = result.getAllUniqueGLExtensions();
    for (size_t i = 0; i < uniqueGLExtensions.size(); ++i)
    {
        const std::string &extension = uniqueGLExtensions[i];
        glAddExtensionBlock << "        if (!" << prefix << "GL.support[" << extension <<  "] && (extensionStrLength == " << extension.length() << ") && strncmp(extensionStr, \"" << extension <<  "\", extensionStrLength) == 0) { " << prefix << "GL.support[" << extension <<  "] = KHRONOS_TRUE; return; }" << std::endl;
    }

    std::stringstream getProcBlock;
    for(size_t i = 0; i < uniqueCommands.size(); ++i)
    {
        const GLBrebisData::Command &command = uniqueCommands[i];
        getProcBlock << "    " << prefix << "GL." << &command.proto.name[2] << " = (" << command.proto.signature << "(KHRONOS_APIENTRY *)(";
        if (command.params.size() == 0)
        {
            getProcBlock << "void"; // for strict ANSI-C compliance
        }
        else
        {
            for (size_t j = 0; j < command.params.size(); ++j)
            {
                const GLBrebisData::Param &param = command.params[j];
                if (j > 0) getProcBlock << ", ";
                getProcBlock << param.signature;
            }
        }
        getProcBlock << "))" << prefix << "GLGetProcAddress(\"" << command.proto.name << "\");" << std::endl;
    }

    std::stringstream glesSupportBlock;
    std::stringstream glSupportBlock;
    for(size_t i = 0; i < result.registry.features.size(); ++i)
    {
        const GLBrebisData::Feature &feature = result.registry.features[i];

        int majorVersion=-1, minorVersion=-1;
        sscanf(feature.number.c_str(), "%d.%d", &majorVersion, &minorVersion);

        if (feature.api == "gl")
        {
            glSupportBlock << "        " << prefix << "GL.support[" << feature.name << "] = ((versionMajor > " << majorVersion << ") || ((versionMajor == "<< majorVersion << ") && (versionMinor >= " << minorVersion << ")));" << std::endl;
        }
        else
        {
            glesSupportBlock << "        " << prefix << "GL.support[" << feature.name << "] = ((versionMajor > " << majorVersion << ") || ((versionMajor == "<< majorVersion << ") && (versionMinor >= " << minorVersion << ")));" << std::endl;
        }
    }

    std::string content;

    //GLBrebisUtilities::readFile("template/GL.c.template", header);
    content = std::string((char*)GL_c_template, sizeof(GL_c_template));
    Poco::replaceInPlace(content, "<%=now%>", nowStr.c_str());
    Poco::replaceInPlace(content, "<%=year%>", year.c_str());
    Poco::replaceInPlace(content, "<%=prefix%>", prefix.c_str());
    Poco::replaceInPlace(content, "<%=Prefix%>", Prefix.c_str());
    Poco::replaceInPlace(content, "<%=PREFIX%>", PREFIX.c_str());
    Poco::replaceInPlace(content, "<%=includePrefix%>", includePath.c_str());
    Poco::replaceInPlace(content, "<%=glesAddExtensionBlock%>", glesAddExtensionBlock.str().c_str());
    Poco::replaceInPlace(content, "<%=glAddExtensionBlock%>", glAddExtensionBlock.str().c_str());
    Poco::replaceInPlace(content, "<%=getProcBlock%>", getProcBlock.str().c_str());
    Poco::replaceInPlace(content, "<%=glSupportBlock%>", glSupportBlock.str().c_str());
    Poco::replaceInPlace(content, "<%=glesSupportBlock%>", glesSupportBlock.str().c_str());
    sourceOut << content;

    //GLBrebisUtilities::readFile("template/GL.h.template", header);
    content = std::string((char*)GL_h_template, sizeof(GL_h_template));
    Poco::replaceInPlace(content, "<%=now%>", nowStr.c_str());
    Poco::replaceInPlace(content, "<%=year%>", year.c_str());
    Poco::replaceInPlace(content, "<%=prefix%>", prefix.c_str());
    Poco::replaceInPlace(content, "<%=Prefix%>", Prefix.c_str());
    Poco::replaceInPlace(content, "<%=PREFIX%>", PREFIX.c_str());
    Poco::replaceInPlace(content, "<%=defineBlock%>", defineBlock.str().c_str());
    Poco::replaceInPlace(content, "<%=versionBlock%>", versionBlock.str().c_str());
    Poco::replaceInPlace(content, "<%=extensionBlock%>", extensionBlock.str().c_str());
    Poco::replaceInPlace(content, "<%=typeBlock%>", typeBlock.str().c_str());
    Poco::replaceInPlace(content, "<%=IdCount%>", std::to_string(IdCount).c_str());
    Poco::replaceInPlace(content, "<%=funcPtrBlock%>", funcPtrBlock.str().c_str());
    Poco::replaceInPlace(content, "<%=funcImplBlock%>", funcImplBlock.str().c_str());
    Poco::replaceInPlace(content, "<%=funcDefineBlock%>", funcDefineBlock.str().c_str());
    headerOut << content;

}
