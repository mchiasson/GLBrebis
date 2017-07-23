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

#include "GLBrebisParser.h"

#include <cstring>
#include <sstream>
#include <Poco/Logger.h>
#include <Poco/String.h>

#include "GLBrebisUtilities.h"

static const char * types_node = "types";
static const char * type_node = "type";
static const char * enums_node = "enums";
static const char * enum_node = "enum";
static const char * commands_node = "commands";
static const char * command_node = "command";
static const char * proto_node = "proto";
static const char * param_node = "param";
static const char * ptype_node = "ptype";
static const char * name_node = "name";
static const char * feature_node = "feature";
static const char * require_node = "require";
static const char * extensions_node = "extensions";
static const char * extension_node = "extension";

#define IS_NODE(pNode, type) strncmp(pNode->name(), type##_node, strlen(type##_node)) == 0
#define VALUE_TO_STR(pNode) std::string(pNode->value(), pNode->value_size())

GLBrebisParser::GLBrebisParser()
{
}

void GLBrebisParser::parse(const Poco::URI &uri)
{
    Poco::Logger &logger = Poco::Logger::get("GLBrebisParser");

    logger.information("Downloading %s ... ", uri.getPath());
    m_rawcontent = GLBrebisUtilities::download(uri);

    logger.information("Parsing %s ... ", uri.getPath());
    parse();
}

void GLBrebisParser::parse(const std::string &path)
{
    Poco::Logger &logger = Poco::Logger::get("GLBrebisParser");

    logger.information("Loading %s ... ", path);
    GLBrebisUtilities::readFile(path, m_rawcontent);

    logger.information("Parsing %s ... ", path);
    parse();
}

void GLBrebisParser::parse(const unsigned char *buffer, size_t bufferSize)
{
    m_rawcontent = std::string((char*)buffer, bufferSize);
    parse();
}

void GLBrebisParser::parse()
{
    rapidxml::xml_document<> document;
    document.parse<0>(&m_rawcontent[0]);

    rapidxml::xml_node<> *pRegistry = document.first_node();

    for (rapidxml::xml_node<> *pChildNode = pRegistry->first_node(); pChildNode; pChildNode = pChildNode->next_sibling())
    {
        if (IS_NODE(pChildNode, types))
        {
            for(rapidxml::xml_node<> *pTypeNode = pChildNode->first_node(type_node); pTypeNode; pTypeNode = pTypeNode->next_sibling())
            {
                m_result.registry.types.resize(m_result.registry.types.size()+1);
                parseType(m_result.registry.types.back(), pTypeNode);
            }
        }
        if (IS_NODE(pChildNode, enums))
        {
            m_result.registry.enums.resize(m_result.registry.enums.size()+1);
            parseEnums(m_result.registry.enums.back(), pChildNode);
        }
        else if (IS_NODE(pChildNode, commands))
        {
            for(rapidxml::xml_node<> *pCommandNode = pChildNode->first_node(command_node); pCommandNode; pCommandNode = pCommandNode->next_sibling())
            {
                m_result.registry.commands.resize(m_result.registry.commands.size()+1);
                parseCommand(m_result.registry.commands.back(), pCommandNode);
            }
        }
        else if (IS_NODE(pChildNode, feature))
        {
            m_result.registry.features.resize(m_result.registry.features.size()+1);
            parseFeature(m_result.registry.features.back(), pChildNode);
        }
        else if (IS_NODE(pChildNode, extensions))
        {
            for(rapidxml::xml_node<> *pExtension = pChildNode->first_node(extension_node); pExtension; pExtension = pExtension->next_sibling())
            {
                m_result.registry.extensions.resize(m_result.registry.extensions.size()+1);
                parseExtension(m_result.registry.extensions.back(), pExtension);
            }
        }
    }
}

void GLBrebisParser::parseType(GLBrebisData::Type &type, rapidxml::xml_node<> *pTypeNode)
{
    rapidxml::xml_attribute<> *pName = pTypeNode->first_attribute("name");
    if (pName)
    {
        std::string nameAttr = VALUE_TO_STR(pName);
        if (nameAttr == "stddef" || nameAttr == "khrplatform" || nameAttr == "inttypes") return; // not interested in those at all.
    }

    rapidxml::xml_attribute<> *pApi = pTypeNode->first_attribute("api");
    if (pApi) { type.api = VALUE_TO_STR(pApi); }

    std::stringstream signature;
    for (rapidxml::xml_node<> *pChildNode = pTypeNode->first_node(); pChildNode; pChildNode = pChildNode->next_sibling())
    {
        signature << VALUE_TO_STR(pChildNode);
    }
    type.signature = signature.str();
    rapidxml::xml_node<> *pNameNode = pTypeNode->first_node(name_node);
    if (pNameNode) {
        type.name = VALUE_TO_STR(pNameNode);
    }

}


