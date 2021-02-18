#pragma once
#include "entity.hpp"

enum class LightType {
	Directional,
	Point,
	Spot
};

struct ELight : public Entity
{
	ELight(/* args */);
	~ELight();

	void draw(const glm::mat4& tranform) final;

	constexpr	void            	setIntensity(const glm::vec4& newIntensity)     noexcept 
		{ m_intensity = newIntensity; 	}

	constexpr	const glm::vec4&	getIntensity(void)                              const noexcept 
		{ return m_intensity; 			}

	constexpr	void            	setType(const LightType& newType)               noexcept 
		{ m_type = newType; 			}
		
	constexpr	const LightType&	getType(void)                                   const noexcept 
		{ return m_type; 				}
		
	constexpr	void            	setAperture(const float& newfloat)              noexcept 
		{ m_aperture = newfloat; 		}
		
	constexpr	const float&    	getAperture(void)                               const noexcept 
		{ return m_aperture; 			}
		
	constexpr	void            	setAtenAngular(const float& newfloat)           noexcept 
		{ m_aten_angular = newfloat; 	}
		
	constexpr	const float&    	getAtenAngular(void)                            const noexcept 
		{ return m_aten_angular; 		}
		
	constexpr	void            	setAtenConst(const float& newfloat)             noexcept 
		{ m_aten_const = newfloat; 		}
		
	constexpr	const float&    	getAtenConst(void)                              const noexcept 
		{ return m_aten_const; 			}
		
	constexpr	void            	setAtenLinear(const float& newfloat)            noexcept 
		{ m_aten_linear = newfloat; 	}
		
	constexpr	const float&    	getAtenLinear(void)                             const noexcept 
		{ return m_aten_linear; 		}
		
	constexpr	void            	setAtenQuadratic(const float& newfloat)         noexcept 
		{ m_aten_quadratic = newfloat; 	}
		
	constexpr	const float&    	getAtenQuadratic(void)                         	const noexcept 
		{ return m_aten_quadratic; 		}

private:
	glm::vec4 m_intensity;
	LightType m_type;
	float m_aperture, m_aten_angular;
	float m_aten_const, m_aten_linear, m_aten_quadratic;
};
