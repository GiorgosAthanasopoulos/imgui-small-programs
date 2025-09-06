#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"

#include <cctype>
#include <iomanip>
#include <stdexcept>
#include <string>

// TODO: add keypad numbers to actions
// TODO: fix key pressed/chord precedence (the chord is always last)

class Parser {
  const char *str;

public:
  Parser(const std::string &s) : str(s.c_str()) {}

  double parse() {
    double result = expr();
    if (*str)
      throw std::runtime_error("Unexpected input");
    return result;
  }

private:
  double expr() {
    double result = term();
    while (*str == '+' || *str == '-') {
      char op = *str++;
      double t = term();
      if (op == '+')
        result += t;
      else
        result -= t;
    }
    return result;
  }

  double term() {
    double result = factor();
    while (*str == '*' || *str == '/') {
      char op = *str++;
      double f = factor();
      if (op == '*')
        result *= f;
      else
        result /= f;
    }
    return result;
  }

  double factor() {
    while (isspace(*str))
      str++; // skip spaces

    if (*str == '(') {
      str++;
      double result = expr();
      if (*str != ')')
        throw std::runtime_error("Expected ')'");
      str++;
      return result;
    }

    // parse number
    const char *start = str;
    while (isdigit(*str) || *str == '.')
      str++;
    if (start == str)
      throw std::runtime_error("Expected number");
    return std::stod(std::string(start, str));
  }
};

void ToggleSign(std::string &expr) {
  if (expr.empty())
    return;

  // Find start of last number
  size_t i = expr.size() - 1;
  while (i > 0 && (isdigit(expr[i]) || expr[i] == '.'))
    i--;

  // Check if the last number is already negative
  if (i > 0 && expr[i] == '-' && (i == 0 || !isdigit(expr[i - 1]))) {
    // Remove the negative sign
    expr.erase(i, 1);
  } else {
    // Insert a minus sign before the last number
    if (i == 0 && expr[i] == '-') {
      // already negative number at start, do nothing
      return;
    }
    expr.insert(i + 1, "-");
  }
}

void ApplyPercent(std::string &display) {
  if (display.empty())
    return;

  try {
    double value = Parser(display).parse(); // evaluate expression
    value *= 100.0;                         // convert to percent

    // Convert back to string with trimming
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(10) << value;
    std::string str = ss.str();

    // Trim trailing zeros
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    if (str.back() == '.')
      str.pop_back();

    display = str;
  } catch (...) {
    display = "Error"; // invalid expression
  }
}

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "Calculator");
  SetWindowMinSize(640, 480);
  SetTargetFPS(30);
  SetExitKey(KEY_NULL);
  InitAudioDevice();

  rlImGuiSetup(false);

  ImGui::GetStyle().FontScaleMain = 2;

  std::string display;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);

    rlImGuiBegin();

    if (IsWindowResized()) {
      ImGui::SetNextWindowPos(ImVec2(0, 20));
      ImGui::SetNextWindowSize(
          ImVec2(GetRenderWidth(), GetRenderHeight() - 20));
    }
    if (ImGui::Begin("##empty")) {
      ImVec2 avail = ImGui::GetContentRegionAvail();
      ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(1.0f, 0.5f));
      ImGui::Button(strcmp(display.c_str(), "") == 0 ? "##empty"
                                                     : display.c_str(),
                    ImVec2(avail.x, 0));
      ImGui::PopStyleVar();

      for (int i = 0; i < 2; ++i) {
        ImGui::Spacing();
      }

      avail = ImGui::GetContentRegionAvail();
      float table_height = avail.y - 30;
      if (ImGui::BeginTable("##empty", 4, 0, ImVec2(avail.x, table_height))) {
        float button_height = table_height / 5; // 5 == rows

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button("%", ImVec2(-1, button_height)) ||
            ImGui::IsKeyChordPressed(ImGuiKey_LeftShift | ImGuiKey_5)) {
          ApplyPercent(display);
        }

        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("÷", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_Slash)) {
          display += "/";
        }

        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button("x", ImVec2(-1, button_height)) ||
            ImGui::IsKeyChordPressed(ImGuiKey_LeftShift | ImGuiKey_8)) {
          display += "*";
        }

        ImGui::TableSetColumnIndex(3);
        if (ImGui::Button("-", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_Minus)) {
          display += "-";
        }

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button("7", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_7)) {
          display += "7";
        }

        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("8", ImVec2(-1, button_height)) ||
            (ImGui::IsKeyPressed(ImGuiKey_8) &&
             !ImGui::IsKeyPressed(ImGuiKey_LeftShift))) {
          display += "8";
        }

        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button("9", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_9)) {
          display += "9";
        }

        ImGui::TableSetColumnIndex(3);
        if (ImGui::Button("+", ImVec2(-1, button_height)) ||
            ImGui::IsKeyChordPressed(ImGuiKey_LeftShift | ImGuiKey_Minus)) {
          display += "+";
        }

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button("4", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_4)) {
          display += "4";
        }

        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("5", ImVec2(-1, button_height)) ||
            (ImGui::IsKeyPressed(ImGuiKey_5) &&
             !ImGui::IsKeyPressed(ImGuiKey_LeftShift))) {
          display += "5";
        }

        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button("6", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_6)) {
          display += "6";
        }

        ImGui::TableSetColumnIndex(3);
        if (ImGui::Button("C", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_C)) {
          display = "";
        }

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button("1", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_1)) {
          display += "1";
        }

        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("2", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_2)) {
          display += "2";
        }

        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button("3", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_3)) {
          display += "3";
        }

        ImGui::TableSetColumnIndex(3);
        if (ImGui::Button("AC", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_A)) {
          display = "";
        }

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button("0", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_0)) {
          display += "0";
        }

        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("+/-", ImVec2(-1, button_height))) {
          ToggleSign(display);
        }

        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button(".", ImVec2(-1, button_height)) ||
            ImGui::IsKeyPressed(ImGuiKey_Period)) {
          display += ".";
        }

        ImGui::TableSetColumnIndex(3);
        if (ImGui::Button("=", ImVec2(-1, button_height)) ||
            (ImGui::IsKeyPressed(ImGuiKey_Equal) &&
             !ImGui::IsKeyPressed(ImGuiKey_LeftShift)) ||
            ImGui::IsKeyPressed(ImGuiKey_Enter)) {
          display = std::to_string(Parser(display).parse());
        }

        if (ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
          if (!display.empty())
            display.pop_back();
        }
      }
      ImGui::EndTable();
    }
    ImGui::End();

    rlImGuiEnd();

    DrawFPS(0, 0);
    EndDrawing();
  }

  rlImGuiShutdown();

  CloseAudioDevice();
  CloseWindow();
  return 0;
}
