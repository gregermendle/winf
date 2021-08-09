#include <iostream>
#include <napi.h>
#include <vector>

typedef uint32_t WindowID;
const uint32_t empty_window_id = -1;

// WindowID findWindowByTitle(std::string title);
Napi::Array ListWindows(const Napi::CallbackInfo &info);
// void windowRect(WindowID id);
Napi::Value WindowTitle(const Napi::CallbackInfo &info);
// bool isWindowFullScreen(WindowID id);
// int windowPid(WindowID id);

// bool isFullScreen(std::string titlePredicate);