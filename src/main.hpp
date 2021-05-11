#pragma once
#include <iostream>
#include <memory>
#include <chrono>
#include <ratio>
#define __cplusplus 201703L
#include <filesystem>
#include <dirent.h>
#include <util/macros.hpp>
#include <engine/hyperengine.hpp>
// #include <entities/e_light.hpp>
#include <entities/e_model.hpp>
#include <entities/e_animatedmodel.hpp>
#include <tree/node.hpp>
#include <util/functions.hpp>

// Esto es para una prueba, cube_test
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
// Esto es para una prueba, cube_test

// Esto es para una prueba, loading_textures_soil_test
#include <SOIL/SOIL.h>

// Esto es para una prueba, imgui_testing
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

// Esto es para una prueba, print matrix
#include <glm/gtx/string_cast.hpp>

// Flags para testeo
enum Flags_t {
        f0 = 1 << 0
    ,   f1 = 1 << 1
    ,   f2 = 1 << 2
    ,   f3 = 1 << 3
    ,   f4 = 1 << 4
    ,   f5 = 1 << 5
    ,   f6 = 1 << 6
    ,   f7 = 1 << 7
};
