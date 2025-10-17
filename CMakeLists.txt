
#CMakeLists.txt

cmake_minimum_required(VERSION 3.17)
project(my_project LANGUAGES CXX)

# --- Настройки проекта ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
#add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Включаем сбор информации о файле/строке для всех уровней логирования
# PUBLIC, чтобы действовало на все цели, которые линкуют spdlog
# target_compile_definitions(my_ai_tutor PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
# target_compile_definitions(g_tests PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

if(MSVC)
  add_compile_options(/utf-8)
endif()

enable_testing()

# Определяем .cpp файлы, которые являются "библиотекой" приложения
set(AITUTOR_LIB_SOURCES
    src/app/Application.cpp
    src/app/CreateCurriculumUseCase.cpp
    src/app/VirtualUserInterface.cpp
    src/infrastructure/GeminiService.cpp
    src/infrastructure/SQLiteManager.cpp
    src/infrastructure/GeminiParser.cpp
    src/core/PromptRegistry.cpp
    src/process/CurriculumGenerationProcess.cpp
)

# Определяем .h файлы, чтобы Visual Studio их видела
set(AITUTOR_HEADERS
    src/app/Application.h
    src/app/CreateCurriculumUseCase.h
    src/app/VirtualUserInterface.h
    src/infrastructure/GeminiService.h
    src/infrastructure/SQLiteManager.h
    src/infrastructure/GeminiParser.h
    src/interfaces/IDatabaseManager.h
    src/interfaces/ILLMService.h
    src/interfaces/IUserInterface.h
    src/core/DataModels.h
    src/core/PromptRegistry.h
    src/process/CurriculumGenerationProcess.h
    src/process/IAgent.h
    src/process/IGenerativeProcess.h
)

# Определяем точку входа
set(APP_ENTRY_SOURCE src/main.cpp)

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

add_executable(my_ai_tutor ${APP_ENTRY_SOURCE} ${AITUTOR_LIB_SOURCES})
target_sources(my_ai_tutor PRIVATE ${AITUTOR_HEADERS})
target_link_libraries(my_ai_tutor PRIVATE ${COMMON_LIBRARIES})
target_compile_definitions(my_ai_tutor PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

# --- Тесты --- 

add_executable(g_tests tests/test_main.cpp ${AITUTOR_LIB_SOURCES})
target_link_libraries(g_tests PRIVATE GTest::gtest_main ${COMMON_LIBRARIES})
target_sources(g_tests PRIVATE ${AITUTOR_HEADERS})
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