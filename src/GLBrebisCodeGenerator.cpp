#include "GLBrebisCodeGenerator.h"
#include "GLBrebisUtilities.h"

#include <string>
#include <cctype>
#include <sstream>

#include <Poco/Logger.h>
#include <Poco/String.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>

template < typename T > std::string toString(const T& n)
{
    std::stringstream ss;
    ss << n ;
    return ss.str();
}

void GLBrebisCodeGenerator::generateGL(const std::string &inPrefix, const GLBrebisData &result)
{
    Poco::Logger &logger = Poco::Logger::get("GLBrebisCodeGenerator");

    std::string Prefix = inPrefix; std::toupper(Prefix[0]);
    std::string prefix = Poco::toLower(inPrefix);
    std::string PREFIX = Poco::toUpper(inPrefix);

    Poco::DateTime now;
    std::string nowStr = Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::RFC1123_FORMAT);
    std::string year = "2017"; if (now.year() > 2017) { year += "-" + toString(now.year());}

    std::vector<GLBrebisData::Enum> uniqueDefines = result.getAllUniqueDefines();
    std::stringstream defineBlock;
    for(size_t i = 0; i < uniqueDefines.size(); ++i)
    {
        defineBlock << "    #define " << uniqueDefines[i].name;
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

    int Id = 0;

    std::stringstream versionBlock;
    for(size_t i = 0; i < result.registry.features.size(); ++i)
    {
        const GLBrebisData::Feature &feature = result.registry.features[i];
        versionBlock << "    #undef " << feature.name << std:: endl;
        versionBlock << "    #define " << feature.name << " " << ++Id << std:: endl;
    }

    std::stringstream extensionBlock;
    for(size_t i = 0; i < result.registry.extensions.size(); ++i)
    {
        const GLBrebisData::Extension &extension = result.registry.extensions[i];
        extensionBlock << "    #undef " << extension.name << std:: endl;
        extensionBlock << "    #define " << extension.name << " " << ++Id << std:: endl;
    }

    logger.information("Generating %sGL.h ...", inPrefix);

    std::string header;
    GLBrebisUtilities::readFile("template/GL.h.in", header);
    Poco::replaceInPlace(header, "<%=now%>", nowStr.c_str());
    Poco::replaceInPlace(header, "<%=year%>", year.c_str());
    Poco::replaceInPlace(header, "<%=prefix%>", prefix.c_str());
    Poco::replaceInPlace(header, "<%=Prefix%>", Prefix.c_str());
    Poco::replaceInPlace(header, "<%=PREFIX%>", PREFIX.c_str());
    Poco::replaceInPlace(header, "<%=defineBlock%>", defineBlock.str().c_str());
    Poco::replaceInPlace(header, "<%=versionBlock%>", versionBlock.str().c_str());
    Poco::replaceInPlace(header, "<%=extensionBlock%>", extensionBlock.str().c_str());
    GLBrebisUtilities::writeFile(inPrefix + "GL.h", header);

    logger.information("Generating %sGL.c ...", inPrefix);



    logger.information("Done!");

}
