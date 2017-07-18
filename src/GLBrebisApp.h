#ifndef GLBREBISAPP_H
#define GLBREBISAPP_H

#include <Poco/Util/Application.h>
#include <Poco/Util/OptionSet.h>


class GLBrebisApp : public Poco::Util::Application
{
protected:
    void initialize(Application& self);
    void reinitialize();
    void uninitialize();
    void defineOptions(Poco::Util::OptionSet &options);
    int main(const std::vector<std::string>& args);

    void handleHelp(const std::string& name, const std::string& value);
    void handlePrefix(const std::string& name, const std::string& value);

    std::string m_prefix;
};

POCO_APP_MAIN(GLBrebisApp)


#endif // GLBREBISAPP_H
