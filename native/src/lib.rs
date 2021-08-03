use neon::prelude::*;
mod mac;

register_module!(mut cx, {
  cx.export_function("hello", mac::window::hello)
});
