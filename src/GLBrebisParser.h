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

#ifndef GLBREBISPARSER_H
#define GLBREBISPARSER_H

#include <Poco/URI.h>
#include <rapidxml/rapidxml.hpp>
#include "GLBrebisData.h"


class GLBrebisParser
{
public:
    GLBrebisParser();
    void parse(const Poco::URI &uri);
    void parse(const std::string &path);
    void parse(const unsigned char *buffer, size_t bufferSize);

    const std::string &getRawContent() const { return m_rawcontent; }
    const GLBrebisData &getResult() const { return m_result; }

protected:
    void parse();
    void parseType(GLBrebisData::Type &enum_, rapidxml::xml_node<> *pEnumNode);
    void parseEnums(GLBrebisData::Enums &enums, rapidxml::xml_node<> *pEnumsNode);
    void parseEnum(GLBrebisData::Enum &enum_, rapidxml::xml_node<> *pEnumNode);
    void parseCommand(GLBrebisData::Command &command, rapidxml::xml_node<> *pCommandNode);
    void parseProto(GLBrebisData::Proto &proto, rapidxml::xml_node<> *pProtoNode);
    void parseParam(GLBrebisData::Param &param, rapidxml::xml_node<> *pParamNode);
    void parseFeature(GLBrebisData::Feature &feature, rapidxml::xml_node<> *pFeatureNode);
    void parseExtension(GLBrebisData::Extension &feature, rapidxml::xml_node<> *pExtensionNode);

    std::string m_rawcontent;
    GLBrebisData m_result;

};

#endif // GLBREBISPARSER_H
