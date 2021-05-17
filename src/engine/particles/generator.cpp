#include "generator.hpp"
#include <glm/gtx/norm.hpp>
#include <resources/r_shader.hpp>
#include <resources/r_texture.hpp>
#include <managers/resource_manager.hpp>

namespace hyen {

ParticleGenerator::ParticleGenerator(RShader* shader, int size)
:   m_shader(shader)
,   m_maxsize(size)
{
    // anterior TODO:: falta cargar e initialize() de la textura
    // ahora:  pls parametrizar
    m_texture = ResourceManager::getResource_t<RTexture>("assets/particles/particle.DDS");
    // m_texture = ResourceManager::getResource_t<RTexture>("assets/particles/smoke.png");
    // m_texture = ResourceManager::getResource_t<RTexture>("assets/particles/enlazame-esta.png");
    
    m_texture->initialize();

    Particle base;
    m_particles         = Container<Particle>(m_maxsize, base);

    m_positionBuffer    = Container<GLfloat>(m_maxsize * 4);

    m_colorBuffer       = Container<GLubyte>(m_maxsize * 4);

    // Inicializar valores a pasar a OpenGL
    m_shader->bind();

    glGenVertexArrays(1, &m_vertexArrayVAO);
	glBindVertexArray(m_vertexArrayVAO);

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.

    static const GLfloat g_vertex_buffer_data[] = { 
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};

	glGenBuffers(1, &m_billboardVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_billboardVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &m_positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionsVBO);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, m_maxsize * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &m_colorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorsVBO);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, m_maxsize * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

    m_shader->unbind();
}

ParticleGenerator::~ParticleGenerator()
{
    // delete[] g_particule_position_size_data;

	// // Cleanup VBO and shader
	// glDeleteBuffers(1, &particles_color_buffer);
	// glDeleteBuffers(1, &particles_position_buffer);
	// glDeleteBuffers(1, &billboard_vertex_buffer);
	// glDeleteProgram(programID);
	// glDeleteTextures(1, &TextureID);
	// glDeleteVertexArrays(1, &VertexArrayID);
}

void 
ParticleGenerator::setMatrices(
    glm::mat4 const& projection
,   glm::mat4 const& view
,   glm::vec3 const& cameraPosition
)
{
    m_projection = projection;
    m_view = view;
    m_cameraPosition = cameraPosition;
}

void 
ParticleGenerator::update(float dt)
{
    // Generate 10 new particule each millisecond,
    // but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
    // newparticles will be huge and the next frame even longer.
    // int newparticles = (int)(dt*10000.0);

    int newparticles = (int)(dt * m_particlesPerSecond);
    // if (newparticles > (int)(0.016f*10000.0))
    //     newparticles = (int)(0.016f*10000.0);
    

    //  Por la cantidad que se generen, 
    for(int i {0}; i < newparticles; ++i) 
    {
        float spread = 1.5f;
        // glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
        glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
        // Very bad way to generate a random direction; 
        // See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
        // combined with some user-controlled parameters (main direction, spread, etc)

        // glm::vec3 randomdir = glm::vec3(
        //     (rand()%2000 - 1000.0f)/1000.0f,
        //     (rand()%2000 - 1000.0f)/1000.0f,
        //     (rand()%2000 - 1000.0f)/1000.0f
        // );

        // El efecto este mola para una explosión
        // glm::vec3 randomdir = glm::vec3(
        //     (rand()%2000 - 1000.0f)/100.0f,
        //     (rand()%2000 - 1000.0f)/100.0f,
        //     (rand()%2000 - 1000.0f)/100.0f
        // );

        glm::vec3 randomdir;
        // m_dirFunc(randomdir);
        randomDirFunc_Outburst(randomdir);
        
        int particleIndex = findUnusedParticle();
        auto& particle { m_particles.at(particleIndex) };

        particle.life = 5.0f; // This particle will live 5 seconds.
        // particle.life = 0.3f; 
        // particle.pos = glm::vec3(0,0,0); // Hardcoded
        randomPosFunc_spawnInsideBox(particle.pos, {0,0,0}, {2,2,2});
        particle.speed = maindir + randomdir * spread;
        // particle.r = rand() % 256;  // Very bad way to generate a random color
        // particle.g = rand() % 256;
        // particle.b = rand() % 256;
        // particle.a = (rand() % 256) / 3;
        particle.size = (rand()%1000)/2000.0f + 0.1f;

        // por probar, punteros a función
        randomColorFunc_random255(particle.r);
        randomColorFunc_random255(particle.g);
        randomColorFunc_random255(particle.b);
        randomColorFunc_setTo255(particle.a);
        // particle.size = (rand()%1000)/200.0f + 0.1f;
    }

    // Simulate all particles
    m_particlesCount = 0;
    for(int i {0}; i < m_maxsize; ++i) {

        auto& p { m_particles.at(i) }; // shortcut

        if(p.life > 0.0f) {

            // Decrease life
            p.life -= dt;

            if (p.life > 0.0f) 
            {
                // Simulate simple physics : gravity only, no collisions
                p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)dt * 0.5f;
                // p.speed = glm::vec3(0.0f, 0.0f, 0.0f);
                p.pos += p.speed * (float)dt;
                p.cameradistance = glm::length2( p.pos - m_cameraPosition );

                // Fill the GPU buffer
                m_positionBuffer.at(4 * m_particlesCount + 0) = p.pos.x;
                m_positionBuffer.at(4 * m_particlesCount + 1) = p.pos.y;
                m_positionBuffer.at(4 * m_particlesCount + 2) = p.pos.z;                                               
                m_positionBuffer.at(4 * m_particlesCount + 3) = p.size;
                                                
                m_colorBuffer.at(4 * m_particlesCount + 0) = p.r;
                m_colorBuffer.at(4 * m_particlesCount + 1) = p.g;
                m_colorBuffer.at(4 * m_particlesCount + 2) = p.b;
                m_colorBuffer.at(4 * m_particlesCount + 3) = p.a;

            }
            else
            {
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p.cameradistance = -1.0f;
            }

            m_particlesCount++;
        }
    }

    sortParticles();
}

