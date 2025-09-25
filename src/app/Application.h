
// src/app/Application.h

#pragma once
#include "../interfaces/IUserInterface.h"
#include "../interfaces/ILLMService.h"
#include "../interfaces/IDatabaseManager.h"
#include <memory>

namespace tutor::app {

    using tutor::interfaces::IUserInterface;
    using tutor::interfaces::IDatabaseManager;
    using tutor::interfaces::ILLMService;

    class Application {
    public:
        Application(std::unique_ptr<IUserInterface> ui);
        void run();

    private:
        std::unique_ptr<IUserInterface> ui_;
        std::unique_ptr<IDatabaseManager> dbManager_;
        std::unique_ptr<ILLMService> llmService_;
    };
} // tutor::app
