#include "TerminalOut.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

TerminalOut::TerminalOut() : screen(ftxui::Screen::Create(ftxui::Dimension::Full()))
{

}

void TerminalOut::Render()
{

  auto content = ftxui::text(" TUI DB ") | ftxui::center | ftxui::border;

  screen = ftxui::Screen::Create(ftxui::Dimension::Fit(content));
  ftxui::Render(screen, content);
  screen.Print();
}
