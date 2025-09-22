
// src/app/Application.cpp

#include "Application.h"
#include "CreateCurriculumUseCase.h"
#include "../infrastructure/GeminiService.h"
#include "../infrastructure/SQLiteManager.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

Application::Application(std::unique_ptr<IUserInterface> ui) 
    : ui_(std::move(ui))
{
    // Приложение создает и владеет всеми долгоживущими сервисами
    const char* apiKeyEnv = std::getenv("GEMINI_API_KEY");
    if (!apiKeyEnv) {
        throw std::runtime_error("FATAL: GEMINI_API_KEY environment variable is not set.");
    }
    const std::string dbFilename = "tutor_db.sqlite";

    llmService_ = std::make_unique<GeminiService>(apiKeyEnv);
    dbManager_ = std::make_unique<SQLiteManager>(dbFilename);
    
    SPDLOG_INFO("Application initialized with all services.");
}

void Application::run() {
    while (true) {
        Command cmd = ui_->getNextCommand();
        
        switch (cmd.type) {
            case CommandType::CreateCurriculum: {
                // Создаем UseCase по требованию и выполняем его
                CreateCurriculumUseCase useCase(*llmService_, *dbManager_);
                useCase.Execute(cmd.params.at("language"), cmd.params.at("standard"));
                break;
            }
            
            case CommandType::Exit: {
                SPDLOG_INFO("Exit command received. Shutting down.");
                return;
            }

            case CommandType::Unknown:
            default: {
                SPDLOG_WARN("Unknown command received.");
                break;
            }
        }
    }
}
