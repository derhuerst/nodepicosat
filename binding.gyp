{
  "targets": [{
    "target_name": "picosat",
    "include_dirs": [
      "<!(node -e \"require('napi-macros')\")"
    ],
    "sources": [ "lib/native/index.c", "lib/picosat.c" ]
  }]
}
