#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"
#include "utils.hpp"
#include <vector>

typedef struct Todo {
  std::string text;
  bool completed;
} Todo;

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "TODO");
  SetWindowMinSize(640, 480);
  SetTargetFPS(30);
  SetExitKey(KEY_NULL);
  InitAudioDevice();

  rlImGuiSetup(true);

  ImGui::GetStyle().FontScaleMain = 2;

  std::string input;
  std::vector<Todo> todos;
  int editing_idx = -1;
  auto submit = [&todos, &input, &editing_idx]() {
    if (input.empty())
      return;
    if (editing_idx >= 0) {
      assert(editing_idx < (int)todos.size() &&
             "Tried to apply edit with invalid i");
      todos[editing_idx].text = input;
      input.clear();
      editing_idx = -1;
      return;
    }
    todos.push_back(Todo{.text = input, .completed = false});
    input.clear();
  };
  auto edit = [&editing_idx, &input, &todos](size_t i) {
    if (editing_idx >= 0) {
      assert(editing_idx < (int)todos.size() &&
             "Tried to cancel with invalid i");
      if (editing_idx != (int)i) {
        goto start_edit;
      }
      input.clear();
      editing_idx = -1;
      return;
    }
  start_edit:
    editing_idx = i;
    assert(editing_idx < (int)todos.size() && "Tried to edit with invalid i");
    input = todos[editing_idx].text;
  };

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GetColor(0x181818FF));

    rlImGuiBegin();

    if (IsWindowResized()) {
      ImGui::SetNextWindowPos(ImVec2(0, 20));
      ImGui::SetNextWindowSize(
          ImVec2(GetRenderWidth(), GetRenderHeight() - 20));
    }
    if (ImGui::Begin("##empty")) {
      ImVec2 avail = ImGui::GetContentRegionAvail();
      ImGui::SetNextItemWidth(avail.x - 60);
      if (InputTextString("##empty", &input,
                          ImGuiInputTextFlags_EnterReturnsTrue)) {
        submit();
        ImGui::SetKeyboardFocusHere(-1);
      }
      ImGui::SameLine();
      if (ImGui::Button(editing_idx >= 0 ? "Confirm" : "Add")) {
        submit();
      }

      avail = ImGui::GetContentRegionAvail();
      ImGui::SetNextItemWidth(avail.x);
      if (ImGui::BeginListBox("##empty")) {
        for (int i = (int)todos.size() - 1; i >= 0; --i) {
          Todo *todo = &todos[i];
          ImGui::LabelText(TextFormat("##empty##foo%d", i), "%s",
                           todo->text.c_str());
          ImGui::SameLine();
          ImGui::Checkbox(TextFormat("##empty##foo%d", i), &todo->completed);
          ImGui::SameLine();
          if (ImGui::Button(TextFormat("Delete##foo%d", i))) {
            if (editing_idx == i) {
              editing_idx = -1;
              input.clear();
            }
            todos.erase(todos.begin() + i);
            --i;
          }
          ImGui::SameLine();
          if (ImGui::Button(TextFormat(
                  "%s##foo%d", editing_idx == i ? "Cancel" : "Edit", i))) {
            edit(i);
          }
        }
      }
      ImGui::EndListBox();
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
