
// src\interfaces\IDatabaseManager.h

#pragma once
#include "../core/DataModels.h"
#include <vector>
#include <memory>

class IDatabaseManager {
public:
    virtual ~IDatabaseManager() = default;
    // Метод теперь принимает Curriculum
    virtual void SaveCurriculum(const Curriculum& curriculum) = 0;
    // Новый метод для проверки
    virtual bool HasCurriculum(const std::string& language, const std::string& standard) = 0;
};
