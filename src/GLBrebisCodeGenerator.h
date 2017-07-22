#ifndef GLBREBISCODEGENERATOR_H
#define GLBREBISCODEGENERATOR_H

#include "GLBrebisData.h"

class GLBrebisCodeGenerator
{
    GLBrebisCodeGenerator();
public:
    static void generateGL(const std::string &prefix, const std::string &includePrefix, const GLBrebisData &result);
};

#endif // GLBREBISCODEGENERATOR_H
