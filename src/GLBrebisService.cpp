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

#include "GLBrebisService.h"

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/LogStream.h>

#include <iostream>
#include <string>

#include "GLBrebisHTTPRequestHandlerFactory.h"


int GLBrebisService::main(const std::vector<std::string>& args)
{
    Poco::LogStream log(Poco::Logger::get("GLBrebisService"));

    Poco::UInt16 port = 3772;
    if (args.size() > 0)
    {
        port = std::stoi(args[0]);
    }

    Poco::Net::ServerSocket socket(port);

    Poco::Net::HTTPServerParams *pParams = new Poco::Net::HTTPServerParams();
    pParams->setMaxQueued(100);
    pParams->setMaxThreads(16);

    Poco::Net::HTTPServer server(new GLBrebisHTTPRequestHandlerFactory(), socket, pParams);

    log.notice() << "Starting GLBrebisService on port " << port << std::endl;
    server.start();

    waitForTerminationRequest();

    log.notice() << "Shutting down server" << std::endl;

    server.stop();

    log.notice() << "Good bye!" << std::endl;

    return EXIT_OK;
}


