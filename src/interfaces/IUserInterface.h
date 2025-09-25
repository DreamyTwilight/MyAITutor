
// src/interfaces/IUserInterface.h

#pragma once
#include <string>
#include <map>
#include <variant>

namespace tutor::interfaces {

    // Определяем типы команд, которые может вернуть UI
    enum class CommandType {
        CreateCurriculum,
        Exit,
        Unknown
    };

    // Структура для представления команды и ее параметров
    struct Command {
        CommandType type = CommandType::Unknown;
        std::map<std::string, std::string> params;
    };

    // Абстрактный интерфейс для любого UI
    class IUserInterface {
    public:
        virtual ~IUserInterface() = default;
        virtual Command getNextCommand() = 0;
    };
} // tutor::interfaces
