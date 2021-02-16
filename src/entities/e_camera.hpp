#pragma once
#include "entity.hpp"

struct ECamera : public Entity
{
    ECamera(/* args */);
    ~ECamera();

    void draw(const glm::mat4& tranform) final;

    void setPerspective(const float& x, const float& y, const float& z, const float& w);
    void setParallel(const float& x, const float& y, const float& z, const float& w);
private:
    bool isPerspective;
    float left, right, lower, upper, close, far;
};
