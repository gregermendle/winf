use neon::prelude::*;
use core_graphics::window::*;
use core_foundation::string::*;
use core_foundation::number::*;
use core_foundation::dictionary::*;
use core_foundation::base::*;

pub fn hello(mut cx: FunctionContext) -> JsResult<JsString> {
  let window_list = create_window_list(kCGWindowListOptionAll, kCGNullWindowID).unwrap();
  for window in window_list.iter() {
    println!("{:?}", window);
  }
  match create_description_from_array(window_list) {
    Some(window_list) => {
      for window in window_list.iter() {
        let key = CFString::from_static_string("kCGWindowBounds");
        let bounds_dict = window.get(key);
        match bounds_dict.downcast::<CFDictionary>() {
          Some(dict) => {
            let height_key = CFString::from_static_string("Height");
            let num_ptr = dict.get(height_key.to_void());
            println!("{:?}", num_ptr);
            // let value: *mut std::ffi::c_void = std::ptr::null_mut();
            // if unsafe { CFNumberGetValue(*num_ptr as CFNumberRef, kCFNumberFloat64Type, value) } == true {
            //   if !value.is_null() {
            //     println!("{:?}", num_ptr);
            //   }
            // }
          },
          None => println!("Cannot downcast value.")
        }
      }    
    }
    None => println!("Failed to get windows.")
  }
  Ok(cx.string("hello node"))
}
