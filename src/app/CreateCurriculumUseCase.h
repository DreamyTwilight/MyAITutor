
// src\app\StageZeroInitializer.h

#pragma once
#include "../process/IGenerativeProcess.h"
#include "../interfaces/IDatabaseManager.h"
#include <memory>

namespace tutor::app {

    using IDatabaseManager = tutor::interfaces::IDatabaseManager;
    using IGenerativeProcess = tutor::process::IGenerativeProcess;

    class CreateCurriculumUseCase {
    public:
        CreateCurriculumUseCase(IGenerativeProcess& process, IDatabaseManager& dbManager);
        void Execute(const std::string& lang, const std::string& standard);
    private:
        IGenerativeProcess& process_;
        IDatabaseManager& dbManager_;
    };

} // tutor::app

