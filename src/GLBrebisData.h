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

#ifndef GLBREBISDATA_H
#define GLBREBISDATA_H

#include <vector>
#include <string>

struct GLBrebisData {

    struct Type
    {
        std::string api;
        std::string name;
        std::string signature;
    };

    struct Enum
    {
        std::string value;
        std::string name;
        std::string api;
        std::string alias;
        bool operator< (const Enum &other) { return name.compare(other.name) < 0; }
    };

    struct Enums
    {
        std::string namespace_;
        std::string group;
        std::string type;
        std::vector<Enum> entries;
    };

    struct Proto
    {
        std::string group;
        std::string name;
        std::string signature;
    };

    struct Param
    {
        std::string group;
        std::string signature;
        std::string signatureFull;
        std::string name;
    };

    struct Command
    {
        Proto proto;
        std::vector<Param> params;
        bool operator< (const Command &other) { return proto.name.compare(other.proto.name) < 0; }
    };

    struct Feature
    {
        std::string api;
        std::string name;
        std::string number;
        std::vector<std::string> enums;
        std::vector<std::string> commands;
    };

    struct Extension
    {
        std::string name;
        std::string supported;
        std::vector<std::string> enums;
        std::vector<std::string> commands;
    };

    struct Registry
    {
        std::vector<Type> types;
        std::vector<Enums> enums;
        std::vector<Command> commands;
        std::vector<Feature> features;
        std::vector<Extension> extensions;
    };

    std::vector<Enum> getAllUniqueDefines() const;
    std::vector<Type> getAllUniqueTypes() const;
    std::vector<Command> getAllUniqueCommands() const;
    std::vector<Command> getAllUniqueFeatureCommands() const;
    std::vector<Command> getAllUniqueExtensionCommands() const;
    std::vector<std::string> getAllUniqueGLExtensions() const;
    std::vector<std::string> getAllUniqueGLESExtensions() const;
    std::vector<std::string> getAllUniqueFeatures() const;
    std::vector<std::string> getAllUniqueExtensions() const;

    Registry registry;
};

#endif // GLBREBISDATA_H
