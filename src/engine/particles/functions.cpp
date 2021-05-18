#include "functions.hpp"
#include <stdlib.h>
#include <engine/particles/generator.hpp>

namespace hyen {

void
ParticleGeneratorFunctions::generateRandomColors(Particle& p)
{
    p.r = rand() % 256;
    p.g = rand() % 256;
    p.b = rand() % 256;
    p.a = 255;
}

void 
randomDirFunc_Outburst(glm::vec3& v)
{
    v = glm::vec3(
        (rand()%2000 - 1000.0f)/100.0f
    ,   (rand()%2000 - 1000.0f)/100.0f
    ,   (rand()%2000 - 1000.0f)/100.0f
    );
}

void 
randomDirFunc_Snowwarning(glm::vec3& v)
{
    v = glm::vec3(
        (rand()%2000 - 1000.0f)/1000.0f
    ,   (rand()%2000 - 1000.0f)/1000.0f
    ,   (rand()%2000 - 1000.0f)/1000.0f
    );
}

void 
randomDirFunc_Static(glm::vec3& v)
{
    v = glm::vec3(0.0f, 0.0f, 0.0f);
}

void 
randomColorFunc_random255(uint8_t& i)
{
    i = rand() % 256;
}

void 
randomColorFunc_setToZero(uint8_t& i)
{
    i = 0;
}

void 
randomColorFunc_setTo255(uint8_t& i)
{
    i = 255;
}

void 
randomPosFunc_spawnInsideBox(glm::vec3& output, glm::vec3 const& offset, glm::vec3 const& boxBounds)
{
    output = glm::vec3(
        offset.x + (rand()%(int)boxBounds.x)
    ,   offset.y + (rand()%(int)boxBounds.y)
    ,   offset.z + (rand()%(int)boxBounds.z)
    );
}

void 
mainDirFunc_followCameraTarget(ParticleGenerator& generator, glm::vec3 const&)
{
    // generator.m_mainDir = generator.m_cameraTarget - generator.m_cameraPosition;
}


}