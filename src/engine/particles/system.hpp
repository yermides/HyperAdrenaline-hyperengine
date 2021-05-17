#pragma once
#include "particle.hpp"
#include <resources/r_shader.hpp>
#include <resources/r_texture.hpp>

namespace hyper {

// Actúa de generador con ciertas propiedades, puede que la engine maneje más de un sistema de partículas
struct ParticleSystem
{
    explicit ParticleSystem(RShader* updater, RShader* renderer);
    ~ParticleSystem();

    void setMatrices(
        glm::mat4 const& projection
    ,   glm::vec3 const& cameraPosition
    ,   glm::vec3 const& cameraTarget
    ,   glm::vec3 const& cameraUp = {0,1,0}
    );

    void update(float deltatime);

    void render(void);

    void setProperties(
        glm::vec3 const& a_vGenPosition
    ,   glm::vec3 const& a_vGenVelocityMin
    ,   glm::vec3 const& a_vGenVelocityMax
    ,   glm::vec3 const& a_vGenGravityVector
    ,   glm::vec3 const& a_vGenColor
    ,   float a_fGenLifeMin
    ,   float a_fGenLifeMax
    ,   float a_fGenSize
    ,   float fEvery
    ,   int a_iNumToGenerate
    );

    int getNumParticles(void) const noexcept;

private:
    inline static int const max_particles_on_screen { 100000 };

    void init(void);

	glm::mat4 
        m_projection
    ,   m_view
    ;

	glm::vec3 
        m_quad1
    ,   m_quad2
    ,   m_generationPosition
    ,   m_generationVelocityMin
    ,   m_generationVelocityRange
    ,   m_generationGravity
    ,   m_generationColor
    ;

    RShader* m_updater      { nullptr };    // solo usado como transformador de vértices gracias al código
    RShader* m_renderer     { nullptr };    // el shader con el que realmente se renderizan 
    RTexture* m_texture     { nullptr };    // la textura con la que se generarán todas las partículas
    
    uint32_t 
        m_transformFeedbackBuffer
    ,   m_particleBuffers[2]
    ,   m_vaos[2]
    ,   m_query
    ,   m_generationQuantityToGen
    ;

    float 
        m_elapsedTime {0.0f}
    ,   m_nextGenerationTimestamp
    ,   m_generationLifespanMin
    ,   m_generationLifespanRange
    ,   m_generationParticleSize
    ;

	int 
        m_currentReadBuffer
    ,   m_numParticles
    ;

};

}

