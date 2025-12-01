#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

using ConnectionTuple = std::tuple<std::string, std::string, std::string,
                                   std::string, std::string, std::string>;

class LoginForm {
public:
  LoginForm();
  void RUN();
  ftxui::Component CreateConnectionForm();
  ConnectionTuple GetConnectionParams() const;
  struct ConnectionData {
    std::string host = "localhost";
    std::string port = "5432";
    std::string database;
    std::string username;
    std::string password;
    std::string db_type;
  };

private:
  ftxui::ScreenInteractive screen;
  ConnectionData connection_data;

  ftxui::Component host_input;
  ftxui::Component port_input;
  ftxui::Component db_input;
  ftxui::Component user_input;
  ftxui::Component password_input;
  ftxui::Component db_type_radio;
  ftxui::Component connect_button;

  int db_type_selected = 0;
  std::vector<std::string> db_types = {"PostgreSQL", "MariaDB", "SQLite"};
};
