#include <nan.h>

using namespace Nan;
using namespace v8;

#include "window-info.h"

NAN_METHOD(isFullScreen) {
  Nan::Utf8String titlePredicateUTF8(info[0]);
  int len = titlePredicateUTF8.length();
  
  if (len <= 0) {
     return Nan::ThrowTypeError("IsFullScreen - Expects a non-empty string.");
  }

  std::string titlePredicate(*titlePredicateUTF8, len);
  bool fullScreen = isFullScreen(titlePredicate);
  info.GetReturnValue().Set(fullScreen);
}

NAN_MODULE_INIT(Init) {
   Nan::Set(target, New<String>("isFullScreen").ToLocalChecked(), 
    GetFunction(New<FunctionTemplate>(isFullScreen)).ToLocalChecked());
}

NODE_MODULE(native_rt, Init)