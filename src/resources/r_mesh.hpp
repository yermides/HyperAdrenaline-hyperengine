#pragma once
#include <managers/resource_manager.hpp>
#include <resources/r_material.hpp>
#include <resources/r_texture.hpp>
#include <resources/r_shader.hpp>
#include <resources/helpers/mesh.hpp>
#include <util/macros.hpp>
#include <util/functions.hpp>

namespace hyen {

struct BoneInfo {
    glm::mat4 offset    {1.0f}; // Por defecto la identidad
    uint32_t id          { 0 }; // Por defecto cero
};

struct RMesh : public Resource
{
    explicit RMesh();    
    explicit RMesh(std::string const& path);
    ~RMesh();

    void draw(RShader* const shader);

    void loadMesh(std::string const& filepath);

    void loadFromFile(std::string const& path) final;

    friend class EModel;
private:
    std::vector<Mesh*> m_meshes;
};

}