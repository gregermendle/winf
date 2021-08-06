{
  "targets": [
    {
      "target_name": "window-info",
      "include_dirs": ["<!(node -e \"require('nan')\")"],
      "conditions": [
        ["OS==\"mac\"",
          {
            "sources": [
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
              "./src/window-info_win.cc",
              "./src/window-info.cc"
            ]
          }
        ],
      ]
    }
  ]
}