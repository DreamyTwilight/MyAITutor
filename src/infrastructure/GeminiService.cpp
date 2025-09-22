// src/infrastructure/GeminiService.cpp

#include "GeminiService.h"
#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include "../core/PromptRegistry.h"

using json = nlohmann::json;

// ----- public -----
GeminiService::GeminiService(const std::string& api_key): api_key_(api_key){}

Curriculum  GeminiService::GenerateCurriculum(const std::string& lang, const std::string& standard) {

    const std::string prompt = PromptRegistry::GenerateCurriculumPrompt(lang, standard);

    std::string curriculum_json_string = PerformRequest(std::string(prompt));

    return TransformJsonToCurriculum(curriculum_json_string, lang, standard);
}

//  ----- private -----

Curriculum GeminiService::TransformJsonToCurriculum(const std::string& json_string,
                                                    const std::string& language,
                                                    const std::string& standard) {
    if (json_string.empty()) {
        throw std::runtime_error("Cannot transform an empty JSON string.");
    }

    Curriculum curriculum;
    curriculum.language = language;
    curriculum.standard = standard;

    json parsed_json = json::parse(json_string);

    for (const auto& module_json : parsed_json) {
        curriculum.modules.emplace_back(module_json.at("module_name").get<std::string>());

        for (const auto& topic_json : module_json.at("topics")) {
            curriculum.modules.back().topics.emplace_back(
                topic_json.at("topic_name").get<std::string>(),
                topic_json.at("difficulty").get<std::string>()
            );
        }
    }

    return curriculum;
}

// Вспомогательная функция для извлечения "чистого" JSON из ответа Gemini.
// API оборачивает свой результат в дополнительную структуру.
std::string GeminiService::ExtractJsonContent(const std::string& apiResponseText) {
    try {
        json responseJson = json::parse(apiResponseText);
        const auto& textNode = responseJson.at("candidates").at(0).at("content").at("parts").at(0).at("text");

        if (!textNode.is_string()) {
            throw std::runtime_error("The 'text' field in the API response is not a string.");
        }

        std::string generatedText = textNode.get<std::string>();
        // Убираем маркеры ```json, которые добавляет модель
        size_t start = generatedText.find_first_of("[{");
        size_t end = generatedText.find_last_of("}]");

        if (start != std::string::npos && end != std::string::npos) {
            return generatedText.substr(start, end - start + 1);
        }
        return generatedText;
    }
    catch (const json::exception& e) { // Ловим ошибки парсинга/доступа
        std::cerr << "JSON access error in API response: " << e.what() << std::endl;
        std::cerr << "Original response text:\n---\n" << apiResponseText << "\n---" << std::endl;
        throw std::runtime_error("Could not find generated text in Gemini API response.");
    }
}


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
    
    return ExtractJsonContent(r.text);
}
