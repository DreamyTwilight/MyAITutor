
// src\infrastructure\SQLiteManager.h

#pragma once
#include "../interfaces/IDatabaseManager.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>

namespace tutor::infrastructure {

    using tutor::interfaces::IDatabaseManager;
    using tutor::core::Curriculum;

    class SQLiteManager : public IDatabaseManager {
    public:
        explicit SQLiteManager(const std::string& db_filename);
        void SaveCurriculum(const Curriculum& curriculum) override;
        bool HasCurriculum(const std::string& language, const std::string& standard) override;

    private:
        std::string db_filename_;
        std::unique_ptr<SQLite::Database> db_uptr_;

        void EnsureSchemaIsActual();
        bool IsAlreadyPopulated();
        long long GetOrCreateId(const std::string& sql_insert, const std::string& sql_select, const std::vector<std::string>& params);
    };
} // tutor::infrastructure


