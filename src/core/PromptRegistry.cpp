
// src/core/PromptRegistry.cpp

#include "PromptRegistry.h"
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <fmt/core.h> // Включаем {fmt}

namespace tutor::core {

    // ---- ШАБЛОНЫ ----
    namespace {
        // Используем именованные плейсхолдеры {language} и {standard}
        constexpr char CURRICULUM_GENERATION_TEMPLATE[] = R"(Твоя роль: Ты — опытный методист и ведущий преподаватель {language}, создающий учебный план для студента с уровнем "Джун", который знает основы синтаксиса, но хочет получить системные знания по {standard}.
Правила: Разработай структурированный план, разбитый на модули и атомарные темы. Темы должны быть выстроены в логическом порядке. 
Для каждой темы укажи уровень сложности ("базовый", "средний", "продвинутый"). 
Твой вывод должен быть в формате строгого JSON: массив модулей, где каждый модуль содержит ключ "module_name" и массив объектов-тем "topics", 
где каждый объект-тема содержит ключи "topic_name" и "difficulty".
Задача: Создай учебный план по {language} {standard}.)";
    }


    std::string PromptRegistry::GenerateCurriculumPrompt(const std::string& language, const std::string& standard) {
        return fmt::format(CURRICULUM_GENERATION_TEMPLATE,
            fmt::arg("language", language),
            fmt::arg("standard", standard));
    }
}
