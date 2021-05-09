#pragma once
#include <vector>
#include <entities/entity.hpp>
#include <resources/r_mesh.hpp>

// Esta clase es como una especie de EAnimatedModel, realmente debería ser un EAnimatedModel
// con un vector de Animation structs que sean un vector de Frames, y estos a su vez un Rmesh* y una duración

namespace hyper {

struct EAnimation : public Entity
{
    using Frame = RMesh*;   // Esto luego cambiará
    using MeshFrames = std::vector<RMesh*>;

    EAnimation();
    ~EAnimation();

    void draw(glm::mat4 const& tranform) final;

    void addFrame(Frame frame) noexcept; 

    void setFrameIndex(uint32_t index) noexcept;

    std::size_t getFrameCount(void) const noexcept;
private:
    MeshFrames m_frames;
    RMesh* m_currentFrame   { nullptr };
};

} // namespace hyper