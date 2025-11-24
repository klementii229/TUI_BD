
#include "DataBaseFactory.hpp"

std::unique_ptr<SQLiteConnector> Factory::MakeSQLiteConn()
{
  return std::make_unique<SQLiteConnector>();
}

std::unique_ptr<MariadbConnector> Factory::MakeMariaDBConn()
{
  return std::make_unique<MariadbConnector>();
}

std::unique_ptr<PostgresConnector> Factory::MakePostgresConn()
{
  return std::make_unique<PostgresConnector>();
}
