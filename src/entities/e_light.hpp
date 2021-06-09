#pragma once
#include "entity.hpp"

namespace hyen {

enum class LightType {
        Directional
    ,   Point
    ,   Spot
};

struct LightIntensity {
    glm::vec3 ambient   {0};
    glm::vec3 diffuse   {0};
    glm::vec3 specular  {0};
};

struct LightAttenuation {
    float constant  {0.0f};
    float linear    {0.0f};
    float quadratic {0.0f};
};

struct LightAperture {
    float innerCutoff   {0.0f};
    float outerCutoff   {0.0f};
};

using LightDirection =  glm::vec3;
using LightID = int;

struct ELight : public Entity
{
    explicit ELight();
    explicit ELight(LightType type, LightIntensity intensity, LightAttenuation attenuation, LightAperture aperture, LightDirection direction);
    ~ELight();

    void draw(glm::mat4 const& tranform) final;

    // void drawFromHyperengine(RShader* const shader);

    constexpr void setType(LightType const& type) noexcept 
        { m_type = type; 			    }
        
    constexpr LightType const& getType(void) const noexcept 
        { return m_type; 				}

    constexpr void setIntensity(LightIntensity const& intensity) noexcept 
        { m_intensity = intensity; 	    }

    constexpr LightIntensity const& getIntensity(void) const noexcept 
        { return m_intensity; 			}

    constexpr void setAttenuation(LightAttenuation const& attenuation) noexcept 
        { m_attenuation = attenuation; 	}
        
    constexpr LightAttenuation const& getAttenuation(void) const noexcept 
        { return m_attenuation; 		}

    constexpr void setAperture(LightAperture const& aperture) noexcept 
        { m_aperture = aperture; 		}
        
    constexpr LightAperture const& getAperture(void) const noexcept
        { return m_aperture;            }

    constexpr void setDirection(LightDirection const& direction) noexcept 
        { m_direction = direction; 		}
        
    constexpr LightDirection const& getDirection(void) const noexcept
        { return m_direction;           }

    constexpr void setID(LightID const& id) noexcept 
        { m_id = id; 		            }
        
    constexpr LightID const& getID(void) const noexcept
        { return m_id;                  }
private:
    LightType m_type;
    LightIntensity m_intensity;
    LightAttenuation m_attenuation;
    LightAperture m_aperture;
    LightDirection m_direction;
    LightID m_id {-1};

    // glm::vec4 m_intensity;
    // LightType m_type;
    // float m_aperture, m_aten_angular;
    // float m_aten_const, m_aten_linear, m_aten_quadratic;
};

}