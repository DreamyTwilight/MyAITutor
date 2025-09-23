#include <gtest/gtest.h>

#include <SQLiteCpp/SQLiteCpp.h>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <curl/curl.h>


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
#include "../src/interfaces/IDatabaseManager.h"
#include "../src/infrastructure/SQLiteManager.h"
#include "../src/core/DataModels.h"             

// Test Fixture для SQLiteManager
class SQLiteManagerTest : public ::testing::Test {
protected:
    const std::string test_db_filename_ = ":memory:";

    std::unique_ptr<SQLiteManager> manager_;

    void SetUp() override {
        manager_ = std::make_unique<SQLiteManager>(test_db_filename_);
    }

    void TearDown() override {
        manager_.reset(); // Объект удаляется, соединение с БД закрывается, память освобождается
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
