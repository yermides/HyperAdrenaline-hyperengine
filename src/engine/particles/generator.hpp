#pragma once
#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <util/typealiases.hpp>
#include <engine/particles/functions.hpp>

namespace hyen {
    
struct RShader;
struct RTexture;
struct Node;

struct Particle
{
    glm::vec3 pos, speed;
	uint8_t r, g, b, a; // Color
	float size, angle, weight;
	float maxLife {-1.0f}; // Remaining life of the particle. if <0 : dead and unused.
	float life {-1.0f}; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance {-1.0f}; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

struct ParticleGenerator
{
    template <typename T>
    using Container = std::vector<T>;
    using Index = int;

    struct CInfo {
        std::string texturePath;
        glm::vec3 mainDirection;
        glm::vec3 gravity;
        float spreadFactor;
        float lifeSpan;

        // function pointers pls
    };

    explicit ParticleGenerator(RShader* shader, int size = 10000);
    ~ParticleGenerator();

    void setMatrices(
        glm::mat4 const& projection
    ,   glm::mat4 const& view
    ,   glm::vec3 const& cameraPosition
    );

    void setMatrices(Node* const camnode);

    void update(float dt);

    void render(void);

    // funciones amigas para que las funciones de cálculo puedan acceder a las variables privadas
    friend void mainDirFunc_followCameraTarget(ParticleGenerator&);
    friend struct ParticleGeneratorFunctions;

private:
    Index findUnusedParticle();
    void sortParticles();

    RShader* m_shader   { nullptr };
    int const m_maxsize;

    glm::mat4 
        m_projection
    ,   m_view
    ;
    
    glm::vec3 m_cameraPosition  {0};
    glm::vec3 m_cameraTarget    {0};

    Container<Particle> m_particles;
    Container<GLfloat> m_positionBuffer;
    Container<GLubyte> m_colorBuffer;

    RTexture* m_texture { nullptr };

    Index m_index { 0 };
    int m_particlesCount { 0 }; // Se resetea cada vez que se actualiza, comprueba cúantas partículas vivas hay
    float m_particlesPerSecond { 100.0f }; // TODO:: parametrizable

    GLuint
    	m_vertexArrayVAO
    ,   m_billboardVBO
    ,   m_positionsVBO
    ,   m_colorsVBO
    ;

    glm::vec3 m_origin  { 0.0f,   0.0f, 0.0f };
    // glm::vec3 m_gravity { 0.0f, -9.81f, 0.0f };
    glm::vec3 m_gravity { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_mainDir { 0.0f,  10.0f, 0.0f };
    float m_spreadFactor {1.5f};
    float m_lifeSpan {5.0f};
    float m_minParticleSize {0.15f};
    float m_maxParticleSize {0.5f};

    // glm::vec3 m_shapeOffset;
    float m_shapeRadius { 3.f }; // pls parametrizar

    PGF::ParticleColor m_funcColor          { nullptr };
    PGF::ParticleSize m_funcSize            { nullptr };
    PGF::GeneratorRandomDir m_funcRandomdir { nullptr };
    PGF::ParticleStartingPosition m_funcPos { nullptr };
    PGF::GeneratorMainDir m_funcMaindir     { nullptr };
};

} // namespace hyen
