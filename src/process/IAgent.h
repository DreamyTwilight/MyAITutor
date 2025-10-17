
// src/process/IAgent.h

#pragma once
#include <string>

namespace tutor::process {

    // Р’С…РѕРґРЅС‹Рµ РґР°РЅРЅС‹Рµ РґР»СЏ Р»СЋР±РѕРіРѕ Р°РіРµРЅС‚Р°
    struct AgentInput {
        std::string initial_prompt_data; // РСЃС…РѕРґРЅС‹Р№ Р·Р°РїСЂРѕСЃ (e.g., "C++ C++17")
        std::string previous_artifact;   // РџСЂРµРґС‹РґСѓС‰РёР№ СЂРµР·СѓР»СЊС‚Р°С‚ (e.g., JSON РїР»Р°РЅР°)
        std::string critique;            // Р—Р°РјРµС‡Р°РЅРёСЏ РѕС‚ РєСЂРёС‚РёРєР°
    };

    // Р‘Р°Р·РѕРІС‹Р№ РёРЅС‚РµСЂС„РµР№СЃ РґР»СЏ РІСЃРµС… Р°РіРµРЅС‚РѕРІ
    class IAgent {
    public:
        virtual ~IAgent() = default;
        virtual std::string Execute(const AgentInput& input) = 0;
    };

} // namespace tutor::process
