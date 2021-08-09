#include <iostream>
#include <napi.h>
#include <vector>

typedef uint32_t WindowID;
const uint32_t empty_window_id = -1;

Napi::Array ListWindows(const Napi::CallbackInfo &info);
Napi::Value WindowRect(const Napi::CallbackInfo &info);
Napi::Value WindowTitle(const Napi::CallbackInfo &info);
Napi::Boolean IsWindowFullScreen(const Napi::CallbackInfo &info);