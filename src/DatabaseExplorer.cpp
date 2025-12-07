#include <algorithm>
#include <exception>

#include "DatabaseExplorer.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>

#include "ftxui/dom/elements.hpp"

DataBaseExplorer::DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_)
    : screen(ftxui::ScreenInteractive::Fullscreen())
    , conn(std::move(conn_))
{
  result_panel = ftxui::Renderer([this] { return ftxui::text(""); });
  req_input = ftxui::Input(&t_request, "Enter your SQL request");

  btn_send_req = ftxui::Button("Отправить запрос:",
                               [this]
                               {
                                 if (!t_request.empty()) {
                                   try {
                                     has_result = true;
                                     resultTable =
                                         conn->ExecuteQuery(t_request);
                                   } catch (std::exception e) {
                                     last_err = e.what();
                                     has_result = false;
                                   }
                                   UpdateResultDisplay();
                                 }
                               });
  btn_close = ftxui::Button("Выйти", [this] { screen.Exit(); });
  btn_prev_page = ftxui::Button("<",
                                [this]
                                {
                                  if (current_page > 1) current_page--;
                                  UpdateResultDisplay();
                                });
  btn_next_page = ftxui::Button(">",
                                [this]
                                {
                                  if (current_page < total_pages) current_page++;
                                  UpdateResultDisplay();
                                });

  auto container = ftxui::Container::Vertical({req_input,
                                               btn_send_req,
                                               result_panel,
                                               btn_prev_page,
                                               btn_next_page,
                                               btn_close});

  form_container =
      ftxui::Renderer(
          container,
          [this]
          {
            return ftxui::vbox(
                {ftxui::text("T U I D B") | ftxui::center
                     | ftxui::color(ftxui::Color::Cyan) | ftxui::bold,
                 ftxui::separator(),

                 ftxui::hbox(req_input->Render(), btn_send_req->Render()),
                 ftxui::separator(),

                 result_panel->Render(),
                 ftxui::separator(),

                 ftxui::hbox(btn_prev_page->Render(), btn_next_page->Render()),
                 ftxui::separator(),

                 btn_close->Render()});
          })
      | ftxui::border;
}

ftxui::Component DataBaseExplorer::CreateForm()
{
  return form_container;
}

void DataBaseExplorer::RUN()
{
  screen.Loop(CreateForm());
}

void DataBaseExplorer::UpdateResultDisplay()
{

  if (has_result == false) {
    result_panel = ftxui::Renderer(
        [this] { return ftxui::text(last_err) | ftxui::bold | ftxui::center; });
  } else if (resultTable.empty()) {
    result_panel = ftxui::Renderer(
        [this]
        { return ftxui::text("No results") | ftxui::bold | ftxui::center; });

  }

  else {
    total_rows = resultTable.size();
    total_pages = (total_rows + rows_per_page - 1) / rows_per_page;
    int start_idx = (current_page - 1) * rows_per_page;

    int end_idx = std::min(start_idx + rows_per_page, total_rows);

    std::vector<ftxui::Element> headers;
    for (const auto& cell : resultTable[0]) {
      headers.push_back(ftxui::text(cell) | ftxui::bold);
    }
    std::vector<ftxui::Element> rows;

    // заголовки
    rows.push_back(ftxui::hbox(std::move(headers)));
    rows.push_back(ftxui::separator());

    // данные
    for (int i = start_idx; i < end_idx; i++) {
      std::vector<ftxui::Element> row_cells;
      for (const auto& cell : resultTable[i]) {
        row_cells.push_back(ftxui::text(cell));
      }
      rows.push_back(ftxui::hbox(std::move(row_cells)));
    }

    // информация о странице
    rows.push_back(ftxui::separator());
    rows.push_back(ftxui::text("Page " + std::to_string(current_page) + "/"
                               + std::to_string(total_pages))
                   | ftxui::center);

    result_panel = ftxui::Renderer(
        [rows = std::move(rows)]
        { return ftxui::vbox(std::move(rows)) | ftxui::border; });
  }
}
