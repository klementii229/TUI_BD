#pragma once
#include <memory>

#include "mariadb_connector.hpp"
#include "postgres_connector.hpp"
#include "sqlite_connector.hpp"

namespace Factory
{
std::unique_ptr<SQLiteConnector> MakeSQLiteConn(void);
std::unique_ptr<MariadbConnector> MakeMariaDBConn(void);
std::unique_ptr<PostgresConnector> MakePostgresConn(void);
}
