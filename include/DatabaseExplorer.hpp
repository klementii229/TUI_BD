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

  ftxui::Component m_component;
  ftxui::Component req_imput;
  ftxui::Component btn_send_req;

  std::string t_request {};
};
