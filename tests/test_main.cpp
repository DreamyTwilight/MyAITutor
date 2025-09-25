#include <gtest/gtest.h>

#include <SQLiteCpp/SQLiteCpp.h>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <curl/curl.h>

#include "../src/interfaces/IDatabaseManager.h"
#include "../src/infrastructure/SQLiteManager.h"
#include "../src/core/DataModels.h"             

#include "../src/infrastructure/GeminiService.h"
#include "../src/infrastructure/GeminiParser.h"

using namespace tutor::core;
using namespace tutor::infrastructure;
using namespace tutor::interfaces;


TEST(SampleTest, BasicAssertions) {
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
}

// -------- ОБЩИЕ ТЕСТЫ ----------

TEST(DependencyLinkage, SQLiteCpp) {
    // Проверяем, что создание объекта БД не бросает исключение
    ASSERT_NO_THROW({
        SQLite::Database db(":memory:", SQLite::OPEN_READWRITE);
        db.exec("CREATE TABLE test (id INTEGER)");
        });
}

TEST(DependencyLinkage, NlohmannJson) {
    // Проверяем, что парсинг работает и значение корректно
    auto j = nlohmann::json::parse(R"({"status":"ok"})");
    ASSERT_EQ(j.at("status"), "ok");
}

TEST(DependencyLinkage, CprHttpsRequest) {
    cpr::Response r;

#ifdef CACERT_PATH
    // Этот блок скомпилируется ТОЛЬКО для Windows, т.к. CMake определил CACERT_PATH
    r = cpr::Get(cpr::Url{ "https://www.example.com" },
        cpr::Ssl(cpr::ssl::CaInfo{ CACERT_PATH }));
    std::cout << "windows" << std::endl;
#else
    r = cpr::Get(cpr::Url{ "https://www.example.com" });
    std::cout << "other" << std::endl;

#endif

    // Проверяем, что запрос прошел успешно (статус 200)
    ASSERT_EQ(r.status_code, 200);
    // Проверяем, что тело ответа не пустое
    ASSERT_FALSE(r.text.empty());
}

// -------- ТЕСТЫ ДЛЯ SQLITEMANAGER ----------

// Test Fixture для SQLiteManager
class SQLiteManagerTest : public ::testing::Test {
protected:
    const std::string test_db_filename_ = ":memory:";

    std::unique_ptr<SQLiteManager> manager_;

    void SetUp() override {
        manager_ = std::make_unique<SQLiteManager>(test_db_filename_);
    }
};

// Тест 1: Проверяем, что конструктор не падает.
// Проверить наличие таблиц можно только косвенно.
TEST_F(SQLiteManagerTest, InitializationDoesNotThrow) {
    // Arrange & Act: SetUp уже все сделал
    // Assert: Если мы здесь, значит конструктор отработал без исключений
    SUCCEED();
}

// Тест 2: Проверяем, что метод SaveCurriculum корректно добавляет данные
TEST_F(SQLiteManagerTest, SaveNewCurriculumAndCheckWithHasCurriculum) {
    // Arrange: Создаем тестовый учебный план
    Curriculum curriculum;
    curriculum.language = "C++";
    curriculum.standard = "C++17";
    curriculum.modules.emplace_back("Core Language");
    curriculum.modules.back().topics.emplace_back("RAII", "базовый");

    // Act: Сохраняем план
    manager_->SaveCurriculum(curriculum);

    // Assert: Проверяем через публичный метод, что данные появились
    ASSERT_TRUE(manager_->HasCurriculum("C++", "C++17"));
}

// Тест 3: Проверяем идемпотентность (косвенно)
TEST_F(SQLiteManagerTest, SaveExistingCurriculumIsIdempotent) {
    // Arrange: Создаем и сохраняем план
    Curriculum curriculum;
    curriculum.language = "C++";
    curriculum.standard = "C++17";
    curriculum.modules.emplace_back("Core");
    manager_->SaveCurriculum(curriculum);

    // Act & Assert: Повторное сохранение не должно выбросить исключение
    // из-за нарушения UNIQUE constraint в базе данных.
    ASSERT_NO_THROW(manager_->SaveCurriculum(curriculum));
}

