#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
    float opacity;
    int usesAmbientTexture;
    int usesDiffuseTexture;
    int usesSpecularTexture;
    sampler2D texture_ambient1;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

in vec2 TexCoords;

uniform Material material;

void main()
{    
    if(material.usesDiffuseTexture == 1)
    {
        FragColor = texture(material.texture_diffuse1, TexCoords);
    }
    else
    {
        FragColor = vec4(material.diffuse, 1.0);
    }
}
