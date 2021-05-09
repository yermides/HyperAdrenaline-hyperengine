#pragma once
#include <vector>
#include <entities/entity.hpp>
#include <resources/r_mesh.hpp>

// Esta clase es como una especie de EAnimatedModel, realmente debería ser un EAnimatedModel
// con un vector de Animation structs que sean un vector de Frames, y estos a su vez un Rmesh* y una duración

namespace hyper {

struct Frame {
    RMesh* const mesh;        // Malla en la timestamp X
    double duration;    // Tiempo en segundos
};

struct EAnimation : public Entity
{
    // using Frame = RMesh*;   // Esto luego cambiará
    using MeshFrames = std::vector<Frame>;

    EAnimation();
    ~EAnimation();

    void draw(glm::mat4 const& transform) final;

    void update(double const& dt = (1.0 / 60.0)) noexcept;

    void addFrame(RMesh* const mesh, double const duration = 0.2) noexcept; 

    void addFrames(std::vector<std::string> const& frames, double const duration = 0.2) noexcept; 

    void setFrameIndex(uint32_t index) noexcept;

    std::size_t getFrameCount(void) const noexcept;

    bool getLooped(void) const noexcept;
    void setLooped(bool) noexcept;

    bool getPaused(void) const noexcept;
    void setPaused(bool) noexcept;

    bool getDone(void) const noexcept;
    void setDone(bool) noexcept;
private:
    MeshFrames m_frames;
    double m_duration       { 0.0   };
    double m_elapsed        { 0.0   };
    uint32_t m_index        { 0     };

    // flags
    bool m_looped           { false };
    bool m_paused           { false };
    bool m_done             { false };
};

} // namespace hyper