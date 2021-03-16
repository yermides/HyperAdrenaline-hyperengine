#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
    int usesAmbientTexture;
    int usesDiffuseTexture;
    int usesSpecularTexture;
};

in vec2 TexCoords;

uniform sampler2D texture_ambient1;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform Material material;

uniform int usesTextures;

void main()
{    
    if(usesTextures == 1)
    {
        FragColor = texture(texture_diffuse1, TexCoords);
    }
    else
    {
        // FragColor = vec4(1.0);
        FragColor = vec4(material.diffuse, 1.0);
    }
}
