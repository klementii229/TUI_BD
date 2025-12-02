#include "DatabaseExplorer.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "ftxui/dom/elements.hpp"

DataBaseExplorer::DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_)
    : screen(ftxui::ScreenInteractive::Fullscreen())
    , conn(std::move(conn_))
{
  // Инициализация компонентов
  req_input = ftxui::Input(&t_request, "Enter your SQL request");

  btn_send_req = ftxui::Button("Отправить запрос:",
                               [this]
                               {
                                 if (!t_request.empty()) {
                                   resultTable = conn->ExecuteQuery(t_request);
                                 }
                               });

  btn_close = ftxui::Button("Выйти", [this] { screen.Exit(); });

  // Собираем все компоненты в контейнер
  form_container =
      ftxui::Container::Vertical({req_input, btn_send_req, btn_close});

  // Добавляем глобальную обработку клавиши 'q'
  form_container |= ftxui::CatchEvent(
      [&](ftxui::Event event)
      {
        if (event.is_character() && event.character() == "q") {
          screen.Exit();
          return true;
        }
        return false;
      });
}

ftxui::Component DataBaseExplorer::CreateForm()
{
  // Возвращаем уже созданный контейнер
  return ftxui::Renderer(
      form_container,
      [&]
      {
        // 1. Создаем элементы для таблицы
        ftxui::Elements table_rows;
        if (!resultTable.empty()) {
          // Заголовок таблицы (если нужно)
          // table_rows.push_back(ftxui::hbox({
          //     ftxui::text("Column1") | ftxui::bold | ftxui::border,
          //     ftxui::text("Column2") | ftxui::bold | ftxui::border
          // }));

          // Данные таблицы
          for (const auto& row : resultTable) {
            ftxui::Elements cells;
            for (const auto& cell : row) {
              cells.push_back(ftxui::text(cell) | ftxui::border);
            }
            table_rows.push_back(ftxui::hbox(std::move(cells)));
          }
        }

        // 2. Собираем полный интерфейс
        return ftxui::vbox({// Панель ввода
                            ftxui::hbox({req_input->Render() | ftxui::flex,
                                         ftxui::filler(),
                                         btn_send_req->Render()})
                                | ftxui::border,

                            ftxui::separator(),

                            // Кнопка выхода
                            ftxui::hbox({ftxui::filler(),
                                         btn_close->Render(),
                                         ftxui::filler()}),

                            ftxui::separator(),

                            // Область результатов
                            resultTable.empty() ? ftxui::center(ftxui::text(
                                                      "Результатов пока нет"))
                                                : ftxui::vbox(table_rows)
                                    | ftxui::border | ftxui::frame})
            | ftxui::border;
      });
}

void DataBaseExplorer::RUN()
{
  screen.Loop(CreateForm());
}
