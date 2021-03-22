#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// Coordenadas de textura para pasar al fragment shader
out vec2 TexCoords;
// Posición del píxel para el cálculo de luces
out vec3 FragPos;
// Normal del vértice para el cáculo de luces
out vec3 Normal;

// Matrices: proyección, vista y modelo
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    // Sacar la posición 2D del píxel y su normal para las luces
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  

    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0); // Equivalente a: projection * view * model * vec4(aPos, 1.0)
}
