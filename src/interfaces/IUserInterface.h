
// src/interfaces/IUserInterface.h

#pragma once
#include <string>
#include <map>
#include <variant>

namespace tutor::interfaces {

    // РћРїСЂРµРґРµР»СЏРµРј С‚РёРїС‹ РєРѕРјР°РЅРґ, РєРѕС‚РѕСЂС‹Рµ РјРѕР¶РµС‚ РІРµСЂРЅСѓС‚СЊ UI
    enum class CommandType {
        CreateCurriculum,
        Exit,
        Unknown
    };

    // РЎС‚СЂСѓРєС‚СѓСЂР° РґР»СЏ РїСЂРµРґСЃС‚Р°РІР»РµРЅРёСЏ РєРѕРјР°РЅРґС‹ Рё РµРµ РїР°СЂР°РјРµС‚СЂРѕРІ
    struct Command {
        CommandType type = CommandType::Unknown;
        std::map<std::string, std::string> params;
    };

    // РђР±СЃС‚СЂР°РєС‚РЅС‹Р№ РёРЅС‚РµСЂС„РµР№СЃ РґР»СЏ Р»СЋР±РѕРіРѕ UI
    class IUserInterface {
    public:
        virtual ~IUserInterface() = default;
        virtual Command GetNextCommand() = 0;
    };
} // tutor::interfaces

