cmake_minimum_required(VERSION 3.16)
project(TUI_DB)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ================================
# Ищем SOCI и его бэкенды
# ================================
find_path(SOCI_INCLUDE_DIR soci/soci.h)

find_library(SOCI_CORE_LIB soci_core)
find_library(SOCI_SQLITE_LIB soci_sqlite3)
find_library(SOCI_POSTGRESQL_LIB soci_postgresql)
find_library(SOCI_MYSQL_LIB soci_mysql)

if(NOT SOCI_INCLUDE_DIR OR NOT SOCI_CORE_LIB)
    message(FATAL_ERROR "SOCI not found! Установи libsoci-dev и нужные бэкенды.")
endif()

# ================================
# Добавляем FTXUI
# ================================
include(FetchContent)
FetchContent_Declare(ftxui
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
    ${SOCI_INCLUDE_DIR}
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
    ${SOCI_CORE_LIB}
)

if(SOCI_SQLITE_LIB)
    target_link_libraries(tui_db PRIVATE ${SOCI_SQLITE_LIB})
endif()

if(SOCI_POSTGRESQL_LIB)
    find_library(POSTGRESQL_LIB pq)
    if(POSTGRESQL_LIB)
        target_link_libraries(tui_db PRIVATE ${SOCI_POSTGRESQL_LIB} ${POSTGRESQL_LIB})
    else()
        message(WARNING "PostgreSQL client library (libpq) not found, PostgreSQL backend отключён")
    endif()
endif()

if(SOCI_MYSQL_LIB)
    find_library(MARIADB_LIB mariadb)
    if(MARIADB_LIB)
        target_link_libraries(tui_db PRIVATE ${SOCI_MYSQL_LIB} ${MARIADB_LIB})
    else()
        find_library(MYSQLCLIENT_LIB mysqlclient)
        if(MYSQLCLIENT_LIB)
            target_link_libraries(tui_db PRIVATE ${SOCI_MYSQL_LIB} ${MYSQLCLIENT_LIB})
        else()
            message(WARNING "MySQL/MariaDB client library not found, MySQL backend отключён")
        endif()
    endif()
endif()
