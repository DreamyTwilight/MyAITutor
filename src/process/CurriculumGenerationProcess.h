
// src/process/CurriculumGenerationProcess.h

#pragma once
#include "IGenerativeProcess.h"
#include "../interfaces/ILLMService.h"
#include <memory>
#include <vector>

namespace tutor::process {

    using tutor::interfaces::ILLMService;

    // РљРѕРЅРєСЂРµС‚РЅР°СЏ СЂРµР°Р»РёР·Р°С†РёСЏ РґР»СЏ РіРµРЅРµСЂР°С†РёРё СѓС‡РµР±РЅРѕРіРѕ РїР»Р°РЅР°
    class CurriculumGenerationProcess : public IGenerativeProcess {
    public:
        explicit CurriculumGenerationProcess(ILLMService& llmService);
        Curriculum Execute(const std::string& language, const std::string& standard) override;

    private:
        ILLMService& llmService_;
        // Р’ Р±СѓРґСѓС‰РµРј Р·РґРµСЃСЊ Р±СѓРґСѓС‚ РєРѕРЅРєСЂРµС‚РЅС‹Рµ Р°РіРµРЅС‚С‹
    };

} // namespace tutor::process
