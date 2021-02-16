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

    void                setIntensity(const glm::vec4& newIntensity);
    const glm::vec4&    getIntensity();
    void                setType(const LightType& newType);
    const LightType&    getType();
    void                setAperture(const float& newfloat);
    const float&        getAperture();
    void                setAtenAngular(const float& newfloat);
    const float&        getAtenAngular();
    void                setAtenConst(const float& newfloat);
    const float&        getAtenConst();
    void                setAtenLinear(const float& newfloat);
    const float&        getAtenLinear();
    void                setAtenQuadratic(const float& newfloat);
    const float&        getAtenQuadratic();
private:
    glm::vec4 intensity;
    LightType type;
    float aperture, aten_angular;
    float aten_const, aten_linear, aten_quadratic;
};
