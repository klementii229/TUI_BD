#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <vector>

using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;



class TerminalOut {
public:
  TerminalOut();
  void PrintHeader();
  void Run();
  ftxui::Component CreateConnectionForm();

  struct ConnectionData {
    std::string host = "localhost";
    std::string port;
    std::string database;
    std::string username;
    std::string password;
    std::string db_type; // "postgresql" или "mariadb"
  };

private:
  ftxui::ScreenInteractive screen;
  ftxui::Element header;
  ftxui::Element layout;
  ftxui::Component mainComp;
  ConnectionData connection_data;
};
