#ifndef GLBREBISUTILITIES_H
#define GLBREBISUTILITIES_H

#include <Poco/URI.h>

class GLBrebisUtilities
{
    GLBrebisUtilities();
public:

    static std::string download(const Poco::URI &uri);
    static void readFile(const std::string &file, std::string &out);
    static void writeFile(const std::string &file, const std::string &in);
};

#endif // GLBREBISUTILITIES_H
