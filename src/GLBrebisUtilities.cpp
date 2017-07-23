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

#include "GLBrebisUtilities.h"

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>

#include <iostream>
#include <fstream>

std::string GLBrebisUtilities::download(const Poco::URI &uri)
{
    std::string content;
    const Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
    Poco::Net::HTTPResponse response;
    session.sendRequest(request);
    std::istream& is = session.receiveResponse(response);
    if (response.getStatus() == Poco::Net::HTTPServerResponse::HTTP_OK) {
        Poco::StreamCopier::copyToString(is, content);
    }
    return content;
}


void GLBrebisUtilities::readFile(const std::string &file, std::string &out)
{
    std::ifstream in(file.c_str());

    in.seekg(0, std::ios::end);
    out.reserve(in.tellg());
    in.seekg(0, std::ios::beg);

    out.assign((std::istreambuf_iterator<char>(in)),
                std::istreambuf_iterator<char>());
}

void GLBrebisUtilities::writeFile(const std::string &file, const std::string &in)
{
    std::ofstream out(file.c_str(), std::ios::out | std::ios::trunc);
    out << in;
}
