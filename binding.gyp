{
  "targets": [
    {
      "target_name": "window-info",
      "sources": [
        "./src/window-info_mac.cc",
        "./src/window-info.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"],
      "conditions": [
        ["OS==\"mac\"",
          {
            "link_settings": {
              "libraries": [
                "-framework CoreGraphics",
                "-framework CoreFoundation"
              ],
            }
          }
        ],
      ]
    }
  ]
}