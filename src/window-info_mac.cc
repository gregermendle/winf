#include "window-info.h"
#include <CoreGraphics/CoreGraphics.h>
#include <iostream>

std::string toString(CFStringRef str)
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

CFArrayRef createWindowDescription(CGWindowID id)
{
  CGWindowID idCArray[1] = {id};
  CFArrayRef idArray = CFArrayCreate(NULL, (const void **)idCArray, 1, NULL);
  CFArrayRef result = CGWindowListCreateDescriptionFromArray(idArray);
  CFRelease(idArray);
  return result;
}

std::string windowOwnerName(CGWindowID id)
{
  CFArrayRef windows = createWindowDescription(id);
  CFDictionaryRef window = (CFDictionaryRef)CFArrayGetValueAtIndex(windows, 0);
  CFStringRef ownerNameRef = (CFStringRef)CFDictionaryGetValue(window, kCGWindowOwnerName);
  std::string result = toString(ownerNameRef);
  CFRelease(windows);
  return result;
}

std::string windowName(CGWindowID id)
{
  CFArrayRef windows = createWindowDescription(id);
  CFDictionaryRef window = (CFDictionaryRef)CFArrayGetValueAtIndex(windows, 0);
  CFStringRef nameRef = (CFStringRef)CFDictionaryGetValue(window, kCGWindowName);
  std::string result = toString(nameRef);
  CFRelease(windows);
  return result;
}

std::string __windowTitle(CGWindowID id)
{
  std::string name = windowName(id);
  if (name.empty())
  {
    std::string ownerName = windowOwnerName(id);
    return ownerName;
  }
  return name;
}

CGRect windowGeometry(CGWindowID id)
{
  CGRect rect = CGRectMake(0, 0, 100, 100);
  CGWindowID idCArray[1] = {id};
  CFArrayRef idArray = CFArrayCreate(NULL, (const void **)idCArray, 1, NULL);
  CFArrayRef descArray = CGWindowListCreateDescriptionFromArray(idArray);
  CFDictionaryRef description = (CFDictionaryRef)CFArrayGetValueAtIndex(descArray, 0);
  if (CFDictionaryContainsKey(description, kCGWindowBounds))
  {
    CFDictionaryRef bounds = (CFDictionaryRef)CFDictionaryGetValue(description, kCGWindowBounds);
    if (bounds)
      CGRectMakeWithDictionaryRepresentation(bounds, &rect);
  }
  CFRelease(descArray);
  CFRelease(idArray);
  return rect;
}

CGRect mainDisplayBounds()
{
  CGDirectDisplayID mainDisp = CGMainDisplayID();
  CGRect mainDispRect = CGDisplayBounds(mainDisp);
  return mainDispRect;
}

bool equalsWindowRect(CGWindowID id)
{
  CGRect dispRect = mainDisplayBounds();
  CGRect windowRect = windowGeometry(id);
  bool isFullScreen = CGRectEqualToRect(dispRect, windowRect);
  return isFullScreen;
}

// EXPORTED

// Convert windowName and windowOwnerName to char*
v8::Local<v8::String> windowTitle(WindowID id)
{
  std::string name = windowName((CGWindowID)id);
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  if (name.empty())
  {
    std::string ownerName = windowOwnerName((CGWindowID)id);
    v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(isolate, ownerName.c_str());
    return result.ToLocalChecked();
  }
  v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(isolate, name.c_str());
  return result.ToLocalChecked();
}

v8::Local<v8::Array> listWindows()
{
  CFArrayRef windowList = CGWindowListCreate(kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements, kCGNullWindowID);
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::Local<v8::Array> result = Nan::New<v8::Array>();
  for (unsigned int i = 0; i < CFArrayGetCount(windowList); i++)
  {
    uint32_t winId = (uint32_t)(uintptr_t)CFArrayGetValueAtIndex(windowList, i);
    Nan::Set(result, i, v8::Number::New(isolate, winId));
  }
  return result;
}

bool isFullScreen(std::string titlePredicate)
{
  CFArrayRef windowList = CGWindowListCreate(kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements, kCGNullWindowID);
  for (int i = 0; i < CFArrayGetCount(windowList); i++)
  {
    CGWindowID winId = (CGWindowID)(uintptr_t)CFArrayGetValueAtIndex(windowList, i);
    std::string title = __windowTitle(winId);
    if (title.compare(titlePredicate) == 0 and equalsWindowRect(winId))
    {
      return true;
    }
  }
  return false;
}
