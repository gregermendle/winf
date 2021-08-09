#include <napi.h>

Napi::Object CreateRect(const Napi::Env &env,
                        const double x,
                        const double y,
                        const double height,
                        const double width);
