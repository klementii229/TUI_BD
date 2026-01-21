#pragma once
#include <memory>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/node.hpp>

#include "DataBaseInterface.hpp"

class DataBaseExplorer {
  public:
    DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_);

    void RUN();

    DataBaseExplorer() = delete;

  private:
    std::unique_ptr<IDatabaseConnector> conn;
    ftxui::ScreenInteractive screen;
    ftxui::Component main_window;
};
