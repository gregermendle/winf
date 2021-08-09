#include "window-info.h"
#include <napi.h>

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "listWindows"),
              Napi::Function::New(env, ListWindows));
  exports.Set(Napi::String::New(env, "windowTitle"),
              Napi::Function::New(env, WindowTitle));
  exports.Set(Napi::String::New(env, "windowRect"),
              Napi::Function::New(env, WindowRect));
  return exports;
}

NODE_API_MODULE(windowInfo, Init)