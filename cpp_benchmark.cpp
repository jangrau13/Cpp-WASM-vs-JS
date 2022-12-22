#include <emscripten/emscripten.h>
#include <stdint.h>




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


