/*******************************************************************************
 * MIT License
 *
 * Copyright (c) 2017-2018 Mathieu-André Chiasson
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

#ifndef GLBREBISAPP_H
#define GLBREBISAPP_H

#include <Poco/Util/Application.h>
#include <Poco/Util/OptionSet.h>


class GLBrebisApp : public Poco::Util::Application
{
public:
    GLBrebisApp();

protected:
    void initialize(Application& self);
    void reinitialize();
    void uninitialize();
    void defineOptions(Poco::Util::OptionSet &options);
    int main(const std::vector<std::string>& args);

    void handleHelp(const std::string& name, const std::string& value);
    void handlePrefix(const std::string& name, const std::string& value);
    void handleInclude(const std::string& name, const std::string& value);
    void handleZip(const std::string& name, const std::string& value);

    std::string m_prefix;
    std::string m_includePath;
    bool m_zip;
};

POCO_APP_MAIN(GLBrebisApp)


#endif // GLBREBISAPP_H
