#include "DatabaseExplorer.hpp"

#include <format>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>

#include "DataBaseInterface.hpp"

static ftxui::ButtonOption CreateRoundedButtonOption();
static ftxui::Table FormatTable(const Table& table);

DataBaseExplorer::DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_)
    : screen(ftxui::ScreenInteractive::Fullscreen()), conn(std::move(conn_)) {
   req_input = ftxui::Input(&req_text, "Enter SQL request") | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1) | ftxui::flex | ftxui::border;

   table_component = ftxui::Renderer([this] {
      if (db_result.empty()) {
         return ftxui::text("No data") | ftxui::center;
      }
      return ftxui::Table(db_result).Render() | ftxui::border;
   });

   btn_send_req = ftxui::Button(
       "Enter",
       [this] {
          db_result.clear();
          if (req_text.find("select") != std::string::npos || req_text.find("SELECT") != std::string::npos) {
             auto res = conn->FetchAll(req_text);
             if (res.has_value()) {
                db_result = std::move(res.value());
             } else {
                db_result = {{res.error()}};
             }
          } else {
             auto res = conn->ExecuteUpdate(req_text);
             if (res.has_value()) {
                db_result = {{std::format("Success, rows affected: {}", res.value())}};
             } else {
                db_result = {{res.error()}};
             }
          }
          screen.Post(ftxui::Event::Custom);
       },
       CreateRoundedButtonOption());

   main_container = ftxui::Container::Vertical({req_input, btn_send_req, table_component});

   main_window = ftxui::Renderer(main_container, [this] {
      return ftxui::vbox({ftxui::text("T U I D B") | ftxui::center | ftxui::color(ftxui::Color::Cyan) | ftxui::bold, ftxui::separator(),
                          // ФИКСИРОВАННАЯ ВЫСОТА ДЛЯ ВЕРХНЕЙ ПАНЕЛИ ↓
                          ftxui::hbox({req_input->Render(), btn_send_req->Render() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30)}) |
                              ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 3),  // ← ДОБАВЬ ЭТО
                          ftxui::separator(),
                          // ТАБЛИЦА ЗАНИМАЕТ ВСЁ ОСТАВШЕЕСЯ МЕСТО ↓
                          table_component->Render() | ftxui::flex}) |
             ftxui::border;
   });
}

void DataBaseExplorer::RUN() { screen.Loop(main_window); }

static ftxui::ButtonOption CreateRoundedButtonOption() {
   ftxui::ButtonOption option;
   option.transform = [](const ftxui::EntryState& s) {
      auto text_element = ftxui::text(s.label) | ftxui::center;
      if (s.focused) text_element |= ftxui::inverted;
      if (s.active) text_element |= ftxui::bold;
      return ftxui::borderRounded(ftxui::hbox({ftxui::filler(), text_element, ftxui::filler()}));
   };
   return option;
}

static ftxui::Table FormatTable(const Table& table) {}
