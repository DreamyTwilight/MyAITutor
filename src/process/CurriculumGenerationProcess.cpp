
// src/process/CurriculumGenerationProcess.cpp

#include "CurriculumGenerationProcess.h"
#include "../core/PromptRegistry.h"
#include "../infrastructure/GeminiParser.h"
#include <spdlog/spdlog.h>

namespace tutor::process {

    using namespace tutor::core;
    using namespace tutor::infrastructure;

    CurriculumGenerationProcess::CurriculumGenerationProcess(ILLMService& llmService)
        : llmService_(llmService) {}

    Curriculum CurriculumGenerationProcess::Execute(const std::string& language, const std::string& standard) {
        SPDLOG_INFO("Starting Curriculum Generation Process...");
        
        // --- РЁРђР“ 1: Р“Р•РќР•Р РђРўРћР  ---
        SPDLOG_DEBUG("Step 1: Generating initial draft...");
        const std::string initial_prompt = PromptRegistry::GenerateCurriculumPrompt(language, standard);
        std::string current_plan_json = llmService_.PerformRequest(initial_prompt); // РџСЂРµРґРїРѕР»Р°РіР°РµРј, С‡С‚Рѕ PerformRequest СЃС‚Р°РЅРµС‚ public
        
        // --- РЁРђР“ 2: Р¦РРљР› РЈРўРћР§РќР•РќРРЇ (РїРѕРєР° С‚РѕР»СЊРєРѕ 1 РёС‚РµСЂР°С†РёСЏ РґР»СЏ РїСЂРѕС‚РѕС‚РёРїР°) ---
        // TODO: Р РµР°Р»РёР·РѕРІР°С‚СЊ РїРѕР»РЅРѕС†РµРЅРЅС‹Р№ С†РёРєР» СЃ РљСЂРёС‚РёРєРѕРј Рё Р РµРґР°РєС‚РѕСЂРѕРј
        
        SPDLOG_INFO("Generative process finished. Transforming final artifact.");
        
        // --- РЁРђР“ 3: Р¤РРќРђР›Р¬РќРћР• РџР Р•РћР‘Р РђР—РћР’РђРќРР• ---
        return TransformJsonToCurriculum(current_plan_json, language, standard);
    }

} // namespace tutor::process
