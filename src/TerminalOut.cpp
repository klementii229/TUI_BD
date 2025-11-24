#include "TerminalOut.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

TerminalOut::TerminalOut() : screen(ftxui::ScreenInteractive::Fullscreen())
{

}


void TerminalOut::Run() {
    screen.Loop(CreateConnectionForm());
}

ftxui::Component TerminalOut::CreateConnectionForm() {
    using namespace ftxui;

    // Поля ввода
    Component host_input = Input(&connection_data.host, "localhost");
    Component port_input = Input(&connection_data.port, "5432");
    Component db_input = Input(&connection_data.database, "");
    Component user_input = Input(&connection_data.username, "");
    Component password_input = Input(&connection_data.password, "");

    // Пароль скрываем
    password_input |= CatchEvent([&](Event event) {
        return false;
    });

    // Выбор типа БД
    int db_type_selected = 0;
    std::vector<std::string> db_types = {"PostgreSQL", "MariaDB"};
    Component db_type_radio = Radiobox(&db_types, &db_type_selected);

    // Кнопка подключения
    Component connect_button = Button("Подключиться", [this, &db_type_selected] {
        connection_data.db_type = (db_type_selected == 0) ? "postgresql" : "mariadb";
        screen.Exit();
    });

    // Собираем форму
    Component container = Container::Vertical({
        host_input,
        port_input,
        db_input,
        user_input,
        password_input,
        db_type_radio,
        connect_button
    });

    // Рендерер с передачей всех компонентов
    Component renderer = Renderer(container, [host_input, port_input, db_input, user_input, password_input, db_type_radio, connect_button] {
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
