
// src\interfaces\ILLMService.h

#pragma once
#include "../core/DataModels.h"
#include <vector>
#include <memory>

namespace tutor::interfaces {

    using Curriculum = tutor::core::Curriculum;

    class ILLMService {
    public:
        virtual ~ILLMService() = default;
        virtual Curriculum GenerateCurriculum(const std::string& language, const std::string& standard) = 0;
    };
} // tutor::interfaces

