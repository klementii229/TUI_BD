#include <print>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include "dataBaseInterface.hpp"
#include "sqlite_connector.hpp"
using namespace ftxui;

int main()
{
  DatabaseResultTable result {};
  SQLiteConnector SQLite {};
  SQLite.Connect("test.db");
  result = SQLite.ExecuteQuery("select * from users;");
  SQLite.Disconnect();
  for (const auto& row : result) {
    for (const auto& cell : row) {
      std::print("{} ", cell);
    }
    std::println("");  // новая строка после каждой строки
  }
  return 0;
}
