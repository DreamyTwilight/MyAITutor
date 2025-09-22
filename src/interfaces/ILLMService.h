
// src\interfaces\ILLMService.h

#pragma once
#include "../core/DataModels.h"
#include <vector>
#include <memory>

class ILLMService {
public:
    virtual ~ILLMService() = default;
    virtual Curriculum GenerateCurriculum(const std::string& language, const std::string& standard) = 0;
};
