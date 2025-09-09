cmake_minimum_required(VERSION 4.0)
project(TUI_DB)

set(CMAKE_CXX_STANDARD 23 REQUIRED)

# Ищем SOCI в системе
find_path(SOCI_INCLUDE_DIR soci/soci.h)
find_library(SOCI_CORE_LIB soci_core)
find_library(SOCI_SQLITE_LIB soci_sqlite3)

if(NOT SOCI_INCLUDE_DIR OR NOT SOCI_CORE_LIB)
    message(FATAL_ERROR "SOCI not found!")
endif()

# Добавляем FTXUI
include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

# Рекурсивно находим ВСЕ файлы в папках src и include
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Создаем исполняемый файл из всех найденных исходников
add_executable(tui_db ${SOURCES} ${HEADERS})

# Добавляем include директории
target_include_directories(tui_db
  PRIVATE
    ${SOCI_INCLUDE_DIR}
    include  # Добавляем нашу папку include
)

# Связываем библиотеки
target_link_libraries(tui_db
  PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
    ${SOCI_CORE_LIB}
    ${SOCI_SQLITE_LIB}
)

# Опционально: устанавливаем свойства для исходных файлов
set_target_properties(tui_db PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
   # CXX_EXTENSIONS OFF
)
