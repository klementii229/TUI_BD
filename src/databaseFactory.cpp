
#include "databaseFactory.hpp"

std::unique_ptr<SQLiteConnector> Factory::MakeSQLiteConn(void)
{
  return std::make_unique<SQLiteConnector>();
}

std::unique_ptr<MariadbConnector> Factory::MakeMariaDBConn(void)
{
  return std::make_unique<MariadbConnector>();
}

std::unique_ptr<PostgresConnector> Factory::MakePostgresConn(void)
{
  return std::make_unique<PostgresConnector>();
}