void GLBrebisParser::parseEnums(GLBrebisData::Enums &enums, rapidxml::xml_node<> *pEnumsNode)
{
    // <enums namespace="GL" group="AttribMask" type="bitmask">

    rapidxml::xml_attribute<> *pNamespace = pEnumsNode->first_attribute("namespace");
    rapidxml::xml_attribute<> *pGroup = pEnumsNode->first_attribute("group");
    rapidxml::xml_attribute<> *pType = pEnumsNode->first_attribute("type");

    if (pNamespace) enums.namespace_  = VALUE_TO_STR(pNamespace);
    if (pGroup) enums.group  = VALUE_TO_STR(pGroup);
    if (pType) enums.type  = VALUE_TO_STR(pType);

    for (rapidxml::xml_node<> *pChildNode = pEnumsNode->first_node(); pChildNode; pChildNode = pChildNode->next_sibling())
    {
        if (IS_NODE(pChildNode, enum))
        {
            enums.entries.resize(enums.entries.size()+1);
            parseEnum(enums.entries.back(), pChildNode);
        }
    }
}

void GLBrebisParser::parseEnum(GLBrebisData::Enum &enum_, rapidxml::xml_node<> *pEnumNode)
{
    // <enum value="0x00000001" name="GL_CURRENT_BIT"/>
    // <enum value="0x8259" api="gles2" name="GL_ACTIVE_PROGRAM_EXT" comment="For the OpenGL ES version of EXT_separate_shader_objects"/>
    // <enum value="0x8B8D" api="gl" name="GL_ACTIVE_PROGRAM_EXT" alias="GL_CURRENT_PROGRAM" comment="For the OpenGL version of EXT_separate_shader_objects"/>

    rapidxml::xml_attribute<> *pValue = pEnumNode->first_attribute("value");
    rapidxml::xml_attribute<> *pName = pEnumNode->first_attribute("name");
    rapidxml::xml_attribute<> *pApi = pEnumNode->first_attribute("api");
    rapidxml::xml_attribute<> *pAlias = pEnumNode->first_attribute("alias");
    rapidxml::xml_attribute<> *pType = pEnumNode->first_attribute("type");


    if (pValue) enum_.value = VALUE_TO_STR(pValue);
    if (pName) enum_.name = VALUE_TO_STR(pName);
    if (pApi) enum_.api = VALUE_TO_STR(pApi);
    if (pAlias) enum_.alias = VALUE_TO_STR(pAlias);
    if (pType) enum_.value += VALUE_TO_STR(pType);

}

void GLBrebisParser::parseCommand(GLBrebisData::Command &command, rapidxml::xml_node<> *pCommandNode)
{
    // <command>
    for (rapidxml::xml_node<> *pChildNode = pCommandNode->first_node(); pChildNode; pChildNode = pChildNode->next_sibling())
    {
        if (IS_NODE(pChildNode, proto))
        {
            parseProto(command.proto, pChildNode);
        }
        else if (IS_NODE(pChildNode, param))
        {
            command.params.resize(command.params.size()+1);
            parseParam(command.params.back(), pChildNode);
        }
    }

}

void GLBrebisParser::parseProto(GLBrebisData::Proto &proto, rapidxml::xml_node<> *pProtoNode)
{
    // Some variations with that one.
    // <proto>void <name>glBindMultiTextureEXT</name></proto>
    // <proto><ptype>GLboolean</ptype> <name>glAcquireKeyedMutexWin32EXT</name></proto>
    // <proto group="Boolean"><ptype>GLboolean</ptype> <name>glIsFenceNV</name></proto>

    rapidxml::xml_attribute<> *pGroup = pProtoNode->first_attribute("group");
    if (pGroup) proto.group  = VALUE_TO_STR(pGroup);

    rapidxml::xml_node<> *pNameNode = pProtoNode->first_node(name_node);
    if (pNameNode) proto.name = pNameNode->value();

    std::stringstream signature;
    for (rapidxml::xml_node<> *pChildNode = pProtoNode->first_node(); pChildNode; pChildNode = pChildNode->next_sibling())
    {
        if (!IS_NODE(pChildNode, name))
        {
            signature << VALUE_TO_STR(pChildNode);
        }
    }
    proto.signature = signature.str();
}

