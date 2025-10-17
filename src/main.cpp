
// src/main.cpp

#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>

#include "app/Application.h"
#include "app/VirtualUserInterface.h"

using tutor::app::VirtualUserInterface;
using tutor::app::Application;

int main() {

        // --- БЛОК НАСТРОЙКИ ФОРМАТА ЛОГИРОВАНИЯ ---
    // %^...%$ - начало и конец цветного вывода
    // %l - уровень лога (info, warn, critical)
    // %@ - источник в формате file:line
    // %v - само сообщение
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
    // меняем:
        //spdlog::info(...)->SPDLOG_INFO(...)
        //spdlog::warn(...)->SPDLOG_WARN(...)
        //spdlog::critical(...)->SPDLOG_CRITICAL(...)
        //spdlog::debug(...)->SPDlog_DEBUG(...)
    // --- КОНЕЦ БЛОКА ---

    spdlog::set_level(spdlog::level::debug);
    SPDLOG_INFO("AI-Tutor application starting up...");

    try {
        // Создаем конкретную реализацию UI
        auto ui = std::make_unique<VirtualUserInterface>();
        // Создаем и запускаем приложение, передавая ему UI
        Application app(std::move(ui));
        app.run();

    }
    catch (const std::exception& e) {
        SPDLOG_CRITICAL("A critical error occurred: {}", e.what());
        // Вывод в cerr полезен, если логгер не успел инициализироваться или упал
        std::cerr << "\n[CRITICAL ERROR] " << e.what() << std::endl;
        return 1;
    }

    SPDLOG_INFO("Application finished successfully.");
    spdlog::shutdown();
    return 0;
}

