#include <memory>
#include <print>


#include "databaseFactory.hpp"

using namespace ftxui;
using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;


int main()
{
  DatabaseResultTable result {};

  std::unique_ptr<IDatabaseConnector> sqlite = Factory::MakeSQLiteConn();

  if (sqlite->Connect("test.db")) {
    result = sqlite->ExecuteQuery("select * from users;");
    sqlite->Disconnect();

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
