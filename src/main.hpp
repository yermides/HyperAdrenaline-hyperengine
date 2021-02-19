#pragma once
#include <iostream>
#include <entities/e_light.hpp>
#include <tree/node.hpp>
#include <resources/r_shader.hpp>

// Esto es para una prueba, cube_test
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
// Esto es para una prueba, cube_test


#define __HYPER_ENGINE_LOG_SYSTEM

#ifdef  __HYPER_ENGINE_LOG_SYSTEM
    #define LOG(x) std::cout << x << std::endl;
#else
    #define LOG(x)
#endif

