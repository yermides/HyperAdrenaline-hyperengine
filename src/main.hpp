#pragma once
#include <iostream>
#include <util/macros.hpp>
#include <hyperengine.hpp>
#include <entities/e_light.hpp>
#include <entities/e_model.hpp>
#include <tree/node.hpp>
#include <resources/r_shader.hpp>
#include <managers/resource_manager.hpp>

// Esto es para una prueba, cube_test
using namespace glm;
// Esto es para una prueba, cube_test

// Esto es para una prueba, loading_textures_soil_test
#include <SOIL/SOIL.h>

// Esto es para una prueba, imgui_testing
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>


// This matrix belongs to the ECamera
// Projection matrix : 45° Field of View, 16:9 ratio, display range : 0.1 unit <-> 100 units
static glm::mat4 Projection 
	= glm::perspective(
		glm::radians(45.0f)
		, 16.0f / 9.0f
		, 0.1f
		, 100.0f
);

// This matrix belongs to the Node that contains ECamera (and is inverted)
// Camera matrix
static glm::mat4 View  
	= glm::lookAt(
		glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
);

// This matrix belongs to the Node that contains the EMesh
// Model matrix : an identity matrix (model will be at the origin)
static glm::mat4 Model      = glm::mat4(1.0f);

// Our ModelViewProjection : multiplication of our 3 matrices
static glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
