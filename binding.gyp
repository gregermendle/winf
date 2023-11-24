{
  "variables": {
    'openssl_fips': 0
  },
  "targets": [
    {
      "target_name": "winf",
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
              "./src/winf_mac.cc",
              "./src/winf.cc"
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
              "./src/winf_win.cc",
              "./src/winf.cc"
            ]
          }
        ],
      ]
    }
  ]
}