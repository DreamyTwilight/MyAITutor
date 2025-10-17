
// src/app/VirtualUserInterface.cpp

#include "VirtualUserInterface.h"

namespace tutor::app {
    
    using namespace tutor::interfaces;

    Command VirtualUserInterface::GetNextCommand() {
        if (first_call_) {
            first_call_ = false;
            // Р­РјСѓР»РёСЂСѓРµРј РІРІРѕРґ РєРѕРјР°РЅРґС‹ РЅР° СЃРѕР·РґР°РЅРёРµ СѓС‡РµР±РЅРѕРіРѕ РїР»Р°РЅР°
            return Command{
                CommandType::CreateCurriculum,
                {
                    {"language", "C++"},
                    {"standard", "C++17"}
                }
            };
        }
        // РџРѕСЃР»Рµ РїРµСЂРІРѕР№ РєРѕРјР°РЅРґС‹ СЌРјСѓР»РёСЂСѓРµРј РєРѕРјР°РЅРґСѓ РЅР° РІС‹С…РѕРґ
        return Command{ CommandType::Exit };
    }
} // tutor::app

