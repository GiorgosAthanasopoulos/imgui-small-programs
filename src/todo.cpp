#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"

// TODO: implement TODO app

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(800, 600, "TODO");
  SetWindowMinSize(640, 480);
  SetTargetFPS(30);
  SetExitKey(KEY_NULL);
  InitAudioDevice();

  rlImGuiSetup(false);

  ImGui::GetStyle().FontScaleMain = 2;

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
