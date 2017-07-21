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
    std::vector<GLBrebisData::Command> uniqueFeatureCommand = result.getAllUniqueFeatureCommands();
    for(size_t i = 0; i < uniqueFeatureCommand.size(); ++i)
    {
        const GLBrebisData::Command &command = uniqueFeatureCommand[i];

        funcPtrBlock << "    " << command.proto.ptype << "(" << PREFIX << "_STD_CALL *" << prefix << &command.proto.name[2] << ")(";
        for (size_t j = 0; j < command.params.size(); ++j)
        {
            const GLBrebisData::Param &param = command.params[j];
            if (j > 0) funcPtrBlock << ", ";
            funcPtrBlock << param.ptype << param.name;
        }
        funcPtrBlock << ");" << std::endl;
    }

    std::stringstream extFuncPtrBlock;
    std::vector<GLBrebisData::Command> uniqueExtensionCommand = result.getAllUniqueExtensionCommands();
    for(size_t i = 0; i < uniqueExtensionCommand.size(); ++i)
    {
        const GLBrebisData::Command &command = uniqueExtensionCommand[i];

        extFuncPtrBlock << "    " << command.proto.ptype << "(" << PREFIX << "_STD_CALL *" << prefix << &command.proto.name[2] << ")(";
        for (size_t j = 0; j < command.params.size(); ++j)
        {
            const GLBrebisData::Param &param = command.params[j];
            if (j > 0) extFuncPtrBlock << ", ";
            extFuncPtrBlock << param.ptype << param.name;
        }
        extFuncPtrBlock << ");" << std::endl;
    }

    std::stringstream funcImplBlock;
    for(size_t i = 0; i < result.registry.commands.size(); ++i)
    {
        const GLBrebisData::Command &command = result.registry.commands[i];
        funcImplBlock << "#ifndef " << command.proto.name << std::endl;
        funcImplBlock << PREFIX << "_FORCE_INLINE " << command.proto.ptype << " _" << prefix << &command.proto.name[2] << "(";
        for (size_t j = 0; j < command.params.size(); ++j)
        {
            const GLBrebisData::Param &param = command.params[j];
            if (j > 0) funcImplBlock << ", ";
            funcImplBlock << param.ptype << param.name;
        }
        funcImplBlock << ") { ";
        if (command.proto.name == "glGetProgramPipelineivEXT")
        {
            funcImplBlock << "if (" << prefix << "GLSupport(GL_ES_VERSION_2_0) && (pname == GL_CURRENT_PROGRAM)) { pname = GL_ACTIVE_PROGRAM; } else if (" << prefix << "GLSupport(GL_VERSION_1_0) && (pname == GL_ACTIVE_PROGRAM)) { pname = GL_CURRENT_PROGRAM; } ";
        }
        if (command.proto.ptype.compare(0,6,"void *") == 0 || command.proto.ptype.compare(0,5,"void ") != 0)
        {
            funcImplBlock << "return ";
        }
        funcImplBlock << prefix << "GLAPI." << prefix << &command.proto.name[2] << "(";
        for (size_t j = 0; j < command.params.size(); ++j)
        {
            const GLBrebisData::Param &param = command.params[j];
            if (j > 0) funcImplBlock << ", ";
            funcImplBlock << param.name;
        }
        funcImplBlock << "); }" << std::endl;
        funcImplBlock << "#define " << command.proto.name << " _" << prefix << &command.proto.name[2] << std::endl;
        funcImplBlock << "#endif " << command.proto.name << std::endl;
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
    Poco::replaceInPlace(header, "<%=typeBlock%>", typeBlock.str().c_str());
    Poco::replaceInPlace(header, "<%=extensionCount%>", toString(result.registry.extensions.size()).c_str());
    Poco::replaceInPlace(header, "<%=IdCount%>", toString(IdCount).c_str());
    Poco::replaceInPlace(header, "<%=funcPtrBlock%>", funcPtrBlock.str().c_str());
    Poco::replaceInPlace(header, "<%=extFuncPtrBlock%>", extFuncPtrBlock.str().c_str());
    Poco::replaceInPlace(header, "<%=funcImplBlock%>", funcImplBlock.str().c_str());
    GLBrebisUtilities::writeFile(inPrefix + "GL.h", header);

    logger.information("Generating %sGL.c ...", inPrefix);



    logger.information("Done!");

}
