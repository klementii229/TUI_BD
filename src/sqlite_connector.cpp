#include "sqlite_connector.hpp"
#include "DataBaseInterface.hpp"
#include <format>
#include <source_location>
#include <sqlite3.h>

std::expected<bool, std::string> SQLiteConnector::Connect(const std::string_view connectionString) {
    if (sqlite3_open(connectionString.data(), &db) == SQLITE_OK) {
        return true;
    }
    std::string errorMessage = sqlite3_errmsg(db);
    sqlite3_close(db);
    db = nullptr;

    return std::unexpected(errorMessage);
}

std::expected<DatabaseResultTable, std::string> SQLiteConnector::ExecuteQuery(const std::string_view query) {
    DatabaseResultTable result;
    sqlite3_stmt *stmt{};

    int ret_code = sqlite3_prepare_v2(db, query.data(), -1, &stmt, NULL);
    if (ret_code != SQLITE_OK) {
        std::string error_message = sqlite3_errmsg(db);
        constexpr std::source_location loc = std::source_location::current();
        return std::unexpected(std::format("Ошибка запроса в {}:{} {}", loc.line(), loc.column(), error_message));
    }

    int column_count = sqlite3_column_count(stmt);

    while ((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
        DatabaseRow row;
        for (int i = 0; i < column_count; ++i) {
            const char *column_value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
            row.push_back(column_value ? column_value : "NULL");
        }
        result.push_back(row);
    }

    if (ret_code != SQLITE_DONE) {
        std::string error_message = sqlite3_errmsg(db);
        constexpr std::source_location loc = std::source_location::current();
        return std::unexpected(
            std::format("Ошибка завершения запроса в {}:{} {}", loc.line(), loc.column(), error_message));
    }

    sqlite3_finalize(stmt);
    return result;
}

std::expected<bool, std::string> SQLiteConnector::ExecuteCommand(const std::string_view query) {
    int ret_code = sqlite3_exec(db, query.data(), NULL, NULL, NULL);
    if (ret_code != SQLITE_OK) {
        std::string error_message = sqlite3_errmsg(db);
        constexpr std::source_location loc = std::source_location::current();
        return std::unexpected(std::format("Ошибка запроса в {}:{} {}", loc.line(), loc.column(), error_message));
    }
    return true;
}

std::expected<DatabaseRow, std::string> SQLiteConnector::GetTableList() {
    std::string query = "SELECT name FROM sqlite_master "
                        "WHERE type='table' AND name NOT LIKE 'sqlite_%' "
                        "ORDER BY name";
    sqlite3_stmt *stmt{};
    int return_code = sqlite3_prepare_v2(db, query.data(), -1, &stmt, nullptr);
    if (return_code != SQLITE_OK) {
        std::string error_message = sqlite3_errmsg(db);
        constexpr std::source_location loc = std::source_location::current();
        return std::unexpected(std::format("Ошибка списка таблиц {}:{} {}", loc.line(), loc.column(), error_message));
    }

    std::vector<std::string> result{};
    int column_count = sqlite3_column_count(stmt);
    while ((return_code = sqlite3_step(stmt)) == SQLITE_ROW) {
        for (int i = 0; i < column_count; ++i) {
            const char *column_value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
            result.push_back(column_value ? column_value : "NULL");
        }
    }
    if (return_code != SQLITE_DONE) {
        std::string error_message = sqlite3_errmsg(db);
        constexpr std::source_location loc = std::source_location::current();
        return std::unexpected(
            std::format("Ошибка завершения запроса в {}:{} {}", loc.line(), loc.column(), error_message));
    }
    sqlite3_finalize(stmt);

    return result;
}

std::expected<DatabaseRow, std::string> SQLiteConnector::GetTableSchema(const std::string_view tableName) {
    std::string query = std::format("select sql from sqlite_master where type = 'table' and name = '{}'", tableName);

    sqlite3_stmt *stmt = nullptr;
    int ret_code = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    if (ret_code != SQLITE_OK) {
        std::string error_message = sqlite3_errmsg(db);
        auto loc = std::source_location::current();
        return std::unexpected(std::format("Ошибка получения схемы таблицы {} в {}:{} {}", tableName, loc.line(),
                                           loc.column(), error_message));
    }

    DatabaseRow result;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *schema = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        if (schema) {
            result.push_back(schema);
        } else {
            result.push_back("NULL");
        }
    } else {
        // Таблица не найдена
        sqlite3_finalize(stmt);
        return std::unexpected(std::format("Таблица '{}' не найдена", tableName));
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string error_message = sqlite3_errmsg(db);
        auto loc = std::source_location::current();
        sqlite3_finalize(stmt);
        return std::unexpected(
            std::format("Ошибка завершения запроса в {}:{} {}", loc.line(), loc.column(), error_message));
    }

    sqlite3_finalize(stmt);
    return result;
}

void SQLiteConnector::Disconnect() { sqlite3_close(db); }
SQLiteConnector::~SQLiteConnector() { Disconnect(); }
