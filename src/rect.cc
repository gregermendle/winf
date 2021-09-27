#include "rect.h"

Napi::Object CreateRect(
    const Napi::Env &env,
    const double x,
    const double y,
    const double height,
    const double width)
{
  Napi::Object rect = Napi::Object::New(env);
  rect.Set(Napi::String::New(env, "x"), Napi::Number::New(env, x));
  rect.Set(Napi::String::New(env, "y"), Napi::Number::New(env, y));
  rect.Set(Napi::String::New(env, "height"), Napi::Number::New(env, height));
  rect.Set(Napi::String::New(env, "width"), Napi::Number::New(env, width));
  return rect;
}
