
// src/app/VirtualUserInterface.h

#pragma once
#include "../interfaces/IUserInterface.h"

namespace tutor::app {
    
    using tutor::interfaces::IUserInterface;
    using tutor::interfaces::Command;

    // Заглушка, которая эмулирует ввод пользователя
    class VirtualUserInterface : public IUserInterface {
    public:
        Command getNextCommand() override;

    private:
        bool first_call_ = true;
    };
} // tutor::app
