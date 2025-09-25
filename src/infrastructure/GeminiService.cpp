// src/infrastructure/GeminiService.cpp

#include "GeminiService.h"
#include "GeminiParser.h"
#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include "../core/PromptRegistry.h"

using json = nlohmann::json;

namespace tutor::infrastructure {

    using namespace tutor::core;

    // ----- public -----
    GeminiService::GeminiService(const std::string& api_key) : api_key_(api_key) {}

    Curriculum  GeminiService::GenerateCurriculum(const std::string& lang, const std::string& standard) {

        const std::string prompt = PromptRegistry::GenerateCurriculumPrompt(lang, standard);

        std::string curriculum_json_string = PerformRequest(std::string(prompt));

        return tutor::infrastructure::TransformJsonToCurriculum(curriculum_json_string, lang, standard);
    }

    //  ----- private -----

    std::string GeminiService::PerformRequest(const std::string& prompt) {

        const std::string geminiApiUrl = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + api_key_;

        json requestBody = {
        {"contents", {{
            {"parts", {{
                {"text", prompt}
            }}}
        }}}
        };

        cpr::Response r;

#ifdef CACERT_PATH
        // Для Windows используем наш локальный сертификат
        r = cpr::Post(cpr::Url{ geminiApiUrl },
            cpr::Body{ requestBody.dump() },
            cpr::Header{ {"Content-Type", "application/json"} },
            cpr::Ssl(cpr::ssl::CaInfo{ CACERT_PATH }));
#else
        // Для Linux/macOS используем системные сертификаты
        r = cpr::Post(cpr::Url{ geminiApiUrl },
            cpr::Body{ requestBody.dump() },
            cpr::Header{ {"Content-Type", "application/json"} });
#endif

        if (r.status_code != 200) {
            throw std::runtime_error("API Error: " + std::to_string(r.status_code) + " - " + r.text);
        }
        //std::cout << "   -> API response received successfully." << std::endl;

        return tutor::infrastructure::ExtractJsonContent(r.text);
    }
} // tutor::infrastructure
