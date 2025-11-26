{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "srcs/main.cpp",
		"srcs/Game.cpp"
      ],
      "include_dirs": [
        "/home/ellanglo/Project/ft_transcendence/backend/node_modules/node-addon-api",
        "/home/ellanglo/Project/ft_transcendence/backend/src/pong/includes"
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

