// database_interface.h
#pragma once
#include <string>
#include <vector>

#include <soci/soci.h>

// Собственный "легкий" тип для представления строки результата
using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;

enum class DatabaseType : std::uint8_t
{
  Sqlite,
  Postgres,
  Mariadb
};

class IDatabaseConnector
{
public:
  // Чисто виртуальные методы, которые должны быть реализованы
  virtual bool Connect(const std::string& connectionString) = 0;
  virtual void Disconnect() = 0;
  virtual bool IsConnected() const = 0;

  // Выполнить запрос и получить результат (для SELECT)
  virtual DatabaseResultTable ExecuteQuery(const std::string& query) = 0;
  // Выполнить команду (INSERT, UPDATE, DELETE)
  virtual bool ExecuteCommand(const std::string& command) = 0;

  // Методы для получения метаданных (схемы БД)
  virtual std::vector<std::string> GetTableList() = 0;
  virtual DatabaseResultTable GetTableSchema(const std::string& tableName) = 0;

  // Можно добавить метод для получения имени типа БД
  virtual DatabaseType GetDatabaseType() const = 0;  // а зачем он?

  virtual ~IDatabaseConnector() = default;
};
