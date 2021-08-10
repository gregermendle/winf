{
  "targets": [
    {
      "target_name": "window-info",
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "cflags_cc!": ["-std=c++17", "-fno-exceptions"],
      'cflags!': ["-fno-exceptions"],
      "msbuild_settings": {
        "ClCompile": {
            "LanguageStandard": "stdcpp17"
        }
      },
      "conditions": [
        ["OS==\"mac\"",
          {
            "sources": [
              "./src/rect.cc",
              "./src/window-info_mac.cc",
              "./src/window-info.cc"
            ],
            "link_settings": {
              "libraries": [
                "-framework CoreGraphics",
                "-framework CoreFoundation"
              ],
            }
          }
        ],
        ["OS==\"win\"",
          {
            "sources": [
              "./src/rect.cc",
              "./src/window-info_win.cc",
              "./src/window-info.cc"
            ]
          }
        ],
      ]
    }
  ]
}