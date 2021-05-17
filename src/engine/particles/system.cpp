#include "system.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <managers/resource_manager.hpp>

namespace hyper {

ParticleSystem::ParticleSystem(RShader* updater, RShader* renderer)
:   m_updater(updater)
,   m_renderer(renderer)
{
    init();
}

ParticleSystem::~ParticleSystem()
{
    m_updater   = nullptr;
    m_renderer  = nullptr;
    m_texture   = nullptr;
}

void 
ParticleSystem::setMatrices(
    glm::mat4 const& projection
,   glm::vec3 const& cameraPosition
,   glm::vec3 const& cameraTarget
,   glm::vec3 const& cameraUp
)
{
	m_projection = projection;

	m_view = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

    auto target { cameraTarget };
	target = target - cameraPosition;
	target = glm::normalize(target);

	m_quad1 = glm::cross(target, cameraUp);
	m_quad1 = glm::normalize(m_quad1);
	m_quad2 = glm::cross(target, m_quad1);
	m_quad2 = glm::normalize(m_quad2);
}

// función auxiliar que genera un float random entre los números que le pasamos
float grandf(float fMin, float fAdd)
{
	float fRandom = float(rand() % (RAND_MAX+1)) / float(RAND_MAX);
	return fMin + fAdd * fRandom;
}

void 
ParticleSystem::update(float deltatime)
{
    m_updater->bind();

    m_updater->setFloat("fTimePassed",          m_elapsedTime);

    m_updater->setVec3("vGenPosition",          m_generationPosition        );
    m_updater->setVec3("vGenVelocityMin",       m_generationVelocityMin     );
    m_updater->setVec3("vGenVelocityRange",     m_generationVelocityRange   );
    m_updater->setVec3("vGenColor",             m_generationColor           );
    m_updater->setVec3("vGenGravityVector",     m_generationGravity         );

    m_updater->setFloat("fGenLifeMin",          m_generationLifespanMin     );
    m_updater->setFloat("fGenLifeRange",        m_generationLifespanRange   );
    m_updater->setFloat("fGenSize",             m_generationParticleSize    );

    m_updater->setInt("iNumToGenerate",         0);

    m_elapsedTime += deltatime;

    if(m_elapsedTime >= m_nextGenerationTimestamp)
    {
        m_updater->setInt("iNumToGenerate",     m_generationQuantityToGen);
        m_elapsedTime -= m_nextGenerationTimestamp;
        
        // glm::vec3 randomSeed { grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f), grandf(-10.0f, 20.0f) };
        glm::vec3 randomSeed { 0,0,0 };
        m_updater->setVec3("vRandomSeed",       randomSeed);
    }

    //  Retroalimentar a nuestras variables desde el shader a los buffers

    glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedbackBuffer);

	glBindVertexArray(m_vaos[m_currentReadBuffer]);
	glEnableVertexAttribArray(1); // Re-habilitar velocidad

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffers[1 - m_currentReadBuffer]);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_query);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, m_numParticles);

	glEndTransformFeedback();

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	glGetQueryObjectiv(m_query, GL_QUERY_RESULT, &m_numParticles);

	m_currentReadBuffer = 1 - m_currentReadBuffer;

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    // m_updater->unbind();
}

void 
ParticleSystem::render()
{
    // Activar blend function para permitir la transparencia de las partículas
	m_renderer->bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->getProgramID());

    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(0);

	glDisable(GL_RASTERIZER_DISCARD);

    m_renderer->setMat4("matrices.mProj", m_projection);
    m_renderer->setMat4("matrices.mView", m_view);
    m_renderer->setVec3("vQuad1", m_quad1);
    m_renderer->setVec3("vQuad2", m_quad1);
    m_renderer->setInt("gSampler", 0);

	glBindVertexArray(m_vaos[m_currentReadBuffer]);
	glDisableVertexAttribArray(1); // Disable velocity, because we don't need it for rendering

	glDrawArrays(GL_POINTS, 0, m_numParticles);

	glDepthMask(1);	
	glDisable(GL_BLEND);

    // m_renderer->unbind();
}

void 
ParticleSystem::setProperties(
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
)
{
	m_generationPosition        = a_vGenPosition;
	m_generationVelocityMin     = a_vGenVelocityMin;
	m_generationVelocityRange   = a_vGenVelocityMax - a_vGenVelocityMin;

	m_generationGravity         = a_vGenGravityVector;
	m_generationColor           = a_vGenColor;
	m_generationParticleSize    = a_fGenSize;

	m_generationLifespanMin     = a_fGenLifeMin;
	m_generationLifespanRange   = a_fGenLifeMax - a_fGenLifeMin;

	m_nextGenerationTimestamp = fEvery;
	m_elapsedTime = 0.8f;

	m_generationQuantityToGen = a_iNumToGenerate;
}

int 
ParticleSystem::getNumParticles(void) const noexcept
{
    return m_numParticles;
}

// Private functions

void 
ParticleSystem::init(void)
{
    static uint8_t const varyings_size {6};

    Cstring varyings[varyings_size] = {
        "vPositionOut"
    ,   "vVelocityOut"
    ,   "vColorOut"
    ,   "fLifeTimeOut"
    ,   "fSizeOut"
    ,   "iTypeOut"
    };

    for(uint8_t i {0}; i < varyings_size; ++i)
    {
        glTransformFeedbackVaryings(m_updater->getProgramID(), varyings_size, varyings, GL_INTERLEAVED_ATTRIBS);
    }

    glGenTransformFeedbacks(1, &m_transformFeedbackBuffer);
	glGenQueries(1, &m_query);
	glGenBuffers(2, m_particleBuffers);
	glGenVertexArrays(2, m_vaos);

    SParticle seed;
    seed.m_type = SParticle::Type::Generator;

    for (uint8_t i {0}; i < 2; ++i)
    {
        glBindVertexArray(m_vaos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SParticle) * max_particles_on_screen, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SParticle), &seed);

        static uint8_t const numParticleVariables {6};

        for (uint8_t j {0}; j < numParticleVariables; ++j)
        {
            glEnableVertexAttribArray(j);
        }

        // (void*)offsetof(Particle, Particle::m_position)

        // Hardcoded pero bien puesto, si eso usar el offsetof de arriba
        // TODO:: comprobar que los attribs no colisionen con el otro shader, el de materiales
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SParticle), (const GLvoid*)0); // Position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SParticle), (const GLvoid*)12); // Velocity
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SParticle), (const GLvoid*)24); // Color
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(SParticle), (const GLvoid*)36); // Lifetime
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(SParticle), (const GLvoid*)40); // Size
		glVertexAttribPointer(5, 1, GL_INT,	  GL_FALSE, sizeof(SParticle), (const GLvoid*)44); // Type
    }

    m_currentReadBuffer = 0;
	m_numParticles = 1;

    // m_texture = ResourceManager::getResource_t<RTexture>("assets/particles/particle.bmp");
    m_texture = ResourceManager::getResource_t<RTexture>("assets/logo.jpg");
    m_texture->initialize();
    
}


}
