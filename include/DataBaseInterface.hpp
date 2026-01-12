#pragma once
#include <string>
#include <vector>

using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;

//enum class DatabaseType : std::uint8_t { Sqlite, Postgres, MariaDB };
/*Create — INSERT (создание записей),

Read — SELECT (чтение записей),

Update — UPDATE (редактирование записей),

Delete — DELETE (удаление записей). */
class IDatabaseConnector {
public:
  // Чисто виртуальные методы, которые должны быть реализованы
  virtual bool Connect(const std::string &connectionString) = 0;
  virtual void Disconnect() = 0;
  virtual bool IsConnected() const = 0;

  // Выполнить запрос и получить результат (для SELECT)
  virtual DatabaseResultTable ExecuteQuery(const std::string &query) = 0;
  // Выполнить команду (INSERT, UPDATE, DELETE)
  virtual bool ExecuteCommand(const std::string &command) = 0;

  // Методы для получения метаданных (схемы БД)
  virtual std::vector<std::string> GetTableList() = 0;
  virtual DatabaseResultTable GetTableSchema(const std::string &tableName) = 0;

  virtual ~IDatabaseConnector() = default;
};
