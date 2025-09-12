#include "utils.hpp"

#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

std::string GetCurrentDateTime() {
  auto now = std::chrono::system_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(now);

  std::tm tm{};
#ifdef _WIN32
  localtime_s(&tm, &t);
#else
  localtime_r(&t, &tm);
#endif

  std::ostringstream oss;
  oss << std::put_time(&tm, "%I:%M %p %m/%d/%Y");
  return oss.str();
}

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "Untitled - Notepad");
  SetWindowMinSize(640, 480);
  SetTargetFPS(30);
  SetExitKey(KEY_NULL);
  InitAudioDevice();

  rlImGuiSetup(true);

  ImGui::GetStyle().FontScaleMain = 2;

  std::string text;
  bool show_status = true;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(0x181818FF));

    rlImGuiBegin();

    if (IsWindowResized()) {
      ImGui::SetNextWindowPos(ImVec2(0, 20));
      ImGui::SetNextWindowSize(
          ImVec2(GetRenderWidth(), GetRenderHeight() - 20));
    }
    if (ImGui::Begin("##empty", nullptr, ImGuiWindowFlags_MenuBar)) {
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          if (ImGui::MenuItem("New", "Ctrl+N")) {
            // TODO
          }
          if (ImGui::MenuItem("Open...", "Ctrl+O")) {
            // TODO
          }
          if (ImGui::MenuItem("Save", "Ctrl+S")) {
            // TODO
          }
          if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S")) {
            // TODO
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Print...", "Ctrl+P")) {
            // nop
          }
          if (ImGui::MenuItem("Page Setup...", "Ctrl+Alt+P")) {
            // nop
          }
          if (ImGui::MenuItem("Printer Setup...", "Ctrl+Shift+P")) {
            // nop
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Exit", "Ctrl+Q")) {
            // TODO: check if file is saved
            break;
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
          if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
            // nop
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Cut", "Ctrl+X")) {
            // nop
          }
          if (ImGui::MenuItem("Copy", "Ctrl+C")) {
            // nop
          }
          if (ImGui::MenuItem("Paste", "Ctrl+V")) {
            // nop
          }
          if (ImGui::MenuItem("Delete", "Del")) {
            // nop
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Select all", "Ctrl+A")) {
            // nop
          }
          if (ImGui::MenuItem("Time/Date")) {
            text += GetCurrentDateTime();
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Wrap long lines")) {
            // nop
          }
          if (ImGui::MenuItem("Font...")) {
            // nop
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Search")) {
          if (ImGui::MenuItem("Search...", "Ctrl+F")) {
            // TODO
          }
          if (ImGui::MenuItem("Search next", "F3")) {
            // TODO
          }
          if (ImGui::MenuItem("Replace...", "Ctrl+H")) {
            // TODO
          }
          if (ImGui::MenuItem("Go To...", "Ctrl+G")) {
            // TODO
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
          if (ImGui::MenuItem("Status Bar", nullptr, show_status)) {
            show_status = !show_status;
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
          if (ImGui::MenuItem("Contents")) {
            // nop
          }
          if (ImGui::MenuItem("About Notepad")) {
            // TODO
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }

      ImVec2 avail = ImGui::GetContentRegionAvail();
      avail.y -= show_status ? 30 : 0;
      InputTextMultilineString("##empty", &text, avail);

      if (show_status) {
        ImGui::BeginChild("##status", ImVec2(0, 30), false,
                          ImGuiWindowFlags_NoScrollbar |
                              ImGuiWindowFlags_NoSavedSettings |
                              ImGuiWindowFlags_NoTitleBar);
        long lines = std::count(text.begin(), text.end(), '\n') + 1;
        ImGui::Text("%ld lines, %ld characters", lines,
                    text.size() - lines + 1);
        ImGui::EndChild();
      }
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
