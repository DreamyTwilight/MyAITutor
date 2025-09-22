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


