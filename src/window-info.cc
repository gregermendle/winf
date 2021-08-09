#include <nan.h>

using namespace Nan;
using namespace v8;

#include "window-info.h"

NAN_METHOD(isFullScreen)
{
  Nan::Utf8String titlePredicateUTF8(info[0]);
  int len = titlePredicateUTF8.length();

  if (len <= 0)
  {
    return Nan::ThrowTypeError("IsFullScreen - Expects a non-empty string.");
  }

  std::string titlePredicate(*titlePredicateUTF8, len);
  bool fullScreen = isFullScreen(titlePredicate);
  info.GetReturnValue().Set(fullScreen);
}

NAN_METHOD(listWindows)
{
  Local<Array> result = listWindows();
  info.GetReturnValue().Set(result);
}

NAN_METHOD(windowTitle)
{
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  if (!info[0]->IsUint32())
  {
    return Nan::ThrowTypeError("WindowTiel - Expects a number value as its only argument.");
  }

  WindowID id = info[0]->Uint32Value(context).FromJust();
  Local<String> result = windowTitle(id);
  info.GetReturnValue().Set(result);
}

NAN_MODULE_INIT(Init)
{
  Nan::Set(target, New<String>("isFullScreen").ToLocalChecked(),
           GetFunction(New<FunctionTemplate>(isFullScreen)).ToLocalChecked());
  Nan::Set(target, New<String>("listWindows").ToLocalChecked(),
           GetFunction(New<FunctionTemplate>(listWindows)).ToLocalChecked());
  Nan::Set(target, New<String>("windowTitle").ToLocalChecked(),
           GetFunction(New<FunctionTemplate>(windowTitle)).ToLocalChecked());
}

NODE_MODULE(native_rt, Init)