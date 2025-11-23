#include <memory>
#include <print>
#include <string>
#include <vector>
#include "dataBaseInterface.hpp"
#include "databaseFactory.hpp"
#include "TerminalOut.hpp"
// using namespace ftxui;
using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;

std::unique_ptr<IDatabaseConnector> CreateConnector(int argc, char *argv[]);

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::println(
            "\033[31mИспользование: {} --sqlite | --postgresql | "
            "--mariadb\033[0m\n" "\033[33mИспользуй --help для справки\033[0m",
            argv[0]);
    return 0;
  }

  TerminalOut Out = {};
  Out.Render();
  std::unique_ptr<IDatabaseConnector> DBConnector = CreateConnector(argc, argv);
  DatabaseResultTable result {};

  if (DBConnector->Connect("test.db")) {
    result = DBConnector->ExecuteQuery("select * from users;");
    DBConnector->Disconnect();

    // Выводим результат
    for (const auto& row : result) {
      for (const auto& cell : row) {
        std::print("{} ", cell);
      }
      std::println("");
    }
  } else {
    std::println("Failed to connect to database!");
  }

  return 0;
}


std::unique_ptr<IDatabaseConnector> CreateConnector(int argc, char *argv[]) {

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "--help" || arg == "-h") {
      std::println("Помощь: используй --sqlite, --postgresql, --mariadb");
      std::exit(1);
    }

    else if (arg == "--sqlite")
    {
      return Factory::MakeSQLiteConn();
    }

    else if (arg == "--postgresql")
    {
      return Factory::MakePostgresConn();
    }

    else if (arg == "--mariadb")
    {
      return Factory::MakeMariaDBConn();
    }
  }
  return nullptr;
}
