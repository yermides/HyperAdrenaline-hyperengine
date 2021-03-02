#pragma once
#include <resources/resource.hpp>
// #include <managers/resource_manager.hpp>
#include <resources/helpers/mesh.hpp>
#include <util/macros.hpp>

struct RMesh : public Resource
{
    explicit RMesh();
    ~RMesh();

    void draw();

    void loadMesh(const std::string& filepath);

    void loadFromFile(const std::string& path) final;

    // constexpr uint32_t getVertexCount() const noexcept 
    //     { return m_nVertex; }
private:
    std::vector<Mesh*> m_meshes;
    // uint32_t m_nVertex {0};
};
