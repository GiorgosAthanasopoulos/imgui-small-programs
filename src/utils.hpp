#pragma once

#include <imgui.h>
#include <string>

static inline int InputTextCallback(ImGuiInputTextCallbackData *data) {
  if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
    auto *str = (std::string *)data->UserData;
    str->resize(data->BufTextLen);
    data->Buf =
        str->data(); // safe in C++17+, equivalent to &(*str)[0] in C++11
  }
  return 0;
}

inline bool InputTextString(const char *label, std::string *str,
                            ImGuiInputTextFlags flags = 0) {
  // Always add resize flag
  flags |= ImGuiInputTextFlags_CallbackResize;

  // Make sure string has capacity for null terminator
  return ImGui::InputText(
      label,
      str->empty() ? (char *)"" : &(*str)[0], // works for C++11+
      str->capacity() + 1, flags, InputTextCallback, (void *)str);
}

inline ImVec2 operator+(ImVec2 a, ImVec2 b) {
  return ImVec2(a.x + b.x, a.y + b.y);
}

inline ImVec2 operator+(ImVec2 a, int b) { return ImVec2(a.x + b, a.y + b); }

inline ImVec2 operator-(ImVec2 a, ImVec2 b) {
  return ImVec2(a.x - b.x, a.y - b.y);
}

inline ImVec2 operator/(ImVec2 a, int b) {
  if (b == 0)
    return ImVec2(0, 0);
  return ImVec2(a.x / b, a.y / b);
}
