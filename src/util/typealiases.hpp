#pragma once

// OpenGL types replicated, maybe use glad
using GLint = int;
using GLuint = unsigned int;
using GLfloat = float;

// Our own types
using ProgramIdentifier = GLuint;
using Cstring = const char*;
using Image = unsigned char*; // Byte array
