#include "system.hpp"
#include <glm/gtc/matrix_transform.hpp>

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
ParticleSystem::setMatrices(glm::mat4 const& projection, glm::vec3 const& cameraPosition, glm::vec3 const& cameraTarget, glm::vec3 const& cameraUp)
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

void 
ParticleSystem::update(float deltatime)
{
    m_updater->bind();

    // m_updater->setFloat();

    // m_updater->setVec3();
    // m_updater->setVec3();
    // m_updater->setVec3();
    // m_updater->setVec3();
    // m_updater->setVec3();

    // m_updater->setFloat();
    // m_updater->setFloat();
    // m_updater->setFloat();

    // m_updater->setInt();

    m_elapsedTime += deltatime;

    if(m_elapsedTime >= m_nextGenerationTimestamp)
    {
        // coud jiar
    }
}

void 
ParticleSystem::render()
{

}

void 
ParticleSystem::setProperties()
{

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

    Particle seed;
    seed.m_type = Particle::Type::Generator;

    for (uint8_t i {0}; i < 2; ++i)
    {
        glBindVertexArray(m_vaos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * max_particles_on_screen, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle), &seed);

        static uint8_t const numParticleVariables {6};

        for (uint8_t j {0}; j < numParticleVariables; ++j)
        {
            glEnableVertexAttribArray(j);
        }

        // (void*)offsetof(Particle, Particle::m_position)

        // Hardcoded pero bien puesto, si eso usar el offsetof de arriba
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)0); // Position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)12); // Velocity
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)24); // Color
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)36); // Lifetime
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)40); // Size
		glVertexAttribPointer(5, 1, GL_INT,	  GL_FALSE, sizeof(Particle), (const GLvoid*)44); // Type
    }

    m_currentReadBuffer = 0;
	m_numParticles = 1;
}


}
