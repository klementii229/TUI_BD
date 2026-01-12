cmake_minimum_required(VERSION 3.16)
project(TUI_DB)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ================================
# Поиск библиотек: SQLiteCpp и libpqxx
# ================================
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

# Поиск libpqxx через pkg-config
pkg_check_modules(PQXX REQUIRED libpqxx)

# Указываем пути к SQLiteCpp вручную
set(SQLITECPP_INCLUDE_DIR "/usr/local/include")
set(SQLITECPP_LIBRARY "/usr/local/lib/libSQLiteCpp.a")

# ================================
# Добавляем FTXUI
# ================================
include(FetchContent)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

# ================================
# Собираем исходники
# ================================
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

add_executable(tui_db ${SOURCES} ${HEADERS})

# ================================
# Include директории
# ================================
target_include_directories(tui_db
    PRIVATE
        ${SQLITECPP_INCLUDE_DIR}
        ${PQXX_INCLUDE_DIRS}
        include
)

# ================================
# Линковка библиотек
# ================================
target_link_libraries(tui_db
    PRIVATE
        ftxui::screen
        ftxui::dom
        ftxui::component
        ${SQLITECPP_LIBRARY}
        ${PQXX_LIBRARIES}
        Threads::Threads
)
