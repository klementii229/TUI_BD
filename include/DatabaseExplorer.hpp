#pragma once

#include <memory>
#include <string>

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

#include "DataBaseInterface.hpp"

class DataBaseExplorer
{
public:
  DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_);
  DataBaseExplorer() = delete;
  ftxui::Component CreateForm();
  void RUN();
  void UpdateResultDisplay();
private:
  std::unique_ptr<IDatabaseConnector> conn;
  DatabaseResultTable resultTable;
  ftxui::ScreenInteractive screen;

  ftxui::Component form_container;
  ftxui::Component req_input;

  ftxui::Component btn_send_req;
  ftxui::Component btn_close;
  ftxui::Component btn_prev_page;
  ftxui::Component btn_next_page;


  ftxui::Component result_panel;
  ftxui::Component result_container;

  std::string t_request {};
  bool has_result = false;
  std::string last_err;

  int current_page = 1;
  const int rows_per_page = 15;
  int total_rows;
  int total_pages;


};
