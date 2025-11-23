#pragma once
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>

class TerminalOut {
  public:
  TerminalOut();
  void PrintHeader();
  void Render();


  private:
  ftxui::Screen screen;
  ftxui::Element header;
  ftxui::Element layout;
};
