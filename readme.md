# WASM with C++ and emscripten vs built-in javascript

1. run the following command: emcc hello_world.cpp -o main.js -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'
2. start webserver with python3 -m http-server
3. go to localhost:8000/index.html
4. follow instructions on webpage
5. test with the number 1 and number 44651687647

