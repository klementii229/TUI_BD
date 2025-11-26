#include "TerminalOut.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

TerminalOut::TerminalOut()
  : screen(ftxui::ScreenInteractive::Fullscreen())
  , db_type_selected(0)
  , db_types({"PostgreSQL", "MariaDB"})
{
  // Инициализируем компоненты в конструкторе
  host_input = ftxui::Input(&connection_data.host, "localhost");
  port_input = ftxui::Input(&connection_data.port, "5432");
  db_input = ftxui::Input(&connection_data.database, "");
  user_input = ftxui::Input(&connection_data.username, "");
  password_input = ftxui::Input(&connection_data.password, "");
  db_type_radio = ftxui::Radiobox(&db_types, &db_type_selected);

  connect_button = ftxui::Button("Подключиться", [this] {
    connection_data.db_type = (db_type_selected == 0) ? "postgresql" : "mariadb";
    screen.Exit();
  });
}

void TerminalOut::Run() {
  screen.Loop(CreateConnectionForm());
}

ftxui::Component TerminalOut::CreateConnectionForm() {
  using namespace ftxui;

  auto container = Container::Vertical({
    host_input,
    port_input,
    db_input,
    user_input,
    password_input,
    db_type_radio,
    connect_button
  });

  auto renderer = Renderer(container, [this] {
    return vbox({
      text(" 🗄️  Подключение к базе данных ") | bold | color(Color::Cyan) | center,
      separator(),
      hbox(text("Хост:     "), host_input->Render()),
      hbox(text("Порт:     "), port_input->Render()),
      hbox(text("База:     "), db_input->Render()),
      hbox(text("Пользователь: "), user_input->Render()),
      hbox(text("Пароль:   "), password_input->Render()),
      separator(),
      text("Тип базы данных:"),
      db_type_radio->Render(),
      separator(),
      connect_button->Render() | center
    }) | border | size(WIDTH, LESS_THAN, 80);
  });

  return renderer;
}