void 
ParticleGenerator::render(void)
{
// Use our shader
// glUseProgram(programID);

    // TODO:: comprobar si es necesario estas dos lineas de inicialización
    m_shader->bind();
    glBindVertexArray(m_vertexArrayVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_positionsVBO);
    glBufferData(GL_ARRAY_BUFFER, m_maxsize * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particlesCount * sizeof(GLfloat) * 4, &m_positionBuffer.front());

    glBindBuffer(GL_ARRAY_BUFFER, m_colorsVBO);
    glBufferData(GL_ARRAY_BUFFER, m_maxsize * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particlesCount * sizeof(GLubyte) * 4, &m_colorBuffer.front());

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(0);
    glDisable(GL_RASTERIZER_DISCARD);

    // Use our shader(antes estaba aquí)

// Dibujar la textura
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->getProgramID());
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    // glUniform1i(TextureID, 0);
    m_shader->setInt("myTextureSampler", 0);

    // Same as the billboards tutorial
// glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
// glUniform3f(CameraUp_worldspace_ID   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
// glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
    m_shader->setVec3("CameraRight_worldspace", m_view[0][0], m_view[1][0], m_view[2][0] );
    m_shader->setVec3("CameraUp_worldspace", m_view[0][1], m_view[1][1], m_view[2][1] );
    glm::mat4 vp { m_projection * m_view };
    m_shader->setMat4("VP", vp);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_billboardVBO);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    
    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionsVBO);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        4,                                // size : x + y + z + size => 4
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : particles' colors
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorsVBO);
    glVertexAttribPointer(
        2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        4,                                // size : r + g + b + a => 4
        GL_UNSIGNED_BYTE,                 // type
        GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // These functions are specific to glDrawArrays*Instanced*.
    // The first parameter is the attribute buffer we're talking about.
    // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
    // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

    // Draw the particules !
    // This draws many times a small triangle_strip (which looks like a quad).
    // This is equivalent to :
    // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
    // but faster.
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_particlesCount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glDepthMask(1);	
    glDisable(GL_BLEND);

    // TODO::  comprobar si este cleanup es necesario 
    glBindVertexArray(0);
    m_shader->unbind();
}

// Funciones privadas

ParticleGenerator::Index 
ParticleGenerator::findUnusedParticle()
{
    for(int i { m_index }; i < m_maxsize; ++i) {
		if (m_particles.at(i).life < 0){
			m_index = i;
			return i;
		}
	}

	for(int i {0}; i < m_index; ++i) {
		if (m_particles.at(i).life < 0){
			m_index = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void 
ParticleGenerator::sortParticles()
{
    std::sort(m_particles.begin(), m_particles.end());
}

} // namespace hyen
