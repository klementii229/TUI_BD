#include "DatabaseExplorer.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "ftxui/dom/elements.hpp"

DataBaseExplorer::DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_)
    : screen(ftxui::ScreenInteractive::Fullscreen())
    , conn(std::move(conn_))
{
  req_input = ftxui::Input(&t_request, "Enter your SQL request");
  btn_send_req = ftxui::Button("Отправить запрос:",
                               [this]
                               {
                                 if (!t_request.empty()) {
                                   resultTable = conn->ExecuteQuery(t_request);
                                 }
                               });
  btn_close = ftxui::Button("Выйти", [this] { screen.Exit(); });

  form_container =
      ftxui::Container::Vertical({req_input, btn_send_req, btn_close});
}

ftxui::Component DataBaseExplorer::CreateForm()
{
  //ftxui::Component main_comp = ftxui::v

  return ftxui::Component(form_container);

}

void DataBaseExplorer::RUN()
{
  screen.Loop(CreateForm());
}
