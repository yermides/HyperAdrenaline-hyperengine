#pragma once
#include <string>
#include "entity.hpp"

// TODO:: include instead of forward
struct RMesh;

struct EModel : public Entity
{
    EModel(/* args */);
    ~EModel();

    void draw(const glm::mat4& tranform) final;
    void loadFromFile(const std::string& path);
private:
    RMesh* mesh;
};
