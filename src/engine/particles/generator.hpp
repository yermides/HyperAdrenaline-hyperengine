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
        int maxParticles;
        std::string texturePath;

        glm::vec3
            origin
        ,   gravity
        ,   mainDir
        ;

        float 
            particlesPerSecond
        ,   spreadFactor
        ,   lifeSpan
        ,   minParticleSize
        ,   maxParticleSize
        ,   shapeRadius
        ;

        PGF::ParticleColor funcColor         ;
        PGF::ParticleSize funcSize           ;
        PGF::GeneratorRandomDir funcRandomdir;
        PGF::ParticleStartingPosition funcPos;
        PGF::GeneratorMainDir funcMaindir    ;
    };

    explicit ParticleGenerator(RShader* shader, int size = 10000);
    explicit ParticleGenerator(RShader* shader, CInfo const& cInfo);
    ~ParticleGenerator();

    void setMatrices(
        glm::mat4 const& projection
    ,   glm::mat4 const& view
    ,   glm::vec3 const& cameraPosition
    );

    void setMatrices(Node* const camnode);

    void update(float dt);

    void render(void);

    constexpr int getMaxsize(void) const noexcept
        { return m_maxsize;             }
    constexpr float getParticlesPerSecond(void) const noexcept
        { return m_particlesPerSecond;  }
    constexpr glm::vec3 const& getOrigin(void) const noexcept
        { return m_origin;              }
    constexpr glm::vec3 const& getGravity(void) const noexcept
        { return m_gravity;             }
    constexpr glm::vec3 const& getMainDir(void) const noexcept
        { return m_mainDir;             }
    constexpr float getSpreadFactor(void) const noexcept
        { return m_spreadFactor;        }
    constexpr float getLifeSpan(void) const noexcept
        { return m_lifeSpan;            }
    constexpr float getMinParticleSize(void) const noexcept
        { return m_minParticleSize;     }
    constexpr float getMaxParticleSize(void) const noexcept
        { return m_maxParticleSize;     }
    constexpr float getShapeRadius(void) const noexcept
        { return m_shapeRadius;         }
    constexpr PGF::ParticleColor getFuncColor(void) const
        { return m_funcColor;           }
    constexpr PGF::ParticleSize getFuncSize(void) const
        { return m_funcSize;            }
    constexpr PGF::GeneratorRandomDir getFuncRandomdir(void) const
        { return m_funcRandomdir;       }
    constexpr PGF::ParticleStartingPosition getFuncPos(void) const
        { return m_funcPos;             }
    constexpr PGF::GeneratorMainDir getFuncMaindir(void) const
        { return m_funcMaindir;         }
    constexpr RTexture* getTexture(void) const
        { return m_texture;         }

    constexpr void setMaxsize(int i) noexcept
        { m_maxsize = i;                }
    constexpr void setParticlesPerSecond(float f) noexcept
        { m_particlesPerSecond = f;     }
    constexpr void setOrigin(glm::vec3 const& v) noexcept
        { m_origin = v;                 }
    constexpr void setGravity(glm::vec3 const& v) noexcept
        { m_gravity = v;                }
    constexpr void setMainDir(glm::vec3 const& v) noexcept
        { m_mainDir = v;                }
    constexpr void setSpreadFactor(float f) noexcept
        { m_spreadFactor = f;           }
    constexpr void setLifeSpan(float f) noexcept
        { m_lifeSpan = f;               }
    constexpr void setMinParticleSize(float f) noexcept
        { m_minParticleSize = f;        }
    constexpr void setMaxParticleSize(float f) noexcept
        { m_maxParticleSize = f;        }
    constexpr void setShapeRadius(float f) noexcept
        { m_shapeRadius = f;            }
    constexpr void setFuncColor(PGF::ParticleColor func)
        { m_funcColor = func;           }
    constexpr void setFuncSize(PGF::ParticleSize func)
        { m_funcSize = func;            }
    constexpr void setFuncRandomdir(PGF::GeneratorRandomDir func)
        { m_funcRandomdir = func;       }
    constexpr void setFuncPos(PGF::ParticleStartingPosition func)
        { m_funcPos = func;             }
    constexpr void setFuncMaindir(PGF::GeneratorMainDir func)
        { m_funcMaindir = func;         }
    constexpr void setTexture(RTexture* tex)
        { m_texture = tex;              }


    constexpr bool getActive(void) const
        { return m_isActive;         }
    constexpr void setActive(bool value, bool useTimeout = false, float timeout = 0.0f) noexcept
        { 
            if(m_useTimeout && !value) return;

            m_isActive = value;
            m_useTimeout = useTimeout;

            if(useTimeout) 
            {
                m_elapsed = 0.0f;
                m_timeout = timeout;
            }
                
        }

    // constexpr X get(void) const
    //     { return m_;         }
    // constexpr void set(X value) noexcept
    //     { m_ = value;        }

    friend struct ParticleGeneratorFunctions;
private:
    Index findUnusedParticle();
    void sortParticles();

    RShader* m_shader   { nullptr };

    glm::mat4 
        m_projection
    ,   m_view
    ;
    
    glm::vec3 m_cameraPosition  {0};
    glm::vec3 m_cameraTarget    {0};

    Container<Particle> m_particles;
    Container<GLfloat> m_positionBuffer;
    Container<GLubyte> m_colorBuffer;


    Index m_index { 0 };
    int m_particlesCount { 0 }; // Se resetea cada vez que se actualiza, comprueba cúantas partículas vivas hay

    GLuint
    	m_vertexArrayVAO
    ,   m_billboardVBO
    ,   m_positionsVBO
    ,   m_colorsVBO
    ;

    bool m_isActive { true };
    bool m_useTimeout { false };
    float m_elapsed {0.0f}; 
    float m_timeout {0.0f};

    // Parametrizables según en cInfo
    RTexture* m_texture { nullptr };

    int m_maxsize { 10000 };
    float m_particlesPerSecond { 100.0f }; 
    glm::vec3 m_origin { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_gravity { 0.0f, -9.81f, 0.0f };
    glm::vec3 m_mainDir { 0.0f,  10.0f, 0.0f };
    float m_spreadFactor {1.5f};
    float m_lifeSpan {5.0f};
    float m_minParticleSize {0.15f};
    float m_maxParticleSize {0.5f};
    float m_shapeRadius { 3.f };
    PGF::ParticleColor m_funcColor          { nullptr };
    PGF::ParticleSize m_funcSize            { nullptr };
    PGF::GeneratorRandomDir m_funcRandomdir { nullptr };
    PGF::ParticleStartingPosition m_funcPos { nullptr };
    PGF::GeneratorMainDir m_funcMaindir     { nullptr };
};

} // namespace hyen
