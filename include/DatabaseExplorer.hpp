#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/dom/table.hpp>
#include <memory>

#include "DataBaseInterface.hpp"

class DataBaseExplorer {
  public:
   void RUN();

   DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_);
   DataBaseExplorer() = delete;

  private:
   std::unique_ptr<IDatabaseConnector> conn;
   ftxui::ScreenInteractive screen;
   Table db_result;

   ftxui::Component main_window;
   ftxui::Component main_container;

   ftxui::Component req_input;
   std::string req_text;
   ftxui::Component btn_send_req;

   ftxui::Table table;
};
