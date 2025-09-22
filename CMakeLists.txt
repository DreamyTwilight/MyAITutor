
#CMakeLists.txt

cmake_minimum_required(VERSION 3.17)
project(my_project LANGUAGES CXX)

# --- Настройки проекта ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем сбор информации о файле/строке для всех уровней логирования
# PUBLIC, чтобы действовало на все цели, которые линкуют spdlog
# target_compile_definitions(my_ai_tutor PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
# target_compile_definitions(g_tests PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

if(MSVC)
  add_compile_options(/utf-8)
endif()

enable_testing()

set(APP_SOURCES
    src/main.cpp
    src/app/Application.h
    src/app/Application.cpp
    src/app/CreateCurriculumUseCase.h
    src/app/CreateCurriculumUseCase.cpp
    src/app/VirtualUserInterface.h
    src/app/VirtualUserInterface.cpp
    src/infrastructure/GeminiService.h
    src/infrastructure/GeminiService.cpp
    src/infrastructure/SQLiteManager.h
    src/infrastructure/SQLiteManager.cpp
    src/interfaces/IDatabaseManager.h
    src/interfaces/ILLMService.h
    src/interfaces/IUserInterface.h
    src/core/DataModels.h
    src/core/PromptRegistry.h
    src/core/PromptRegistry.cpp
)

set(COMMON_LIBRARIES 
    nlohmann_json::nlohmann_json
    cpr::cpr
    SQLiteCpp
    spdlog::spdlog
    fmt::fmt
)

find_package(SQLiteCpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(cpr REQUIRED)
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
find_package(fmt REQUIRED)

# --- Основное приложение ---

add_executable(my_ai_tutor ${APP_SOURCES})
target_link_libraries(my_ai_tutor PRIVATE ${COMMON_LIBRARIES})
target_compile_definitions(my_ai_tutor PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

# --- Тесты --- 

add_executable(g_tests tests/test_main.cpp)
target_link_libraries(g_tests PRIVATE GTest::gtest_main ${COMMON_LIBRARIES})
add_test(NAME all_tests COMMAND g_tests)
target_compile_definitions(g_tests PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

# --- Платформенно-зависимая конфигурация ---

# в windows используем скачанный с https://curl.se/ca/cacert.pem файл сертификатов
# встроенное хранилище пока не получилось заставить работать
if(WIN32)
    message(STATUS "Windows detected, configuring path to local cacert.pem")
    set(CACERT_FILE_PATH "${CMAKE_SOURCE_DIR}/res/cacert.pem")
    target_compile_definitions(my_ai_tutor PRIVATE "CACERT_PATH=\"${CACERT_FILE_PATH}\"")
    target_compile_definitions(g_tests PRIVATE "CACERT_PATH=\"${CACERT_FILE_PATH}\"")
endif()