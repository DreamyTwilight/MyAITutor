
// src/app/VirtualUserInterface.cpp

#include "VirtualUserInterface.h"

namespace tutor::app {
    
    using namespace tutor::interfaces;

    Command VirtualUserInterface::getNextCommand() {
        if (first_call_) {
            first_call_ = false;
            // Эмулируем ввод команды на создание учебного плана
            return Command{
                CommandType::CreateCurriculum,
                {
                    {"language", "C++"},
                    {"standard", "C++17"}
                }
            };
        }
        // После первой команды эмулируем команду на выход
        return Command{ CommandType::Exit };
    }
} // tutor::app
