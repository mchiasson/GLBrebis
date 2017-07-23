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

#include "GLBrebisHTTPDefaultRequestHandler.h"

#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Path.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Zip/Compress.h>
#include <Poco/StreamCopier.h>

#include <sstream>

#include "GLBrebisParser.h"
#include "GLBrebisCodeGenerator.h"
#include "xml/gl.xml.h"
#include "KHR/khrplatform.h.h"

void GLBrebisHTTPDefaultRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
    Poco::Net::HTMLForm form(request);

    if (!form.has("prefix"))
    {
        sendError(response, 400, "Missing mandatory '<kbd>prefix</kbd>' parameter.");
        return;
    }
    std::string prefix = form.get("prefix");

    if (prefix.length() == 0)
    {
        sendError(response, 400, "'Mandatory <kbd>prefix</kbd>' parameter cannot be blank.");
        return;
    }

    std::string include = form.get("include", "");
    if (include.length() > 0)
    {
        size_t lastCharPos = include.length()-1;
        if (include[lastCharPos] == '\\')
        {
            include[lastCharPos] = '/';
        }
        else if(include[lastCharPos] != '/')
        {
            include += '/';
        }
    }

    std::stringstream headerStream, sourceSteram, khrheaderStream, zipStream;

    GLBrebisParser parser;
    parser.parse(gl_xml, sizeof(gl_xml));

    GLBrebisCodeGenerator::generateGL(prefix, include, true, parser.getResult(), sourceSteram, headerStream);

    khrheaderStream << std::string((char*)khrplatform_h, sizeof(khrplatform_h));

    Poco::DateTime now;
    std::string nowStr = Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::RFC1123_FORMAT);
    std::string year = "2017"; if (now.year() > 2017) { year += "-" + std::to_string(now.year());}

    Poco::Zip::Compress c(zipStream, true);

    c.addFile(headerStream,    now, include + prefix + "GL.h");
    c.addFile(sourceSteram,    now, prefix + "GL.c");
    c.addFile(khrheaderStream, now, "KHR/khrplatform.h");

    std::stringstream comment;
    comment << "This archive was generated by GLBrebis on " << nowStr << std::endl;
    comment << "https://github.com/mchiasson/GLBrebis" << std::endl;
    comment << "Copyright (c) " << year << " Mathieu-André Chiasson" << std::endl;
    comment << "All rights reserved." << std::endl;
    c.setZipComment(comment.str());

    c.close();

#if !defined(NDEBUG)
    response.set("Cache-Control", "no-cache, no-store, must-revalidate");
#endif
    response.setContentType("application/zip");

    Poco::StreamCopier::copyStream(zipStream, response.send());
}


void GLBrebisHTTPDefaultRequestHandler::sendError(Poco::Net::HTTPServerResponse& response, int status, const std::string &reason)
{
    response.setStatusAndReason((Poco::Net::HTTPServerResponse::HTTPStatus)status, reason);
    response.setContentType("text/html");

    std::string title = std::to_string(status) + " " + Poco::Net::HTTPServerResponse::getReasonForStatus(response.getStatus());

    Poco::DateTime now;
    std::string year = "2017"; if (now.year() > 2017) { year += "-" + std::to_string(now.year());}


    std::ostream& os = response.send();
    os << "<html>";
    os << "<head><title>" << title << "</title></head>";
    os << "<body>";
    os << "<h1>" << title << "</h1>";
    os << "<p>" << reason << "</p>";
    os << "<p>A simple GL extension wrangler drop-in generator for OpenGL and OpenGL ES apps.</p>";
    os << "<p>This program will generate a set of highly portable C99 compliant OpenGL and OpenGL ES function wrangler code with any prefix customization that you want. Just generate your wrangler code, drop the two files into your project and start coding! It's as simple as that.</p>";
    os << "<p>Usage Example: <a href=\"/?prefix=Mut&include=mut\">/?prefix=Mut&include=mut" << "</a></p>";
    os << "<p>'<kbd>prefix</kbd>' Mandatory : Project prefix to append to every generated files and symbols.</p>";
    os << "<p>'<kbd>include</kbd>' Optional: Include path prefix to use in the generated GL.c to where it includes the generated GL.h. </p>";
    os << "</body>";
    os << "<footer>";
    os << "<hr>";
    os << "<p>Copyright (c) " << year << " Mathieu-Andre Chiasson</p>";
    os << "<p>All rights reserved.</p>";
    os << "<p><a href=\"https://github.com/mchiasson/GLBrebis\">https://github.com/mchiasson/GLBrebis</a></p>";
    os << "</footer>";
    os << "</html>";

}
