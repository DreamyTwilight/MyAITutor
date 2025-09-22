
// src\infrastructure\SQLiteManager.cpp

#include "SQLiteManager.h"
#include <spdlog/spdlog.h>

SQLiteManager::SQLiteManager(const std::string& db_filename)
    : db_filename_(db_filename),
    db_uptr_(std::make_unique<SQLite::Database>(db_filename_, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE))
{
    SPDLOG_INFO("Database connection established: {}", db_filename_);
    EnsureSchemaIsActual();
}


void SQLiteManager::SaveCurriculum(const Curriculum& curriculum) {
    // 1. Получаем или создаем ID языка
    long long langId = GetOrCreateId(
        "INSERT INTO Languages (name) VALUES (?)",
        "SELECT id FROM Languages WHERE name = ?",
        { curriculum.language }
    );

    // 2. Получаем или создаем ID стандарта, используя ID языка
    long long standardId = GetOrCreateId(
        "INSERT INTO Standards (name, language_id) VALUES (?, ?)",
        "SELECT id FROM Standards WHERE name = ? AND language_id = ?",
        { curriculum.standard, std::to_string(langId) }
    );

    SQLite::Transaction transaction(*db_uptr_);

    // Подготовленные запросы для модулей и тем
    SQLite::Statement moduleQuery(*db_uptr_, "INSERT INTO Modules (name, standard_id) VALUES (?, ?)");
    SQLite::Statement topicQuery(*db_uptr_, "INSERT INTO Topics (name, difficulty, module_id) VALUES (?, ?, ?)");

    for (const auto& module : curriculum.modules) {
        // 3. Получаем или создаем ID модуля
        long long moduleId = GetOrCreateId(
            "INSERT INTO Modules (name, standard_id) VALUES (?, ?)",
            "SELECT id FROM Modules WHERE name = ? AND standard_id = ?",
            { module.name, std::to_string(standardId) }
        );

        for (const auto& topic : module.topics) {
            // 4. Вставляем тему с ID модуля (используем INSERT OR IGNORE для идемпотентности)
            topicQuery.bind(1, topic.name);
            topicQuery.bind(2, topic.difficulty);
            topicQuery.bind(3, moduleId);
            topicQuery.exec();
            topicQuery.reset();
        }
    }

    transaction.commit();
    SPDLOG_INFO("Curriculum for {} {} saved successfully.", curriculum.language, curriculum.standard);
}

bool SQLiteManager::HasCurriculum(const std::string& language, const std::string& standard) {
    // Этот запрос проверит, есть ли хоть одна тема для данного языка/стандарта
    SQLite::Statement query(*db_uptr_,
        "SELECT 1 FROM Topics "
        "JOIN Modules ON Topics.module_id = Modules.id "
        "JOIN Standards ON Modules.standard_id = Standards.id "
        "JOIN Languages ON Standards.language_id = Languages.id "
        "WHERE Languages.name = ? AND Standards.name = ? LIMIT 1");
    query.bind(1, language);
    query.bind(2, standard);
    return query.executeStep();
}

// --- Приватные методы ---

// Этот метод отвечает ТОЛЬКО за схему
void SQLiteManager::EnsureSchemaIsActual() {
    
    // Включаем поддержку внешних ключей
    db_uptr_->exec("PRAGMA foreign_keys = ON;");

    // Таблица-справочник для языков
    db_uptr_->exec("CREATE TABLE IF NOT EXISTS Languages ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL UNIQUE)");

    // Таблица-справочник для стандартов (версий)
    db_uptr_->exec("CREATE TABLE IF NOT EXISTS Standards ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "language_id INTEGER NOT NULL, "
        "FOREIGN KEY(language_id) REFERENCES Languages(id), "
        "UNIQUE(name, language_id))");

    // Таблица для модулей, привязанная к стандарту
    db_uptr_->exec("CREATE TABLE IF NOT EXISTS Modules ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "standard_id INTEGER NOT NULL, "
        "FOREIGN KEY(standard_id) REFERENCES Standards(id), "
        "UNIQUE(name, standard_id))");

    // Основная таблица тем, привязанная к модулю
    db_uptr_->exec("CREATE TABLE IF NOT EXISTS Topics ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "module_id INTEGER NOT NULL, "
        "name TEXT NOT NULL, "
        "difficulty TEXT NOT NULL, "
        "is_archived INTEGER NOT NULL DEFAULT 0, "
        "FOREIGN KEY(module_id) REFERENCES Modules(id), "
        "UNIQUE(module_id, name))");

    SPDLOG_INFO("Database schema is up to date.");
}

// Этот метод отвечает ТОЛЬКО за проверку
bool SQLiteManager::IsAlreadyPopulated() {
    SQLite::Statement query(*db_uptr_, "SELECT COUNT(*) FROM Topics");
    query.executeStep();
    return (query.getColumn(0).getInt() > 0);
}

long long SQLiteManager::GetOrCreateId(const std::string& sql_insert, const std::string& sql_select, const std::vector<std::string>& params) {
    SQLite::Statement select_query(*db_uptr_, sql_select);
    for (size_t i = 0; i < params.size(); ++i) select_query.bind(i + 1, params[i]);

    if (select_query.executeStep()) {
        return select_query.getColumn(0).getInt64();
    }
    else {
        SQLite::Statement insert_query(*db_uptr_, sql_insert);
        for (size_t i = 0; i < params.size(); ++i) insert_query.bind(i + 1, params[i]);
        insert_query.exec();
        return db_uptr_->getLastInsertRowid();
    }
}

