#pragma once
#include <string>
#include <vector>

using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;

class IDatabaseConnector {
  public:
    // Выполнить запрос и получить результат (для SELECT)
    virtual DatabaseResultTable ExecuteQuery(const std::string &query) = 0;
    // Выполнить команду (INSERT, UPDATE, DELETE)
    virtual bool ExecuteCommand(const std::string &command) = 0;

    // Методы для получения метаданных (схемы БД)
    virtual std::vector<std::string> GetTableList() = 0;
    virtual DatabaseResultTable
    GetTableSchema(const std::string &tableName) = 0;
    virtual ~IDatabaseConnector() = default;
    // ##########################################
    virtual bool Connect(const std::string &connectionString) = 0;
    virtual void Disconnect() = 0;
};

/*Create — INSERT (создание записей),

Read — SELECT (чтение записей),

Update — UPDATE (редактирование записей),

Delete — DELETE (удаление записей). */
