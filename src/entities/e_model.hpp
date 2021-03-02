#pragma once
#include <string>
#include <managers/resource_manager.hpp>
#include <resources/r_mesh.hpp>
#include "entity.hpp"

struct EModel : public Entity
{
    EModel();
    ~EModel();

    void draw(const glm::mat4& tranform) final;
    void loadFromFile(const std::string& path);
private:
    RMesh* m_rmesh { nullptr };
};
