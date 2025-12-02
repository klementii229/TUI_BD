#pragma once
#include <memory>

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "DataBaseInterface.hpp"

class DataBaseExplorer
{
public:
  DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_);
  DataBaseExplorer() = delete;
  ftxui::Component CreateForm();
  void RUN();
private:
  std::unique_ptr<IDatabaseConnector> conn;
  DatabaseResultTable resultTable;
  ftxui::ScreenInteractive screen;

  ftxui::Component form_container;
  ftxui::Component req_input;
  ftxui::Component btn_send_req;
  ftxui::Component btn_close;

  std::string t_request {};
};