// Тест 4: Проверяем, что HasCurriculum правильно возвращает false
TEST_F(SQLiteManagerTest, HasCurriculumReturnsFalseForNonexistent) {
    // Arrange: База данных пуста
    // Act & Assert:
    ASSERT_FALSE(manager_->HasCurriculum("C++", "C++17"));
}

// -------- ТЕСТЫ ДЛЯ GEMINIPARSER ----------

// Тестовый набор для парсера ответов от Gemini
class GeminiParserTest : public ::testing::Test {
protected:
    // Пример "сырого" ответа от API Gemini
    const std::string raw_api_response_ = R"({
        "candidates": [
            {
                "content": {
                    "parts": [
                        {
                            "text": "```json\n[{\"module_name\":\"Основы C++\",\"topics\":[{\"topic_name\":\"Переменные и типы данных\",\"difficulty\":\"базовый\"},{\"topic_name\":\"Операторы\",\"difficulty\":\"базовый\"}]}]```"
                        }
                    ],
                    "role": "model"
                }
            }
        ]
    })";

    // Пример "чистого" JSON учебного плана
    const std::string clean_curriculum_json_ = R"([
        {
            "module_name": "Core Language",
            "topics": [
                { "topic_name": "RAII", "difficulty": "базовый" },
                { "topic_name": "Move Semantics", "difficulty": "продвинутый" }
            ]
        },
        {
            "module_name": "STL",
            "topics": [
                { "topic_name": "Containers", "difficulty": "средний" }
            ]
        }
    ])";
};

// Тестируем извлечение контента из полного ответа API
TEST_F(GeminiParserTest, ExtractJsonContentFromValidResponse) {
    std::string extracted = ExtractJsonContent(raw_api_response_);
    // Проверяем, что маркеры ```json были удалены
    ASSERT_EQ(extracted, R"([{"module_name":"Основы C++","topics":[{"topic_name":"Переменные и типы данных","difficulty":"базовый"},{"topic_name":"Операторы","difficulty":"базовый"}]}])");
}

// Тестируем трансформацию чистого JSON в наши структуры данных
TEST_F(GeminiParserTest, TransformJsonToCurriculumFromValidJson) {
    Curriculum curriculum = TransformJsonToCurriculum(clean_curriculum_json_, "C++", "C++17");

    // Проверяем общие данные
    ASSERT_EQ(curriculum.language, "C++");
    ASSERT_EQ(curriculum.standard, "C++17");

    // Проверяем структуру
    ASSERT_EQ(curriculum.modules.size(), 2);
    ASSERT_EQ(curriculum.modules[0].name, "Core Language");
    ASSERT_EQ(curriculum.modules[0].topics.size(), 2);
    ASSERT_EQ(curriculum.modules[1].name, "STL");
    ASSERT_EQ(curriculum.modules[1].topics.size(), 1);

    // Проверяем конкретные данные
    ASSERT_EQ(curriculum.modules[0].topics[1].name, "Move Semantics");
    ASSERT_EQ(curriculum.modules[0].topics[1].difficulty, "продвинутый");
}

// Тестируем обработку ошибок
TEST_F(GeminiParserTest, TransformJsonThrowsOnEmptyInput) {
    ASSERT_THROW(TransformJsonToCurriculum("", "C++", "C++17"), std::runtime_error);
}

TEST_F(GeminiParserTest, TransformJsonThrowsOnMalformedJson) {
    const std::string malformed_json = R"([{"module_name": "Incomplete"})";
    ASSERT_THROW(TransformJsonToCurriculum(malformed_json, "C++", "C++17"), nlohmann::json::parse_error);
}
