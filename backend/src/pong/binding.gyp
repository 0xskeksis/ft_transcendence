{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "srcs/main.cpp",
		"srcs/Game.cpp"
      ],
      "include_dirs": [
        "/app/node_modules/node-addon-api",
        "/app/src/pong/includes"
      ],
      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS"
      ],
      "cflags_cc": [
        "-std=c++17"
      ]
    }
  ]
}