void GLBrebisParser::parseParam(GLBrebisData::Param &param, rapidxml::xml_node<> *pParamNode)
{
    // Some variations with that one.
    // <param group="AccumOp"><ptype>GLenum</ptype> <name>op</name></param>
    // <param><ptype>GLDEBUGPROCAMD</ptype> <name>callback</name></param>
    // <param>void *<name>userParam</name></param>

    rapidxml::xml_attribute<> *pGroup = pParamNode->first_attribute("group");
    if (pGroup) param.group  = VALUE_TO_STR(pGroup);

    rapidxml::xml_node<> *pNameNode = pParamNode->first_node(name_node);
    if (pNameNode) param.name = pNameNode->value();

    std::stringstream signature;
    std::stringstream signatureFull;
    for (rapidxml::xml_node<> *pChildNode = pParamNode->first_node(); pChildNode; pChildNode = pChildNode->next_sibling())
    {
        if (IS_NODE(pChildNode, name))
        {
            signatureFull << " " << VALUE_TO_STR(pChildNode);
        }
        else
        {
            signature << VALUE_TO_STR(pChildNode);
            signatureFull << VALUE_TO_STR(pChildNode);
        }
    }
    param.signature = signature.str();
    param.signatureFull = signatureFull.str();
}

void GLBrebisParser::parseFeature(GLBrebisData::Feature &feature, rapidxml::xml_node<> *pFeatureNode)
{
     // <feature api="gl" name="GL_VERSION_3_0" number="3.0">
    rapidxml::xml_attribute<> *pApi = pFeatureNode->first_attribute("api");
    rapidxml::xml_attribute<> *pName = pFeatureNode->first_attribute("name");
    rapidxml::xml_attribute<> *pNumber = pFeatureNode->first_attribute("number");

    if (pApi) feature.api = VALUE_TO_STR(pApi);
    if (pName) feature.name = VALUE_TO_STR(pName);
    if (pNumber) feature.number = VALUE_TO_STR(pNumber);

    rapidxml::xml_node<> *pRequireNode = pFeatureNode->first_node(require_node);
    if (pRequireNode)
    {
        for (rapidxml::xml_node<> *pChildNode = pRequireNode->first_node(); pChildNode; pChildNode = pChildNode->next_sibling())
        {
            if (IS_NODE(pChildNode, enum))
            {
                pName = pChildNode->first_attribute("name");
                if (pName ) feature.enums.push_back(VALUE_TO_STR(pName));
            }
            else if (IS_NODE(pChildNode, command))
            {
                pName = pChildNode->first_attribute("name");
                if (pName ) feature.commands.push_back(VALUE_TO_STR(pName));
            }
        }
    }

}

void GLBrebisParser::parseExtension(GLBrebisData::Extension &extension, rapidxml::xml_node<> *pExtensionNode)
{
    //<extension name="GL_EXT_map_buffer_range" supported="gles1|gles2">
    rapidxml::xml_attribute<> *pName = pExtensionNode->first_attribute("name");
    rapidxml::xml_attribute<> *pSupported = pExtensionNode->first_attribute("supported");

    if (pName) extension.name = VALUE_TO_STR(pName);
    if (pSupported) extension.supported = VALUE_TO_STR(pSupported);

    rapidxml::xml_node<> *pRequireNode = pExtensionNode->first_node(require_node);
    if (pRequireNode)
    {
        for (rapidxml::xml_node<> *pChildNode = pRequireNode->first_node(); pChildNode; pChildNode = pChildNode->next_sibling())
        {
            if (IS_NODE(pChildNode, enum))
            {
                pName = pChildNode->first_attribute("name");
                if (pName ) extension.enums.push_back(VALUE_TO_STR(pName));
            }
            else if (IS_NODE(pChildNode, command))
            {
                pName = pChildNode->first_attribute("name");
                if (pName ) extension.commands.push_back(VALUE_TO_STR(pName));
            }
        }
    }

}

