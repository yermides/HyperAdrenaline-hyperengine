#pragma once
#include <managers/resource_manager.hpp>
#include <resources/r_mesh.hpp>
#include <entities/entity.hpp>

namespace hyper {

struct EModel : public Entity
{
    explicit EModel();
    explicit EModel(std::string const& path);
    ~EModel();

    void draw(glm::mat4 const& tranform) final;
    void loadFromFile(std::string const& path);

    std::vector<float> const getVertexPositions(void);
    std::vector<float> const getVertexNormals(void);
    std::vector<float> const getVertexIndices(void);
private:
    RMesh* m_rmesh { nullptr };
};

}