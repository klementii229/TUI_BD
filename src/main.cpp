#include "DataBaseFactory.hpp"
#include "DataBaseInterface.hpp"
#include "LoginForm.hpp"
#include "DatabaseExplorer.hpp"
#include <cstdlib>
#include <memory>
#include <print>
#include <string>
#include <vector>
using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;

// std::unique_ptr<IDatabaseConnector> CreateConnector(int argc, char *argv[]);

int main(void) {

  LoginForm Form = {};
  Form.RUN();
  auto [host, port, database, username, password, db_type] =
      Form.GetConnectionParams();
      std::unique_ptr<IDatabaseConnector> conn = Factory::MakeSQLiteConn();
      conn->Connect(database);
      DataBaseExplorer exp = {std::move(conn)};
      exp.RUN();

  return 0;
}

std::unique_ptr<IDatabaseConnector> CreateConnector(int argc, char *argv[]) {
  if (argc != 2) {
    std::println("\033[31mИспользование: {} --sqlite | --postgresql | "
                 "--mariadb\033[0m\n"
                 "\033[31mИспользуй --help для справки\033[0m",
                 argv[0]);
    return nullptr;
  }

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "--help" || arg == "-h") {
      std::println("Помощь: используй --sqlite, --postgresql, --mariadb");
      std::exit(1);
    }

    else if (arg == "--sqlite") {
      return Factory::MakeSQLiteConn();
    }

    else if (arg == "--postgresql") {
      return Factory::MakePostgresConn();
    }

    else if (arg == "--mariadb") {
      return Factory::MakeMariaDBConn();
    }
  }
  return nullptr;
}
