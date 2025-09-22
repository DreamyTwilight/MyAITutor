
// src/core/PromptRegistry.h

#pragma once
#include <string>

class PromptRegistry {
public:
    // Статический метод для генерации промпта учебного плана
    static std::string GenerateCurriculumPrompt(const std::string& language, const std::string& standard);

    // static std::string generateAnswerAnalysisPrompt(...);
};
