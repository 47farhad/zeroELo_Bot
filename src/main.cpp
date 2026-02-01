#include <iostream>
#include <cstdint>
#include "uci.h"
using namespace std;

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

UCIProtocol* uciPtr = nullptr;

int main() {
    uciPtr = new UCIProtocol(); 
    uciPtr->loop();
    
    return 0;
}