#pragma once
#include <managers/resource_manager.hpp>
#include <resources/r_material.hpp>
#include <resources/r_texture.hpp>
#include <resources/helpers/mesh.hpp>
#include <util/macros.hpp>

struct RMesh : public Resource
{
    explicit RMesh();    
    explicit RMesh(std::string const& path);
    ~RMesh();

    void draw(ProgramIdentifier const shaderID = 0);

    void loadMesh(std::string const& filepath);

    void loadFromFile(std::string const& path) final;

    // constexpr uint32_t getVertexCount() const noexcept 
    //     { return m_nVertex; }
private:
    std::vector<Mesh*> m_meshes;
    // uint32_t m_nVertex {0};
};
