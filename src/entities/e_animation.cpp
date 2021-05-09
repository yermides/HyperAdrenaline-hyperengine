#include "e_animation.hpp"

namespace hyper {

EAnimation::EAnimation() 
:   Entity{}
{
    m_frames.reserve(30);
}

EAnimation::~EAnimation()
{
}

void 
EAnimation::draw(glm::mat4 const& transform)
{
    if(!getFrameCount() || !m_shader) return;
    
    // Use our shader
    m_shader->bind();

    // Pass the model matrix
    m_shader->setMat4("model", transform);

    // Draw the mesh now that everything is setup
    auto& currentFrame { m_frames.at(m_index) };
    if(currentFrame.mesh)
        currentFrame.mesh->draw(m_shader);

    // Unbind shader, if other shader is being used for other models
    m_shader->unbind();
}

void 
EAnimation::update(double const& dt) noexcept
{
    if(m_paused || m_done) return;

    m_elapsed += dt;

    while(m_elapsed >= m_duration)
        m_elapsed -= m_duration;

    auto const& currentFrame = m_frames.at(m_index);

    while(m_elapsed >= currentFrame.duration)
    {
        m_elapsed -= currentFrame.duration;
        ++m_index;

        //  Comportamiento de loop, añadir otros comportamientos
        auto const& numFrames { getFrameCount() }; 
        if(m_index >= numFrames)
        {
            if(m_looped)
            {
                m_index -= numFrames;
            }
            else
            {
                m_index = numFrames -1;
                m_done = true;
            }
        }
    }
}

void 
EAnimation::addFrame(RMesh* const mesh, double const duration) noexcept
{
    Frame frame { .mesh {mesh}, .duration {duration} };
    m_duration += duration;
    m_frames.push_back(std::move(frame));
}

void 
EAnimation::addFrames(std::vector<std::string> const& frames, double const duration) noexcept
{
    for(auto& name : frames) {
        INFOLOG(name); 
        auto* mesh { ResourceManager::getResource_t<RMesh>(name) };
        addFrame(mesh, duration);
    }
}

void 
EAnimation::setFrameIndex(uint32_t index) noexcept
{ 
    uint32_t count { getFrameCount() - 1 };
    m_index = std::min(index, count);
}

std::size_t 
EAnimation::getFrameCount(void) const noexcept
{ 
    return m_frames.size();
}

bool 
EAnimation::getLooped(void) const noexcept
{
    return m_looped;
}

void 
EAnimation::setLooped(bool looped) noexcept
{
    m_looped = looped;
}

bool 
EAnimation::getPaused(void) const noexcept
{
    return m_paused;
}

void 
EAnimation::setPaused(bool paused) noexcept
{
    m_paused = paused;
}

bool 
EAnimation::getDone(void) const noexcept
{
    return m_done;
}

void 
EAnimation::setDone(bool done) noexcept
{
    m_index = getFrameCount() - 1;
    m_done = done;
}
    
} // namespace hyper
