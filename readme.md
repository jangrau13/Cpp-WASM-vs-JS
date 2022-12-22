# Programming with Advanced Computer Languages - Project

> A project by Lukas Bamert and Jan Grau

## A Small Performance-Test between WebAssembly written in C++ vs built-in Javascript

With this project we want to compare C++ code compiled to WebAssembly to built-in Javascript in order to have some small performance benchmarking between the two languages. In order to make it a little bit user friendlier, we also provided a small HTML frontend with Tailwind CSS and jQuery.

## The Function to Compare
The program compares two times the same function. The function itself tries to find the bitwise inverse of a number that is given to the function. It could be seen as a blockmining nonce finder (although will be a pretty bad one).
It does not use any fancy algorithm, but starts with a number $x$ where every bit is 0 and adds 1 to it. It then inverses the input number and does a XOR operation with the current $x$. If the result of this operation equals 0, we know, that we found the inverse of your input. If not, the algorithm will add 1 to $x$ again until the final result equals to 0. Or written in Pseudo-Code:


```
Function findInverse:
  Read input;
  SET x to 0;
  DO 
      ADD 1 to x;
      SET output = x XOR (INVERSE of input);
  WHILE(output != 0);
END.
```

Small side-notes: Because Javascript uses unsigned 32-bit integer to do bitwise operation, the operation are done in this set. Furthermore, it is known, that this could be done much faster, but it was the authors goal to have as many operations possible in a small and understandable function, that can be written both in C++ as well as in Javascript.



## Build and Run Instructions

### How to Run
If you just want to see the final product, there is a final version of the program deploy here: https://cpp-wasm-vs-js.pages.dev/.

### How to Build
1. Download emscripten from here: https://emscripten.org/docs/getting_started/downloads.html
2. run the following command in this folder:

```console
emcc cpp_benchmark.cpp -o main.js -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' 
```
3. start a small webserver (python web server just as an example):
```console
python3 -m http-server 
```
4. now you can visit http://localhost:8000/index.html 

### How to Use
1. Test the programm with the number 44651687647 and see the how the result should look like 
2. Test the programm with any number and see which language is faster. Be aware that in some cases it can take up to several minutes for the programm to finish

## How to Understand the Code

### cpp_benchmark.cpp
The whole function:

```c
extern "C" {
  EMSCRIPTEN_KEEPALIVE uint32_t mine(uint32_t input)
{
    uint32_t nonce = 0;
    uint32_t output = 0;
    
    do{
        nonce = nonce + 1;
        output = nonce ^ ~input;

    }while(output != 0);
    return nonce;
}
}
```
#### extern C
```c
extern "C" 
```
This is a help for the compiler/linker in order to use the C naming and calling conventions instead of the C++ one. Because of this we can call the function by it's actual name and do not get a generated one.

#### EMSCRIPTEN KEEP ALIVE
```c
  EMSCRIPTEN_KEEPALIVE uint32_t mine(uint32_t input)
```
With this statement, we tell emscripten to keep the function alive, so we can call it from Javascript at any time.

#### Bitwise Operator
C++ and javascript use the same signs for our bitwise operators
  
- ^ for bitwise XOR
- ~ for bitwise Inverse

#### special Other Functions
```javascript
      const importObject = {
      module: {},
      env: {
        memory: new WebAssembly.Memory({ initial: 256 }),
      }
    };
    let mine;

    WebAssembly.instantiateStreaming(
      fetch('main.wasm'),
      importObject
    ).then(result => {
      mine = result.instance.exports.mine;
      console.log('WASM intialized')
      $('#inputData').removeClass('hidden');
    });
```

With those two functions we import the generated WebAssembly-File and fetch it to be able to call functions in it. The function mine gets exported from the WebAssembly-File and can be used later.

```HTML
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.1/jquery.min.js"></script>
  <script src="https://cdn.tailwindcss.com"></script>
  <script>
    tailwind.config = {
      theme: {
        extend: {
          colors: {
            'regal-blue': '#243c5a'
          }
        }
      }
    }
  </script>
  <style type="text/tailwindcss">
    @layer utilities {
      .content-auto {
        content-visibility: auto;
      }
    }
  </style>

```

Used to import jQuery and Tailwind from a CDN in order to not have it to provide by ourselves.

#### Emscripten
According to their [website](https://emscripten.org/) Emscripten is the following:

>"Emscripten is a complete compiler toolchain to WebAssembly, using LLVM, with a special focus on speed, size, and the Web platform."

In our own words:

> "Emscripten compiles our C++ Code into WASM + JS so that we do not have to bother on the specific implementation. It provides us with the tooling to be able to directly call WASM-function with javascript (as seen above)."