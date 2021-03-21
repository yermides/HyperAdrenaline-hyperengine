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
in vec3 FragPos;  
in vec3 Normal;  

uniform Material material;

// Propiedades de la luz
uniform int usesLightning;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
// uniform vec3 objectColor; // Usar material.diffuse

void main()
{    
    if(material.usesDiffuseTexture == 1)
    {
        FragColor = texture(material.texture_diffuse1, TexCoords);
    }
    else
    {
        if(usesLightning == 1)
        {
            // 2.2.basic_lighting.fs
            // ambient
            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength * lightColor;
            
            // diffuse 
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            
            // specular
            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);  
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor;  
                
            // vec3 result = (ambient + diffuse + specular) * objectColor;
            vec3 result = (ambient + diffuse + specular) * material.diffuse;
            
            // Linea comentada de momento porque no se usan los valores anteriores
            // FragColor = vec4(result, 1.0);
            
            FragColor = vec4(1.0);
        }
        else
        {
            FragColor = vec4(material.diffuse, 1.0);
        }
    }
}
