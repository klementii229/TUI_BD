#include "DatabaseExplorer.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "ftxui/dom/elements.hpp"

DataBaseExplorer::DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_)
    : screen(ftxui::ScreenInteractive::Fullscreen())
    , conn(std::move(conn_))
{
  req_imput = ftxui::Input(&t_request, "Enter your sql requests");
  btn_send_req = ftxui::Button("Отправить запрос:",
                               [this]
                               {
                                 resultTable = conn->ExecuteQuery(t_request);
                               });
}

ftxui::Component DataBaseExplorer::CreateForm()
{
  auto container = ftxui::Container::Vertical({req_imput, btn_send_req});

  return ftxui::Renderer(
      container,
      [&]
      {
        // 1. Создаем элементы для таблицы
        ftxui::Elements table_rows;
        for (const auto& row : resultTable) {
          ftxui::Elements cells;
          for (const auto& cell : row) {
            cells.push_back(ftxui::text(cell) | ftxui::border);
          }
          table_rows.push_back(ftxui::hbox(cells));
        }

        // 2. Собираем полный интерфейс
        return ftxui::vbox(
            {// Панель ввода
             ftxui::hbox(
                 {req_imput->Render() | ftxui::flex, btn_send_req->Render()})
                 | ftxui::border,

             // Разделитель
             ftxui::separator(),

             // Таблица с результатами (если есть)
             resultTable.empty()
                 ? ftxui::text("Результатов пока нет")
                 : ftxui::vbox(table_rows) | ftxui::border | ftxui::frame});
      });
}

void DataBaseExplorer::RUN()
{
  screen.Loop(CreateForm());
}
