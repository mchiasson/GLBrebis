#include "GLBrebisApp.h"

#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionCallback.h>

#include <cctype>
#include <iostream>

#include "GLBrebisParser.h"
#include "GLBrebisCodeGenerator.h"

void GLBrebisApp::initialize(Application& self)
{
    loadConfiguration(); // load default configuration files
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
    Poco::Util::Application::defineOptions(options);

    options.addOption(Poco::Util::Option("help", "h", "Usage Information")
                .required(false)
                .repeatable(false)
                .callback(Poco::Util::OptionCallback<GLBrebisApp>(this, &GLBrebisApp::handleHelp)));

    options.addOption(Poco::Util::Option("prefix", "p", "Project prefix to append to every generated files and symbols")
                .required(true)
                .repeatable(false)
                .argument("name")
                .callback(Poco::Util::OptionCallback<GLBrebisApp>(this, &GLBrebisApp::handlePrefix)));

}

int GLBrebisApp::main(const std::vector<std::string>& args)
{
    GLBrebisParser parser;
    //parser.parse(Poco::URI("https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/master/xml/gl.xml"));
    parser.parse("gl.xml");

    GLBrebisCodeGenerator::generateGL(m_prefix, parser.getResult());

    return EXIT_OK;
}

void GLBrebisApp::handleHelp(const std::string& name, const std::string& value)
{
    Poco::Util::HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.format(std::cout);
    stopOptionsProcessing();
}

void GLBrebisApp::handlePrefix(const std::string& name, const std::string& value)
{
    m_prefix = value;
}
