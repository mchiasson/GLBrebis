#ifndef GLBREBISDATA_H
#define GLBREBISDATA_H

#include <vector>
#include <string>

struct GLBrebisData {
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
        std::string ptype;
    };

    struct Param
    {
        std::string group;
        std::string ptype;
        std::string name;
    };

    struct Command
    {
        Proto proto;
        std::vector<Param> params;
    };

    struct Commands
    {
        std::string namespace_;
        std::vector<Command> entries;
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
        std::vector<Enums> enums;
        std::vector<Commands> commands;
        std::vector<Feature> features;
        std::vector<Extension> extensions;
    };

    std::vector<Enum> getAllUniqueDefines() const;

    Registry registry;
};

#endif // GLBREBISDATA_H
