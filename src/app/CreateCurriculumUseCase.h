
// src\app\StageZeroInitializer.h

#pragma once
#include "../interfaces/ILLMService.h"
#include "../interfaces/IDatabaseManager.h"
#include <memory>

namespace tutor::app {

    using IDatabaseManager = tutor::interfaces::IDatabaseManager;
    using ILLMService = tutor::interfaces::ILLMService;

    class CreateCurriculumUseCase {
    public:
        // Конструктор теперь принимает ССЫЛКИ, а не владеющие указатели
        CreateCurriculumUseCase(ILLMService& llmService, IDatabaseManager& dbManager);
        void Execute(const std::string& lang, const std::string& standard);

    private:
        ILLMService& llmService_;
        IDatabaseManager& dbManager_;
    };
} // tutor::app
