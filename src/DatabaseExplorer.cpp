#include <algorithm>
#include <exception>

#include "DatabaseExplorer.hpp"
#include "ftxui/dom/elements.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/color.hpp>

DataBaseExplorer::DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_)
    : screen(ftxui::ScreenInteractive::Fullscreen()), conn(std::move(conn_)) {
  result_panel = ftxui::Renderer([this] { return ftxui::text(""); });
  req_input = ftxui::Input(&t_request, "Enter your SQL request");

  btn_send_req = ftxui::Button("Отправить запрос:", [this] {
    if (!t_request.empty()) {
      try {
        has_result = true;
        resultTable = conn->ExecuteQuery(t_request);
      } catch (std::exception e) {
        last_err = e.what();
        has_result = false;
      }
      UpdateResultDisplay();
    }
  });
  btn_close = ftxui::Button("Выйти", [this] { screen.Exit(); });
  btn_prev_page = ftxui::Button("<", [this] {
    if (current_page > 1)
      current_page--;
    UpdateResultDisplay();
  });
  btn_next_page = ftxui::Button(">", [this] {
    if (current_page < total_pages)
      current_page++;
    UpdateResultDisplay();
  });


  menu_cont = ftxui::Container::Vertical(
      {req_input, btn_send_req, result_panel, btn_close});

  form_container = ftxui::Renderer(menu_cont, [this] {
    return ftxui::vbox(
               {ftxui::text("T U I D B") | ftxui::center |
                    ftxui::color(ftxui::Color::Cyan) | ftxui::bold,
                ftxui::separator(),

                ftxui::hbox(req_input->Render(), btn_send_req->Render()),
                ftxui::separator(),

                result_panel->Render(),
                ftxui::separator(),

                btn_close->Render()}) |
           ftxui::border;
  });
}

//ftxui::Component DataBaseExplorer::CreateForm() { return form_container; }

void DataBaseExplorer::RUN() { screen.Loop(form_container); }

void DataBaseExplorer::UpdateResultDisplay() {
  if (has_result == false) {
    // ОШИБКА: показываем только ошибку
    result_panel = ftxui::Renderer(
        [this] { return ftxui::text(last_err) | ftxui::bold | ftxui::center; });
  } else if (resultTable.empty()) {
    // НЕТ РЕЗУЛЬТАТОВ: только текст
    result_panel = ftxui::Renderer([this] {
      return ftxui::text("No results") | ftxui::bold | ftxui::center;
    });
  } else {
    // ЕСТЬ РЕЗУЛЬТАТЫ: таблица + пагинация
    total_rows = resultTable.size();
    total_pages = (total_rows + rows_per_page - 1) / rows_per_page;
    int start_idx = (current_page - 1) * rows_per_page;
    int end_idx = std::min(start_idx + rows_per_page, total_rows);

    // Создаем таблицу
    std::vector<std::vector<std::string>> table_data;
    for (int i = start_idx; i < end_idx; i++) {
      table_data.push_back(resultTable[i]);
    }

    auto table = ftxui::Table(table_data);
    table.SelectAll().Border(ftxui::LIGHT);
    table.SelectAll().Separator(ftxui::LIGHT);

    auto table_element = table.Render();

    // Создаем контейнер для таблицы и пагинации
    auto result_container = ftxui::Container::Vertical({
        ftxui::Renderer([table_element] { return table_element; }),
        ftxui::Container::Horizontal({
            btn_prev_page,
            ftxui::Renderer([this] {
              return ftxui::text(" Page " + std::to_string(current_page) + "/" +
                                 std::to_string(total_pages)) |
                     ftxui::vcenter;
            }),
            btn_next_page,
        }) | ftxui::center,
    });

    result_panel = result_container;
  }

  // Принудительно обновляем экран
  screen.PostEvent(ftxui::Event::Custom);
}
