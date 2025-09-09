// database_interface.h
#include <string>
#include <vector>
#include <soci/soci.h>

// Наш собственный "легкий" тип для представления строки результата
using DatabaseRow = std::vector<std::string>;
using DatabaseResult = std::vector<DatabaseRow>;

class IDatabaseConnector {
public:
    virtual ~IDatabaseConnector() = default;

    // Чисто виртуальные методы, которые должны быть реализованы
    virtual bool Connect(const std::string& connectionString) = 0;
    virtual void Disconnect() = 0;
    virtual bool IsConnected() const = 0;

    // Выполнить запрос и получить результат (для SELECT)
    virtual DatabaseResult ExecuteQuery(const std::string& query) = 0;
    // Выполнить команду (INSERT, UPDATE, DELETE)
    virtual bool ExecuteCommand(const std::string& command) = 0;

    // Методы для получения метаданных (схемы БД)
    virtual std::vector<std::string> GetTableList() = 0;
    virtual DatabaseResult GetTableSchema(const std::string& tableName) = 0;

    // Можно добавить метод для получения имени типа БД
    virtual std::string GetDatabaseType() const = 0;
};
