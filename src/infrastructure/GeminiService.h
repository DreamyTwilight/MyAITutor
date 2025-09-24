// src/infrastructure/GeminiService.h

#pragma once
#include "../interfaces/ILLMService.h"

class GeminiService : public ILLMService {
public:
    explicit GeminiService(const std::string& api_key);
    Curriculum GenerateCurriculum(const std::string& lang, const std::string& standard) override;

private:
    std::string api_key_;

    std::string PerformRequest(const std::string& prompt);
};
