
// src\core\DataModels.h

#pragma once
#include <string>
#include <vector>

namespace tutor::core {
    
    struct Topic {
        Topic(std::string name, std::string difficulty) : name(std::move(name)), difficulty(std::move(difficulty)) {}
        std::string name;
        std::string difficulty;
        bool is_archived = false;
    };

    struct Module {
        explicit Module(std::string name) : name(std::move(name)) {}

        std::string name;
        std::vector<Topic> topics;
    };

    struct Curriculum {
        std::string language;
        std::string standard;
        std::vector<Module> modules;
    };
} // tutor::core

