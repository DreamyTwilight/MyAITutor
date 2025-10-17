
// src/process/IGenerativeProcess.h

#pragma once
#include "../core/DataModels.h"
#include <string>

namespace tutor::process {
    
    using tutor::core::Curriculum;

    class IGenerativeProcess {
    public:
        virtual ~IGenerativeProcess() = default;
        virtual Curriculum Execute(const std::string& language, const std::string& standard) = 0;
    };

} // namespace tutor::process
