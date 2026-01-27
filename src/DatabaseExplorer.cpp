#include "DatabaseExplorer.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/color.hpp>

#include "ftxui/dom/elements.hpp"

// table, tab
#include <ftxui/component/component.hpp>

static ftxui::ButtonOption CreateRoundedButtonOption();  // Функция для создания опции кнопки с закругленными краями

DataBaseExplorer::DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn_)
    : screen(ftxui::ScreenInteractive::Fullscreen()), conn(std::move(conn_)) {
    req_input = ftxui::Input(&req_text, "Enter sql request") | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1) |
                ftxui::flex | ftxui::border;
    btn_send_req = ftxui::Button(
        "Enter",
        [this] {
            if (req_text.contains("select") or req_text.contains("SELECT")) {
                db_result = conn->ExecuteQuery(req_text);
            }
        },
        CreateRoundedButtonOption());

    main_container = ftxui::Container::Vertical({req_input, btn_send_req});
    main_window = ftxui::Renderer(main_container, [&]() {
        return ftxui::vbox({ftxui::text("T U I D B") | ftxui::center | ftxui::color(ftxui::Color::Cyan) | ftxui::bold,
            ftxui::separator(),
            ftxui::hbox({req_input->Render(), btn_send_req->Render() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30)}),
            ftxui::separator()});
    }) | ftxui::border;
}

void DataBaseExplorer::RUN() { screen.Loop(main_window); }

static ftxui::ButtonOption CreateRoundedButtonOption() {
    ftxui::ButtonOption option;
    option.transform = [](const ftxui::EntryState& s) {
        // Текст с эффектами
        auto text_element = ftxui::text(s.label) | ftxui::center;

        if (s.focused) {
            text_element |= ftxui::inverted;
        }
        if (s.active) {
            text_element |= ftxui::bold;
        }

        // Создаем рамку отдельно и помещаем текст внутрь
        return ftxui::borderRounded(ftxui::hbox({ftxui::filler(), text_element, ftxui::filler()}));
    };
    return option;
}
