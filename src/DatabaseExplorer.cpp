#include "DatabaseExplorer.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/color.hpp>

#include "ftxui/dom/elements.hpp"

DataBaseExplorer::DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_)
    : screen(ftxui::ScreenInteractive::Fullscreen()), conn(std::move(conn_)) {
    main_window = ftxui::Renderer([this]() {
        return ftxui::vbox({ftxui::text("T U I D B") | ftxui::center | ftxui::color(ftxui::Color::Cyan) | ftxui::bold,
            ftxui::separator()});  // Название и рамка вокруг неё
    }) | ftxui::border;
}

void DataBaseExplorer::RUN() { screen.Loop(main_window); }
