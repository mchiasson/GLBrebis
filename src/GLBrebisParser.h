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
