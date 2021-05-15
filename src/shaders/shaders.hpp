#pragma once

namespace Shader {
    constexpr static char const* hypershader_vertex = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTexCoords;

        out vec3 FragPos;
        out vec3 Normal;
        out vec2 TexCoords;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;  
            TexCoords = aTexCoords;
            
            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    )";

    constexpr static char const* hypershader_fragment = R"(
        #version 330 core
        out vec4 FragColor;

        struct Material {
            vec3 ambientColor;
            vec3 diffuseColor;
            vec3 specularColor;

            int usesDiffuseTexture;
            sampler2D diffuse;
            
            int usesSpecularTexture;
            sampler2D specular;
            
            float shininess;
            float opacity;
        }; 

        struct DirLight {
            vec3 direction;
            
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        struct PointLight {
            vec3 position;
            
            // Atenuaciones
            float constant;
            float linear;
            float quadratic;
            
            // Intensidades
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        struct SpotLight {
            vec3 position;
            vec3 direction;
            
            // Apertura en dos pasos
            float cutOff;
            float outerCutOff;
        
            float constant;
            float linear;
            float quadratic;
        
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;       
        };

        // #define NR_POINT_LIGHTS 1
        #define MAX_DIR_LIGHTS 10
        #define MAX_POINT_LIGHTS 50
        #define MAX_SPOT_LIGHTS 20

        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoords;

        // TODO:: hacer cada uno un vector de luces (dirLight a dirLights[numDirLights] y demás)
        uniform int numDirLights;
        uniform int numPointLights;
        uniform int numSpotLights;

        uniform vec3 viewPos;

        // Estos de abajo realmente son arrays
        // uniform DirLight dirLight;
        // uniform SpotLight spotLight;

        uniform DirLight dirLights[MAX_DIR_LIGHTS];
        uniform PointLight pointLights[MAX_POINT_LIGHTS];
        uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
        uniform Material material;

        // function prototypes
        vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
        vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
        vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

        void main()
        {    
            // properties
            vec3 norm = normalize(Normal);
            vec3 viewDir = normalize(viewPos - FragPos);
            
            // == =====================================================
            // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
            // For each phase, a calculate function is defined that calculates the corresponding color
            // per lamp. In the main() function we take all the calculated colors and sum them up for
            // this fragment's final color.
            // == =====================================================

            vec3 result = vec3(0.0, 0.0, 0.0);

            // phase 1: directional lighting
            // Comentada hasta tener luces dirigidas
            // vec3 result = CalcDirLight(dirLight, norm, viewDir);

            for (int i = 0; i < numDirLights; i++)
            {
                result += CalcDirLight(dirLights[i], norm, viewDir);
            }
            
            // phase 2: point lights
            // for(int i = 0; i < NR_POINT_LIGHTS; i++)
                // result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    

            for(int i = 0; i < numPointLights; i++)
            {
                result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
            }

            // vec3 result = CalcPointLight(pointLights[0], norm, FragPos, viewDir); 
                
            // phase 3: spot light
            // Comentada hasta tener luces focales
            // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);  

            for (int i = 0; i < numSpotLights; i++)
            {
                result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);  
            }

            FragColor = vec4(result, 1.0);
            
            // *********************************************************
            
            // Lo anterior funcionaba, se ha cambiado el nombre de las variables del uniform del material
            // FragColor = texture(material.diffuse, TexCoords);
            
            
        }

        // calculates the color when using a directional light.
        vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
        {
            vec3 lightDir = normalize(-light.direction);
            // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            // combine results

            vec3 ambient = vec3(0.0,0.0,0.0);
            vec3 diffuse = vec3(0.0,0.0,0.0);
            vec3 specular = vec3(0.0,0.0,0.0);

            if(material.usesDiffuseTexture == 1)
            {
                ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
                diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
            }
            else
            {
                ambient = light.ambient * material.ambientColor;
                diffuse = light.diffuse * diff * material.diffuseColor;
            }

            if(material.usesSpecularTexture == 1)
            {
                specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
            }
            else
            {
                specular = light.specular * spec * material.specularColor;
            }

            return (ambient + diffuse + specular);
        }

        // calculates the color when using a point light.
        vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
        {
            vec3 lightDir = normalize(light.position - fragPos);
            // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            // attenuation
            float distance = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
            // combine results

            vec3 ambient = vec3(0.0,0.0,0.0);
            vec3 diffuse = vec3(0.0,0.0,0.0);
            vec3 specular = vec3(0.0,0.0,0.0);

            if(material.usesDiffuseTexture == 1)
            {
                ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
                diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
            }
            else
            {
                ambient = light.ambient * material.ambientColor;
                diffuse = light.diffuse * diff * material.diffuseColor;
            }

            if(material.usesSpecularTexture == 1)
            {
                specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
            }
            else
            {
                specular = light.specular * spec * material.specularColor;
            }

            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;
            
            return (ambient + diffuse + specular);
            // return diffuse;
        }

        // calculates the color when using a spot light.
        vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
        {
            vec3 lightDir = normalize(light.position - fragPos);
            // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            // attenuation
            float distance = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
            // spotlight intensity
            float theta = dot(lightDir, normalize(-light.direction)); 
            float epsilon = light.cutOff - light.outerCutOff;
            float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
            // combine results

            vec3 ambient = vec3(0.0,0.0,0.0);
            vec3 diffuse = vec3(0.0,0.0,0.0);
            vec3 specular = vec3(0.0,0.0,0.0);

            if(material.usesDiffuseTexture == 1)
            {
                ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
                diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
            }
            else
            {
                ambient = light.ambient * material.ambientColor;
                diffuse = light.diffuse * diff * material.diffuseColor;
            }

            if(material.usesSpecularTexture == 1)
            {
                specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
            }
            else
            {
                specular = light.specular * spec * material.specularColor;
            }

            ambient *= attenuation * intensity;
            diffuse *= attenuation * intensity;
            specular *= attenuation * intensity;
            
            return (ambient + diffuse + specular);
        }

    )";

    constexpr static char const* skybox_vertex = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        out vec3 TexCoords;

        uniform mat4 projection;
        uniform mat4 view;

        void main()
        {
            TexCoords = aPos;
            vec4 pos = projection * view * vec4(aPos, 1.0);
            gl_Position = pos.xyww;
        }
    )";

    constexpr static char const* skybox_fragment = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 TexCoords;

        uniform samplerCube skybox;

        void main()
        {    
            FragColor = texture(skybox, TexCoords);
        }
    )";

    constexpr static char const* debugdrawer_vertex = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        out VS_OUT {
            vec3 color;
        } vs_out;

        uniform mat4 projection;
        uniform mat4 view;

        void main()
        {
            gl_Position = projection * view * vec4(position, 1.0f);

            vs_out.color = color;
            // vs_out.color = vec3(0.0f, 1.0f, 0.0f);
        }
    )";

    constexpr static char const* debugdrawer_fragment = R"(
        #version 330 core
        in vec3 fColor;
        out vec4 color;

        void main()
        {
            // color = color;  
            color = vec4(fColor, 1.0); 
        }
    )";

    constexpr static char const* normals_vertex = R"(
        #version 330 core

        void main()
        {
        }
    )";

    constexpr static char const* normals_fragment = R"(
        #version 330 core
        
        void main()
        {
        }
    )";

    /* **************** Partículas **************** */
    
    constexpr static Cstring particles_updater_vertex = R"(
        #version 330 core

        layout (location = 0) in vec3 vPosition;
        layout (location = 1) in vec3 vVelocity;
        layout (location = 2) in vec3 vColor;
        layout (location = 3) in float fLifeTime;
        layout (location = 4) in float fSize;
        layout (location = 5) in int iType;

        out vec3 vPositionPass;
        out vec3 vVelocityPass;
        out vec3 vColorPass;
        out float fLifeTimePass;
        out float fSizePass;
        out int iTypePass;

        void main()
        {
            vPositionPass = vPosition;
            vVelocityPass = vVelocity;
            vColorPass = vColor;
            fLifeTimePass = fLifeTime;
            fSizePass = fSize;
            iTypePass = iType;
        }

    )";

    constexpr static Cstring particles_updater_fragment = R"(
        #version 330 core

        out vec4 vColor;

        void main()
        {
            vColor = vec4(1.0, 1.0, 1.0, 1.0);
        }

    )";

    constexpr static Cstring particles_updater_geometry = R"(
        #version 330 core

        layout(points) in;
        layout(points) out;
        layout(max_vertices = 40) out;

        // All that we get from vertex shader

        in vec3 vPositionPass[];
        in vec3 vVelocityPass[];
        in vec3 vColorPass[];
        in float fLifeTimePass[];
        in float fSizePass[];
        in int iTypePass[];

        // All that we send further

        out vec3 vPositionOut;
        out vec3 vVelocityOut;
        out vec3 vColorOut;
        out float fLifeTimeOut;
        out float fSizeOut;
        out int iTypeOut;

        uniform vec3 vGenPosition; // Position where new particles are spawned
        uniform vec3 vGenGravityVector; // Gravity vector for particles - updates velocity of particles 
        uniform vec3 vGenVelocityMin; // Velocity of new particle - from min to (min+range)
        uniform vec3 vGenVelocityRange;

        uniform vec3 vGenColor;
        uniform float fGenSize; 

        uniform float fGenLifeMin, fGenLifeRange; // Life of new particle - from min to (min+range)
        uniform float fTimePassed; // Time passed since last frame

        uniform vec3 vRandomSeed; // Seed number for our random number function
        vec3 vLocalSeed;

        uniform int iNumToGenerate; // How many particles will be generated next time, if greater than zero, particles are generated

        // This function returns random number from zero to one
        float randZeroOne()
        {
            uint n = floatBitsToUint(vLocalSeed.y * 214013.0 + vLocalSeed.x * 2531011.0 + vLocalSeed.z * 141251.0);
            n = n * (n * n * 15731u + 789221u);
            n = (n >> 9u) | 0x3F800000u;
        
            float fRes =  2.0 - uintBitsToFloat(n);
            vLocalSeed = vec3(vLocalSeed.x + 147158.0 * fRes, vLocalSeed.y*fRes  + 415161.0 * fRes, vLocalSeed.z + 324154.0*fRes);
            return fRes;
        }

        void main()
        {
            vLocalSeed = vRandomSeed;
            
            // gl_Position doesn't matter now, as rendering is discarded, so I don't set it at all

            vPositionOut = vPositionPass[0];
            vVelocityOut = vVelocityPass[0];
            if(iTypePass[0] != 0)vPositionOut += vVelocityOut*fTimePassed;
            if(iTypePass[0] != 0)vVelocityOut += vGenGravityVector*fTimePassed;

            vColorOut = vColorPass[0];
            fLifeTimeOut = fLifeTimePass[0]-fTimePassed;
            fSizeOut = fSizePass[0];
            iTypeOut = iTypePass[0];
                
            if(iTypeOut == 0)
            {
                EmitVertex();
                EndPrimitive();
                
                for(int i = 0; i < iNumToGenerate; i++)
                {
                    vPositionOut = vGenPosition;
                    vVelocityOut = vGenVelocityMin+vec3(vGenVelocityRange.x*randZeroOne(), vGenVelocityRange.y*randZeroOne(), vGenVelocityRange.z*randZeroOne());
                    vColorOut = vGenColor;
                    fLifeTimeOut = fGenLifeMin+fGenLifeRange*randZeroOne();
                    fSizeOut = fGenSize;
                    iTypeOut = 1;
                    EmitVertex();
                    EndPrimitive();
                }
            }
            else if(fLifeTimeOut > 0.0)
            {
                EmitVertex();
                EndPrimitive(); 
            }
        }

    )";

    constexpr static Cstring particles_renderer_vertex = R"(
        #version 330 core
        layout (location = 0) in vec3 vPosition;
        layout (location = 2) in vec3 vColor;
        layout (location = 3) in float fLifeTime;
        layout (location = 4) in float fSize;
        layout (location = 5) in int iType;

        out vec3 vColorPass;
        out float fLifeTimePass;
        out float fSizePass;
        out int iTypePass;

        void main()
        {
            gl_Position = vec4(vPosition, 1.0);
            vColorPass = vColor;
            fSizePass = fSize;
            fLifeTimePass = fLifeTime;
            iTypePass = iType;
        }

    )";

    constexpr static Cstring particles_renderer_fragment = R"(
        #version 330 core

        uniform sampler2D gSampler;

        smooth in vec2 vTexCoord;
        flat in vec4 vColorPart;

        out vec4 FragColor;

        void main()
        {
            vec4 vTexColor = texture2D(gSampler, vTexCoord);
            // FragColor = vec4(vTexColor.xyz, 1.0)*vColorPart;
            FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        }

    )";

    constexpr static Cstring particles_renderer_geometry = R"(
        #version 330 core

        uniform struct Matrices
        {
            mat4 mProj;
            mat4 mView;
        } matrices;

        uniform vec3 vQuad1, vQuad2;

        layout(points) in;
        layout(triangle_strip) out;
        layout(max_vertices = 4) out;

        in vec3 vColorPass[];
        in float fLifeTimePass[];
        in float fSizePass[];
        in int iTypePass[];

        smooth out vec2 vTexCoord;
        flat out vec4 vColorPart;

        void main()
        {
            if(iTypePass[0] != 0)
            {
                vec3 vPosOld = gl_in[0].gl_Position.xyz;
                float fSize = fSizePass[0];
                mat4 mVP = matrices.mProj*matrices.mView;
                
                vColorPart = vec4(vColorPass[0], fLifeTimePass[0]);
                
                vec3 vPos = vPosOld+(-vQuad1-vQuad2)*fSize;
                vTexCoord = vec2(0.0, 0.0);
                gl_Position = mVP*vec4(vPos, 1.0);
                EmitVertex();
                
                vPos = vPosOld+(-vQuad1+vQuad2)*fSize;
                vTexCoord = vec2(0.0, 1.0);
                gl_Position = mVP*vec4(vPos, 1.0);
                EmitVertex();
                
                vPos = vPosOld+(vQuad1-vQuad2)*fSize;
                vTexCoord = vec2(1.0, 0.0);
                gl_Position = mVP*vec4(vPos, 1.0);
                EmitVertex();
                
                vPos = vPosOld+(vQuad1+vQuad2)*fSize;
                vTexCoord = vec2(1.0, 1.0);
                gl_Position = mVP*vec4(vPos, 1.0);
                EmitVertex();
                
                EndPrimitive();
            }
        }
        
    )";

}