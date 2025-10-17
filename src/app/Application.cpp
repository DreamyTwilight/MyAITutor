
// src/app/Application.cpp

#include "Application.h"
#include "CreateCurriculumUseCase.h"
#include "../infrastructure/GeminiService.h"
#include "../infrastructure/SQLiteManager.h"
#include "../process/CurriculumGenerationProcess.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace tutor::app {

    using namespace tutor::interfaces;
    using namespace tutor::infrastructure;
    using namespace tutor::process;

    Application::Application(std::unique_ptr<IUserInterface> ui)
        : ui_(std::move(ui))
    {
        // РџСЂРёР»РѕР¶РµРЅРёРµ СЃРѕР·РґР°РµС‚ Рё РІР»Р°РґРµРµС‚ РІСЃРµРјРё РґРѕР»РіРѕР¶РёРІСѓС‰РёРјРё СЃРµСЂРІРёСЃР°РјРё
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
        bool end = true;
        while (end) {
            Command cmd = ui_->GetNextCommand();

            switch (cmd.type) {
            case CommandType::CreateCurriculum: {
                // Application С‚РµРїРµСЂСЊ РѕС‚РІРµС‡Р°РµС‚ Р·Р° СЃР±РѕСЂРєСѓ РІСЃРµРіРѕ РїСЂРѕС†РµСЃСЃР°
                SPDLOG_INFO("CreateCurriculum command received. Initializing process...");
                
                // 1. РЎРѕР·РґР°РµРј РєРѕРЅРєСЂРµС‚РЅС‹Р№ РїСЂРѕС†РµСЃСЃ, РїРµСЂРµРґР°РІР°СЏ РµРјСѓ LLM-СЃРµСЂРІРёСЃ
                tutor::process::CurriculumGenerationProcess process(*llmService_);
                
                // 2. РЎРѕР·РґР°РµРј UseCase, РїРµСЂРµРґР°РІР°СЏ РµРјСѓ РїСЂРѕС†РµСЃСЃ Рё DB-РјРµРЅРµРґР¶РµСЂ
                CreateCurriculumUseCase useCase(process, *dbManager_);
                
                // 3. Р’С‹РїРѕР»РЅСЏРµРј UseCase
                useCase.Execute(cmd.params.at("language"), cmd.params.at("standard"));
                break;
            }
            }
            end = false;
        }
    }
} //tutor::app


