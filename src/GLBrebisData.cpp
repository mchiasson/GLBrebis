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

#include "GLBrebisData.h"

#include "Poco/Logger.h"
#include <algorithm>

std::vector<GLBrebisData::Enum> GLBrebisData::getAllUniqueDefines() const
{
    Poco::Logger &logger = Poco::Logger::get("GLBrebisData");
    std::vector<Enum> results;

    for(size_t i = 0; i < registry.enums.size(); ++i)
    {
        const Enums &parent = registry.enums[i];
        for(size_t j = 0; j < parent.entries.size(); ++j)
        {
            const Enum &candidate = registry.enums[i].entries[j];

            size_t k;
            for (k = 0; k < results.size(); ++k)
            {
                if (candidate.name == results[k].name)
                {
                    if (results[k].alias.length() > 0)
                    {
                        results[k] = candidate; // override if the one we had has an alias.
                    }
                    else if (candidate.alias.length() > 0)
                    {

                    }
                    else if (candidate.value != results[k].value)
                    {
                        logger.error("ALERT! Found duplicated definitions '%s' with different values: %s vs %s", candidate.name, candidate.value, results[k].value);
                    }
                    else
                    {
                        logger.warning("WARNING! Found duplicated definitions '%s' ", candidate.name);
                    }
                    break;
                }
            }

            if (k == results.size())
            {
                results.push_back(candidate);
            }
        }
    }

    std::sort(results.begin(), results.end());

    return results;
}

std::vector<GLBrebisData::Type> GLBrebisData::getAllUniqueTypes() const
{
    std::vector<Type> results;

    for(size_t i = 0; i < registry.types.size(); ++i) {
        const Type &candidate = registry.types[i];
        size_t k;
        for (k = 0; k < results.size(); ++k) {
            if (candidate.name == results[k].name) {
                results[k] = candidate; // override
                break;
            }
        }

        if (k == results.size()) {
            results.push_back(candidate);
        }
    }

    return results;
}

std::vector<GLBrebisData::Command> GLBrebisData::getAllUniqueCommands() const
{
    std::vector<Command> results;

    for(size_t i = 0; i < registry.commands.size(); ++i)
    {
        const Command &candidate = registry.commands[i];
        size_t k;
        for (k = 0; k < results.size(); ++k) {
            if (candidate.proto.name == results[k].proto.name) {
                break;
            }
        }

        if (k == results.size()) {
            results.push_back(candidate);
        }
    }

    std::sort(results.begin(), results.end());

    return results;

}

std::vector<GLBrebisData::Command> GLBrebisData::getAllUniqueFeatureCommands() const
{
    std::vector<Command> results;

    for(size_t i = 0; i < registry.features.size(); ++i)
    {
        const GLBrebisData::Feature &feature = registry.features[i];
        for(size_t j = 0; j < feature.commands.size(); ++j)
        {
            const std::string &commandName = feature.commands[j];

            for(size_t k = 0; k < registry.commands.size(); ++k)
            {
                const GLBrebisData::Command &commandB = registry.commands[k];
                if (commandName == commandB.proto.name)
                {
                    results.push_back(commandB);
                    break;
                }
            }
        }
    }

    std::sort(results.begin(), results.end());

    return results;
}

std::vector<GLBrebisData::Command> GLBrebisData::getAllUniqueExtensionCommands() const
{
    std::vector<Command> results;

    for(size_t i = 0; i < registry.extensions.size(); ++i)
    {
        const GLBrebisData::Extension &extension = registry.extensions[i];
        for(size_t j = 0; j < extension.commands.size(); ++j)
        {
            const std::string &commandName = extension.commands[j];

            for(size_t k = 0; k < registry.commands.size(); ++k)
            {
                const GLBrebisData::Command &commandB = registry.commands[k];
                if (commandName == commandB.proto.name)
                {
                    results.push_back(commandB);
                    break;
                }
            }
        }
    }

    std::sort(results.begin(), results.end());

    return results;
}

std::vector<std::string> GLBrebisData::getAllUniqueGLExtensions() const
{
    std::vector<std::string> results;

    for(size_t i = 0; i < registry.extensions.size(); ++i)
    {
        const GLBrebisData::Extension &candidate = registry.extensions[i];
        if (candidate.supported == "gl" ||
            candidate.supported.compare(0, 3, "gl|") == 0)
        {
            size_t k;
            for (k = 0; k < results.size(); ++k) {
                if (candidate.name == results[k]) {
                    break;
                }
            }

            if (k == results.size()) {
                results.push_back(candidate.name);
            }
        }
    }

    std::sort(results.begin(), results.end());

    return results;

}


std::vector<std::string> GLBrebisData::getAllUniqueGLESExtensions() const
{
    std::vector<std::string> results;

    for(size_t i = 0; i < registry.extensions.size(); ++i)
    {
        const GLBrebisData::Extension &candidate = registry.extensions[i];
        if (candidate.supported.compare(0, 7, "gl|gles") == 0 ||
            candidate.supported.compare(0, 4, "gles") == 0)
        {
            size_t k;
            for (k = 0; k < results.size(); ++k) {
                if (candidate.name == results[k]) {
                    break;
                }
            }

            if (k == results.size()) {
                results.push_back(candidate.name);
            }
        }
    }

    std::sort(results.begin(), results.end());

    return results;
}

std::vector<std::string> GLBrebisData::getAllUniqueFeatures() const
{
    std::vector<std::string> results;

    for(size_t i = 0; i < registry.features.size(); ++i)
    {
        const GLBrebisData::Feature &candidate = registry.features[i];
        size_t k;
        for (k = 0; k < results.size(); ++k) {
            if (candidate.name == results[k]) {
                break;
            }
        }

        if (k == results.size()) {
            results.push_back(candidate.name);
        }
    }

    std::sort(results.begin(), results.end());

    return results;
}

std::vector<std::string> GLBrebisData::getAllUniqueExtensions() const
{
    std::vector<std::string> results;

    for(size_t i = 0; i < registry.extensions.size(); ++i)
    {
        const GLBrebisData::Extension &candidate = registry.extensions[i];
        size_t k;
        for (k = 0; k < results.size(); ++k) {
            if (candidate.name == results[k]) {
                break;
            }
        }

        if (k == results.size()) {
            results.push_back(candidate.name);
        }
    }

    std::sort(results.begin(), results.end());

    return results;
}

