#pragma once
#include <vector>
#include <string>
#include <glm/matrix.hpp>
#include <resources/r_shader.hpp>
#include <resources/r_mesh.hpp>

namespace hyper {

struct Animation
{
    struct Frame {
        RMesh* const mesh;
        double duration;
    };

    using Key = std::string;

    Animation();
    Animation(Key const& key, std::vector<std::string> const& frames); // rutas de los modelos
    ~Animation();

    void draw(glm::mat4 const& transform, RShader* const shader);

    void update(double const& dt = (1.0 / 60.0)) noexcept;

    void addFrame(RMesh* const mesh, double const duration = (1.0 / 15.0)) noexcept; 

    void setFrameIndex(uint32_t index, bool reset = false) noexcept;

    std::size_t getFrameCount(void) const noexcept;

    bool getLooped(void) const noexcept;
    void setLooped(bool) noexcept;
    bool getPaused(void) const noexcept;
    void setPaused(bool) noexcept;
    bool getDone(void) const noexcept;
    void setDone(bool) noexcept;

private:
    Key m_key;
    std::vector<Frame>      m_frames;
    double m_duration       { 0.0   };
    double m_elapsed        { 0.0   };
    uint32_t m_index        { 0     };

    // // flags
    bool m_looped           { false };
    bool m_paused           { false };
    bool m_done             { false };
};

}
