
// src/infrastructure/GeminiParser.cpp

#include "GeminiParser.h"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <iostream>

using json = nlohmann::json;

namespace GeminiParser {

    Curriculum TransformJsonToCurriculum(const std::string& json_string,
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

    std::string ExtractJsonContent(const std::string& apiResponseText) {
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
            SPDLOG_CRITICAL("JSON access error in API response : ", e.what());
            SPDLOG_CRITICAL("Original response text:\n---\n", apiResponseText ,"\n---");
            std::cerr << "JSON access error in API response: " << e.what() << std::endl;
            std::cerr << "Original response text:\n---\n" << apiResponseText << "\n---" << std::endl;
            throw std::runtime_error("Could not find generated text in Gemini API response.");
        }
    }
}
