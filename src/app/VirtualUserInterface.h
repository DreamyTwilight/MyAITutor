
// src/app/VirtualUserInterface.h

#pragma once
#include "../interfaces/IUserInterface.h"

// Заглушка, которая эмулирует ввод пользователя
class VirtualUserInterface : public IUserInterface {
public:
    Command getNextCommand() override;

private:
    bool first_call_ = true;
};