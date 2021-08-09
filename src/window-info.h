#include <iostream>
#include <nan.h>

typedef uint32_t WindowID;

bool isFullScreen(std::string titlePredicate);
v8::Local<v8::Array> listWindows();
v8::Local<v8::String> windowTitle(WindowID id);