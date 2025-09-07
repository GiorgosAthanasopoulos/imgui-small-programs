#include "utils.hpp"

#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"

#include <cmath>
#include <cstdio>
#include <vector>

typedef struct TimerState {
  float total_seconds = .0f;
  float remaining_seconds = .0f;

  int hours_input = 0;
  int minutes_input = 1;
  int seconds_input = 0;

  bool paused = false;
  bool stopped = true;
} TimerState;

typedef struct StopwatchState {
  float elapsed = .0f;
  float lap = .0f;
  std::vector<std::pair<float, float>> laps;

  bool paused = false;
  bool stopped = true;
} StopwatchState;

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(640, 480, "Timer");
  SetWindowMinSize(640, 480);
  SetTargetFPS(30);
  SetExitKey(KEY_NULL);
  InitAudioDevice();
  Sound beep = LoadSound("assets/sounds/beep.ogg");

  rlImGuiSetup(true);

  ImGui::GetStyle().FontScaleMain = 2;

  TimerState timer;
  StopwatchState stopwatch;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    bool timer_started = !timer.paused && !timer.stopped;
    if (timer_started) {
      timer.remaining_seconds -= dt;
      if (timer.remaining_seconds < 0.0f) {
        PlaySound(beep);
        timer.remaining_seconds = 0.0f;
        timer.stopped = true;
      }
    }

    bool stopwatch_started = !stopwatch.paused && !stopwatch.stopped;
    if (stopwatch_started) {
      stopwatch.elapsed += dt;
      stopwatch.lap += dt;
    }

    BeginDrawing();
    ClearBackground(GetColor(0x181818FF));

    rlImGuiBegin();

    if (IsWindowResized()) {
      ImGui::SetNextWindowPos(ImVec2(0, 20));
      ImGui::SetNextWindowSize(
          ImVec2(GetRenderWidth(), GetRenderHeight() - 20));
    }
    if (ImGui::Begin("##empty")) {
      if (ImGui::BeginTabBar("##empty")) {
        if (ImGui::BeginTabItem("Timer")) {
          ImGui::DragInt("Hours", &timer.hours_input, .05f, 0, 99);
          ImGui::DragInt("Minutes", &timer.minutes_input, .05f, 0, 59);
          ImGui::DragInt("Seconds", &timer.seconds_input, .05f, 0, 59);

          if (ImGui::Button(timer_started ? "Pause" : "Start")) {
            if (timer_started) {
              timer.paused = true;
            } else if (timer.paused) {
              timer.paused = false;
            } else {
              timer.total_seconds = timer.remaining_seconds =
                  timer.hours_input * 3600 + timer.minutes_input * 60 +
                  timer.seconds_input;
              timer.stopped = false;
            }
          }
          ImGui::SameLine();
          if (ImGui::Button("Reset")) {
            timer.paused = false;
            timer.stopped = true;
            timer.remaining_seconds = timer.total_seconds = 0;
          }

          ImDrawList *draw_list = ImGui::GetWindowDrawList();
          float radius = 100.0f;
          // ImVec2 center = ImGui::GetCursorScreenPos() +
          //                 ImVec2(20 + radius, 20 + radius); // circle
          //                 center
          ImVec2 center = ImVec2(GetRenderWidth(),
                                 GetRenderHeight() + ImGui::GetCursorPosY()) /
                          2;
          float progress = timer.total_seconds != 0
                               ? timer.remaining_seconds / timer.total_seconds
                               : 0; // 0..1

          // Draw background circle
          draw_list->AddCircle(center, radius, IM_COL32(100, 100, 100, 255), 0,
                               4);

          // Draw progress arc
          draw_list->PathClear();
          int num_segments = 64;
          for (int i = 0; i <= num_segments * progress; i++) {
            float angle = (i / (float)num_segments) * 2.0f * PI - PI / 2;
            draw_list->PathLineTo(ImVec2(center.x + cosf(angle) * radius,
                                         center.y + sinf(angle) * radius));
          }
          draw_list->PathStroke(timer_started  ? IM_COL32(0, 200, 0, 255)
                                : timer.paused ? IM_COL32(200, 200, 200, 255)
                                               : IM_COL32(0, 0, 0, 0),
                                false, 6.0f);

          int hours = timer.remaining_seconds / 60 / 60;
          int minutes = ((int)timer.remaining_seconds / 60) % 60;
          int seconds = (int)timer.remaining_seconds % 60;

          // Draw time text
          char buf[256];
          sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);
          ImVec2 text_size = ImGui::CalcTextSize(buf);
          center.y -= text_size.y / 2;
          draw_list->AddText(center - text_size / 2,
                             IM_COL32(255, 255, 255, 255), buf);
          sprintf(buf, "%s",
                  timer_started  ? "Counting..."
                  : timer.paused ? "Paused"
                                 : "Stopped");
          draw_list->AddText(
              center - ImVec2(ImGui::CalcTextSize(buf).x / 2, -text_size.y),
              IM_COL32(255, 255, 255, 255), buf);

          ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Stopwatch")) {
          int total_centis = (int)(stopwatch.elapsed * 100 + 0.5); // rounding
          int total_minutes = total_centis / 6000; // 100 cs * 60 sec
          int total_seconds = (total_centis / 100) % 60;
          int total_centiseconds = total_centis % 100;
          ImGui::LabelText("##empty1", "%s",
                           TextFormat("%02d:%02d.%02d", total_minutes,
                                      total_seconds, total_centiseconds));

          int lap_centis =
              (int)(stopwatch.lap * 100 + 0.5); // round to nearest centisecond
          int lap_minutes = lap_centis / 6000;  // 100 cs * 60 sec
          int lap_seconds = (lap_centis / 100) % 60;
          int lap_centiseconds = lap_centis % 100;
          ImGui::LabelText("##empty2", "%s",
                           TextFormat("%02d:%02d.%02d", lap_minutes,
                                      lap_seconds, lap_centiseconds));

          for (int i = 0; i < 3; ++i) {
            ImGui::Spacing();
          }

          if (ImGui::BeginListBox("##empty")) {
            for (int i = 0; i < (int)stopwatch.laps.size(); ++i) {
              int lap_minutes = stopwatch.laps[i].first / 60;
              int lap_seconds = (int)stopwatch.laps[i].first % 60;
              int lap_centiseconds =
                  (int)(fmod(stopwatch.laps[i].first, 1.0) * 100 + 0.5);

              int lap_overall_minutes = stopwatch.laps[i].second / 60;
              int lap_overall_seconds = (int)stopwatch.laps[i].second % 60;
              int lap_overall_centiseconds =
                  (int)((stopwatch.laps[i].second -
                         (int)stopwatch.laps[i].second) *
                        100);

              ImGui::LabelText(
                  TextFormat("##empty1%d", i), "%s",
                  TextFormat("%02d\t%02d:%02d.%02d\t%02d:%02d.%02d", i + 1,
                             lap_minutes, lap_seconds, lap_centiseconds,
                             lap_overall_minutes, lap_overall_seconds,
                             lap_overall_centiseconds));
            }
            ImGui::EndListBox();
          }

          if (ImGui::Button(stopwatch.paused ? "Reset" : "Lap")) {
            if (stopwatch.paused) {
              stopwatch.stopped = true;
              stopwatch.paused = false;
              stopwatch.elapsed = stopwatch.lap = .0f;
              stopwatch.laps.clear();
            } else {
              if (stopwatch_started) {
                PlaySound(beep);
                stopwatch.laps.push_back(
                    std::pair<float, float>(stopwatch.lap, stopwatch.elapsed));
                stopwatch.lap = .0f;
              }
            }
          }
          ImGui::SameLine();
          if (ImGui::Button(stopwatch.stopped  ? "Start"
                            : stopwatch.paused ? "Resume"
                                               : "Stop")) {
            if (stopwatch.stopped) {
              stopwatch.stopped = false;
            } else if (stopwatch.paused) {
              stopwatch.paused = false;
            } else {
              stopwatch.paused = true;
            }
          }

          ImGui::EndTabItem();
        }
      }
      ImGui::EndTabBar();
    }
    ImGui::End();

    rlImGuiEnd();

    DrawFPS(0, 0);
    EndDrawing();
  }

  rlImGuiShutdown();

  UnloadSound(beep);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
