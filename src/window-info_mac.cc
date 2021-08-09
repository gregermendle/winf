#include "rect.h"
#include "window-info.h"
#include <CoreGraphics/CoreGraphics.h>

// CFArrayRef createWindowDescription(CGWindowID id)
// {
//   CGWindowID idCArray[1] = {id};
//   CFArrayRef idArray = CFArrayCreate(NULL, (const void **)idCArray, 1, NULL);
//   CFArrayRef result = CGWindowListCreateDescriptionFromArray(idArray);
//   CFRelease(idArray);
//   return result;
// }

// std::string windowOwnerName(CGWindowID id)
// {
//   CFArrayRef windows = createWindowDescription(id);
//   CFDictionaryRef window = (CFDictionaryRef)CFArrayGetValueAtIndex(windows, 0);
//   CFStringRef ownerNameRef = (CFStringRef)CFDictionaryGetValue(window, kCGWindowOwnerName);
//   std::string result = toString(ownerNameRef);
//   CFRelease(windows);
//   return result;
// }

// std::string windowName(CGWindowID id)
// {
//   CFArrayRef windows = createWindowDescription(id);
//   CFDictionaryRef window = (CFDictionaryRef)CFArrayGetValueAtIndex(windows, 0);
//   CFStringRef nameRef = (CFStringRef)CFDictionaryGetValue(window, kCGWindowName);
//   std::string result = toString(nameRef);
//   CFRelease(windows);
//   return result;
// }

// std::string __windowTitle(CGWindowID id)
// {
//   std::string name = windowName(id);
//   if (name.empty())
//   {
//     std::string ownerName = windowOwnerName(id);
//     return ownerName;
//   }
//   return name;
// }

// CGRect windowGeometry(CGWindowID id)
// {
//   CGRect rect = CGRectMake(0, 0, 100, 100);
//   CGWindowID idCArray[1] = {id};
//   CFArrayRef idArray = CFArrayCreate(NULL, (const void **)idCArray, 1, NULL);
//   CFArrayRef descArray = CGWindowListCreateDescriptionFromArray(idArray);
//   CFDictionaryRef description = (CFDictionaryRef)CFArrayGetValueAtIndex(descArray, 0);
//   if (CFDictionaryContainsKey(description, kCGWindowBounds))
//   {
//     CFDictionaryRef bounds = (CFDictionaryRef)CFDictionaryGetValue(description, kCGWindowBounds);
//     if (bounds)
//       CGRectMakeWithDictionaryRepresentation(bounds, &rect);
//   }
//   CFRelease(descArray);
//   CFRelease(idArray);
//   return rect;
// }

// CGRect mainDisplayBounds()
// {
//   CGDirectDisplayID mainDisp = CGMainDisplayID();
//   CGRect mainDispRect = CGDisplayBounds(mainDisp);
//   return mainDispRect;
// }

// bool equalsWindowRect(CGWindowID id)
// {
//   CGRect dispRect = mainDisplayBounds();
//   CGRect windowRect = windowGeometry(id);
//   bool isFullScreen = CGRectEqualToRect(dispRect, windowRect);
//   return isFullScreen;
// }

// EXPORTED

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
  CFArrayRef windowList = CGWindowListCreate(
      kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements,
      kCGNullWindowID);
  CFIndex count = CFArrayGetCount(windowList);
  Napi::Array result = Napi::Array::New(env);

  for (unsigned int i = 0; i < count; i++)
  {
    uint32_t winId = (uint32_t)(uintptr_t)CFArrayGetValueAtIndex(windowList, i);
    result[i] = Napi::Number::New(env, winId);
  }

  return result;
}

Napi::Value WindowRect(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  WindowID id = GetWindowID(info, "WindowTitle");

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
          CGRectGetMaxX(rect),
          CGRectGetMaxY(rect),
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

// bool isFullScreen(std::string titlePredicate)
// {
//   CFArrayRef windowList = CGWindowListCreate(kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements, kCGNullWindowID);
//   for (int i = 0; i < CFArrayGetCount(windowList); i++)
//   {
//     CGWindowID winId = (CGWindowID)(uintptr_t)CFArrayGetValueAtIndex(windowList, i);
//     std::string title = __windowTitle(winId);
//     if (title.compare(titlePredicate) == 0 and equalsWindowRect(winId))
//     {
//       return true;
//     }
//   }
//   return false;
// }
