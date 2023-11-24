#include "rect.h"
#include "window-info.h"
#include <CoreGraphics/CoreGraphics.h>

CGRect MainDisplayBounds()
{
  CGDirectDisplayID mainDisp = CGMainDisplayID();
  CGRect mainDispRect = CGDisplayBounds(mainDisp);
  return mainDispRect;
}

std::string ToString(CFStringRef str)
{
  if (str == NULL)
    return "";
  CFIndex len = CFStringGetLength(str);
  if (len == 0)
    return "";
  char *result = new char[len + 1];
  CFStringGetCString(str, result, len + 1, kCFStringEncodingUTF8);
  std::string strResult(result);
  return strResult;
}

CGWindowID GetWindowID(const Napi::CallbackInfo &info, std::string methodName)
{
  Napi::Env env = info.Env();
  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, methodName + " - Expects a single argument.")
        .ThrowAsJavaScriptException();
    return empty_window_id;
  }

  if (!info[0].IsNumber())
  {
    Napi::TypeError::New(env, methodName + " - Expects a number as its first argument.")
        .ThrowAsJavaScriptException();
    return empty_window_id;
  }

  WindowID id = info[0].As<Napi::Number>().Uint32Value();
  return id;
}

Napi::Value WindowTitle(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  WindowID id = GetWindowID(info, "WindowTitle");

  if (id == empty_window_id)
  {
    return env.Null();
  }

  WindowID idArray_c[1] = {id};
  CFArrayRef idArray = CFArrayCreate(NULL, (const void **)idArray_c, 1, NULL);
  CFArrayRef windows = CGWindowListCreateDescriptionFromArray(idArray);
  CFDictionaryRef window = (CFDictionaryRef)CFArrayGetValueAtIndex(windows, 0);
  std::string ownerName = ToString(
      (CFStringRef)CFDictionaryGetValue(window, kCGWindowOwnerName));
  Napi::String result;

  if (ownerName.empty())
  {
    std::string name = ToString(
        (CFStringRef)CFDictionaryGetValue(window, kCGWindowName));
    result = Napi::String::New(env, name);
  }

  result = Napi::String::New(env, ownerName);
  CFRelease(idArray);
  CFRelease(windows);
  return result;
}

Napi::Array ListWindows(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  CFArrayRef windowList = CGWindowListCopyWindowInfo(
      kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements,
      kCGNullWindowID);
  CFIndex count = CFArrayGetCount(windowList);
  Napi::Array result = Napi::Array::New(env);

  for (int i = 0, resultIdx = 0; i < count; i++)
  {
    CFDictionaryRef window = (CFDictionaryRef)CFArrayGetValueAtIndex(windowList, i);
    if (!window) {
      continue;
    }
    
    CFNumberRef winId = (CFNumberRef)CFDictionaryGetValue(window, kCGWindowNumber);
    CFNumberRef windowLayer = (CFNumberRef)
        CFDictionaryGetValue(window, kCGWindowLayer);
    if (!windowLayer)
    {
      continue;
    }

    int layer;
    if (!CFNumberGetValue(windowLayer, kCFNumberIntType, &layer) || layer != 0)
    {
      continue;
    }

    CGWindowID id;
    if (!CFNumberGetValue(winId, kCFNumberIntType, &id))
    {
      continue;
    }

    result[resultIdx] = Napi::Number::New(env, id);
    resultIdx++;
  }

  return result;
}

Napi::Value WindowRect(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  WindowID id = GetWindowID(info, "WindowRect");

  if (id == empty_window_id)
  {
    return env.Null();
  }

  CGRect rect = CGRectMake(0, 0, 100, 100);
  WindowID idCArray[1] = {id};
  CFArrayRef idArray = CFArrayCreate(NULL, (const void **)idCArray, 1, NULL);
  CFArrayRef descArray = CGWindowListCreateDescriptionFromArray(idArray);
  CFDictionaryRef description = (CFDictionaryRef)CFArrayGetValueAtIndex(descArray, 0);

  if (CFDictionaryContainsKey(description, kCGWindowBounds))
  {
    CFDictionaryRef bounds = (CFDictionaryRef)CFDictionaryGetValue(description, kCGWindowBounds);
    if (bounds)
    {
      CGRectMakeWithDictionaryRepresentation(bounds, &rect);
      Napi::Object result = CreateRect(
          env,
          CGRectGetMinX(rect),
          CGRectGetMinY(rect),
          CGRectGetHeight(rect),
          CGRectGetWidth(rect));
      CFRelease(descArray);
      CFRelease(idArray);
      return result;
    }
  }

  CFRelease(descArray);
  CFRelease(idArray);
  return env.Null();
}

Napi::Boolean IsWindowFullScreen(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  WindowID id = GetWindowID(info, "IsWindowFullScreen");

  if (id == empty_window_id)
  {
    return Napi::Boolean::New(env, false);
  }

  CGRect dispRect = MainDisplayBounds();
  Napi::Value windowRectVal = WindowRect(info);

  if (windowRectVal.IsNull())
  {
    return Napi::Boolean::New(env, false);
  }

  Napi::Object windowRectObj = windowRectVal.ToObject();
  CGRect windowRect = CGRectMake(
      windowRectObj.Get(Napi::String::New(env, "x")).ToNumber().DoubleValue(),
      windowRectObj.Get(Napi::String::New(env, "y")).ToNumber().DoubleValue(),
      windowRectObj.Get(Napi::String::New(env, "width")).ToNumber().DoubleValue(),
      windowRectObj.Get(Napi::String::New(env, "height")).ToNumber().DoubleValue());

  bool isFullScreen = CGRectEqualToRect(dispRect, windowRect);
  return Napi::Boolean::New(env, isFullScreen);
}
