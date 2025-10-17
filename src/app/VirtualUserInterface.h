
// src/app/VirtualUserInterface.h

#pragma once
#include "../interfaces/IUserInterface.h"

namespace tutor::app {
    
    using tutor::interfaces::IUserInterface;
    using tutor::interfaces::Command;

    // Р—Р°РіР»СѓС€РєР°, РєРѕС‚РѕСЂР°СЏ СЌРјСѓР»РёСЂСѓРµС‚ РІРІРѕРґ РїРѕР»СЊР·РѕРІР°С‚РµР»СЏ
    class VirtualUserInterface : public IUserInterface {
    public:
        Command GetNextCommand() override;

    private:
        bool first_call_ = true;
    };
} // tutor::app

