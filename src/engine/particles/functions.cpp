#include "functions.hpp"
#include <stdlib.h>
#include <engine/particles/generator.hpp>

namespace hyen {

void 
ParticleGeneratorFunctions::generateParticleColorsStandard(Particle& p)
{
    p.r = 255;
    p.g = 255;
    p.b = 255;
    p.a = 255;
}

void
ParticleGeneratorFunctions::generateParticleColorsRandomly(Particle& p)
{
    p.r = rand() % 256;
    p.g = rand() % 256;
    p.b = rand() % 256;
    p.a = 255;
}

void 
ParticleGeneratorFunctions::generateParticleTransparencyRandomly(Particle& p)
{
    p.r = 255;
    p.g = 255;
    p.b = 255;
    p.a = rand() % 256;
}

void 
ParticleGeneratorFunctions::generateParticleColorsAndTransparencyRandomly(Particle& p)
{
    p.r = rand() % 256;
    p.g = rand() % 256;
    p.b = rand() % 256;
    p.a = rand() % 256;
}

void 
ParticleGeneratorFunctions::generateParticleSizeBetween(Particle& p, float min, float max)
{
    float percent = (float)(rand() % 1000) / 1000.0f;
    p.size = ((max - min) * percent) + min;
}

glm::vec3
ParticleGeneratorFunctions::generateRandomDirectionStandard()
{
    return glm::vec3(0,0,0);
}

glm::vec3
ParticleGeneratorFunctions::generateRandomDirectionSoftInfluence()
{
    return glm::vec3(
        (rand()%2000 - 1000.0f)/1000.0f
    ,   (rand()%2000 - 1000.0f)/1000.0f
    ,   (rand()%2000 - 1000.0f)/1000.0f
    );
}

glm::vec3
ParticleGeneratorFunctions::generateRandomDirectionHeavyInfluence()
{
    return glm::vec3(
        (rand()%2000 - 1000.0f)/100.0f
    ,   (rand()%2000 - 1000.0f)/100.0f
    ,   (rand()%2000 - 1000.0f)/100.0f
    );
}

void 
ParticleGeneratorFunctions::generateRandomPositionBoxShape(ParticleGenerator& generator, Particle& p, glm::vec3 const& mainDir)
{
    auto const& offset = generator.m_origin;
    auto const& r = generator.m_shapeRadius;

    p.pos = glm::vec3(
        offset.x - (r/2.0f) + (rand() % 1000 / 1000.0f * r )
    ,   offset.y - (r/2.0f) + (rand() % 1000 / 1000.0f * r )
    ,   offset.z - (r/2.0f) + (rand() % 1000 / 1000.0f * r )
    );
}

void 
ParticleGeneratorFunctions::generateRandomPositionChaoticSphere(ParticleGenerator& generator, Particle& p, glm::vec3 const& mainDir)
{
    // ya iba mal, esto es lo de antes
    //     auto dir = glm::normalize(
    //     glm::vec3(
    //         rand() % 1000 
    //     ,   rand() % 1000 
    //     ,   rand() % 1000 
    //     )
    // );

    // v = dir * r;
}

void 
ParticleGeneratorFunctions::generatePositionStatic(ParticleGenerator& generator, Particle& p, glm::vec3 const& mainDir)
{
    p.pos = generator.m_origin;
}

void 
ParticleGeneratorFunctions::generatePositionCameraTarget(ParticleGenerator& generator, Particle& p, glm::vec3 const& mainDir)
{
    p.pos = generator.m_cameraPosition + ( glm::normalize(mainDir) * 2.0f);
}

glm::vec3 
ParticleGeneratorFunctions::generateMainDirectionStandard(ParticleGenerator& generator)
{
    return generator.m_mainDir;
}

glm::vec3 
ParticleGeneratorFunctions::generateMainDirectionCameraTarget(ParticleGenerator& generator)
{
    return generator.m_cameraTarget - generator.m_cameraPosition;
}

}
