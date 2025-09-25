
// src\app\StageZeroInitializer.cpp

#include "CreateCurriculumUseCase.h"
#include <spdlog/spdlog.h>

namespace tutor::app {

    using namespace tutor::interfaces;
    using namespace tutor::core;

    CreateCurriculumUseCase::CreateCurriculumUseCase(ILLMService& llmService, IDatabaseManager& dbManager)
        : llmService_(llmService), dbManager_(dbManager)
    {
    }

    void CreateCurriculumUseCase::Execute(const std::string& lang, const std::string& standard) {
        SPDLOG_INFO("Executing 'Create Curriculum' use case for {} {}", lang, standard);
        if (dbManager_.HasCurriculum(lang, standard)) {
            SPDLOG_WARN("Curriculum for {} {} already exists. Skipping generation.", lang, standard);
            return;
        }

        Curriculum curriculum = llmService_.GenerateCurriculum(lang, standard);
        dbManager_.SaveCurriculum(curriculum);
        SPDLOG_INFO("'Create Curriculum' use case completed successfully.");
    }
} // tutor::app
