#include "DatabaseExplorer.hpp"
#include "LoginForm.hpp"
#include "sqlite_connector.hpp"
#include <memory>
#include <soci/soci-backend.h>
#include <string>
#include <vector>
using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;

int main(void) {

  LoginForm Form = {};
  Form.RUN();
  auto [host, port, database, username, password, db_type] =
      Form.GetConnectionParams();
  std::unique_ptr<IDatabaseConnector> conn = nullptr;
  if (db_type == "SQLite") {
    conn = std::make_unique<SQLiteConnector>();
  } else {
    return 1;
  }
  conn->Connect("test.db");
  DataBaseExplorer exp = {std::move(conn)};
  exp.RUN();

  return 0;
}
