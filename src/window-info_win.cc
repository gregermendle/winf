#include "rect.h"
#include "window-info.h"
#include <iostream>
#include <napi.h>
#include <windows.h>
#include <winuser.h>

/**
 * NOTE:
 * 
 * 64 and 32 bit Windows systems use 32 bit window handles for interoperability. 
 * So We can convert HWND to a 32 bit long integer. 
 * https://docs.microsoft.com/en-us/windows/win32/winprog64/interprocess-communication?redirectedfrom=MSDN
 */

HWND GetWindowID(const Napi::CallbackInfo &info, std::string methodName)
{
  Napi::Env env = info.Env();
  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, methodName + " - Expects a single argument.")
        .ThrowAsJavaScriptException();
    return NULL;
  }

  if (!info[0].IsNumber())
  {
    Napi::TypeError::New(env, methodName + " - Expects a number as its first argument.")
        .ThrowAsJavaScriptException();
    return NULL;
  }

  WindowID id = info[0].As<Napi::Number>().Uint32Value();
  HWND hwnd = reinterpret_cast<HWND>(id);
  return hwnd;
}

Napi::Array ListWindows(const Napi::CallbackInfo &info)
{

  Napi::Env env = info.Env();
  Napi::Array result = Napi::Array::New(env);

  for (
      auto [i, hwnd] = std::tuple{0, GetTopWindow(NULL)};
      hwnd != NULL;
      hwnd = GetNextWindow(hwnd, GW_HWNDNEXT), i++)
  {
    result[i] = Napi::Number::New(env, (uint32_t)(uintptr_t)hwnd);
  }

  return result;
}

Napi::Value WindowTitle(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  HWND hwnd = GetWindowID(info, "WindowTitle");

  if (hwnd == NULL)
  {
    return env.Null();
  }

  int length = GetWindowTextLength(hwnd);
  if (length == 0)
  {
    return env.Null();
  }

  char *tempTitle = new char[length + 1];
  GetWindowText(hwnd, tempTitle, length + 1);
  Napi::String result = Napi::String::New(env, tempTitle);
  return result;
}

Napi::Value WindowRect(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  HWND hwnd = GetWindowID(info, "WindowTitle");

  if (hwnd == NULL)
  {
    return env.Null();
  }

  RECT windowRect;
  GetWindowRect(hwnd, &windowRect);
  Napi::Object result = CreateRect(
      env,
      (double)windowRect.left,
      (double)windowRect.top,
      (double)(windowRect.bottom - windowRect.top),
      (double)(windowRect.right - windowRect.left));

  return result;
}

Napi::Boolean isWindowFullScreen(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  HWND hwnd = GetWindowID(info, "WindowTitle");

  if (hwnd == NULL)
  {
    return Napi::Boolean::New(env, false);
  }

  MONITORINFO monitorInfo = {0};
  monitorInfo.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

  RECT windowRect;
  GetWindowRect(hwnd, &windowRect);
  Napi::Boolean result = Napi::Boolean::New(env, windowRect.left == monitorInfo.rcMonitor.left &&
                                                     windowRect.right == monitorInfo.rcMonitor.right &&
                                                     windowRect.top == monitorInfo.rcMonitor.top &&
                                                     windowRect.bottom == monitorInfo.rcMonitor.bottom);
  return result;
}
