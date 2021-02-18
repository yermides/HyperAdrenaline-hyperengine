/*
    This file exists only for testing
    it'll be removed once the engine
    is compiled into an .a file
*/
#include "main.hpp"

int main(void) {
    ELight light;
    light.setAperture(0.0f);
    
    LOG("Hello world");
    LOG("The light has " << light.getAperture() << " aperture");
}
