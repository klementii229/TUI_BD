#include "sqlite_connector.hpp"
#include <cstddef>
#include <cstdio>
#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <vector>

SQLiteConnector::~SQLiteConnector() { Disconnect(); }

bool SQLiteConnector::Connect(const std::string &connectionString) {
    if (sqlite3_open(connectionString.c_str(), &db) == SQLITE_OK) {
        return true;
    }
    if (db != nullptr) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        db = nullptr;
    }

    return false;
}

void SQLiteConnector::Disconnect() { sqlite3_close(db); }

DatabaseResultTable SQLiteConnector::ExecuteQuery(const std::string &query) {
    DatabaseResultTable result;

    int ret_code = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (ret_code != SQLITE_OK) {
        throw std::runtime_error(
            "Ошибка подготовки запроса SQLiteConnector::ExecuteQuery");
    }
    int column_count = sqlite3_column_count(stmt);

    while ((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
        DatabaseRow row;
        for (int i = 0; i < column_count; ++i) {
            const char *column_value =
                reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
            row.push_back(column_value ? column_value : "NULL");
        }
        result.push_back(row);
    }

    if (ret_code != SQLITE_DONE) {
        throw std::runtime_error(
            "Ошибка завершения запроса SQLiteConnector::ExecuteQuery");
    }

    // Освобождение ресурсов
    sqlite3_finalize(stmt);
    return result;
}

bool SQLiteConnector::ExecuteCommand(const std::string &query) {
    char *err_msg = nullptr;
    int ret_code = sqlite3_exec(db, query.c_str(), NULL, NULL, &err_msg);
    if (ret_code != SQLITE_OK) {
        std::string err_msg = sqlite3_errmsg(db);
        throw std::runtime_error(
            "Ошибка запроса SQLiteConnector::ExecuteCommand" + err_msg);
    }

    sqlite3_finalize(stmt);
    return true;
}

std::vector<std::string> SQLiteConnector::GetTableList() {
    std::string query = "SELECT name FROM sqlite_master "
                        "WHERE type='table' AND name NOT LIKE 'sqlite_%' "
                        "ORDER BY name";
    int return_code = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (return_code != SQLITE_OK) {
        std::string error_msg = sqlite3_errmsg(db);
        throw std::runtime_error(
            "Ошибка запроса SQLiteConnector::ExecuteQuery " + error_msg);
    }
    std::vector<std::string> result{};
    int column_count = sqlite3_column_count(stmt);
    while ((return_code = sqlite3_step(stmt)) == SQLITE_ROW) {
        for (int i = 0; i < column_count; ++i) {
            const char *column_value =
                reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
            result.push_back(column_value ? column_value : "NULL");
        }
    }
    if (return_code != SQLITE_DONE) {
        throw std::runtime_error(
            "Ошибка завершения запроса SQLiteConnector::ExecuteQuery");
    }
    sqlite3_finalize(stmt);

    return result;
}

DatabaseResultTable
SQLiteConnector::GetTableSchema(const std::string &tableName) {}
