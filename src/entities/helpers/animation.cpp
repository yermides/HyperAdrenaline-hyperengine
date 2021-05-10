#include "animation.hpp"

namespace hyper {
    
Animation::Animation()
{
}

Animation::Animation(Key const& key, std::vector<std::string> const& frames)
:   m_key(key)
{
    // m_looped = true;
    double const duration { 1.0 / 15.0 };

    for(auto f : frames)
    {
        INFOLOG("f: " << VAR(f))
        auto* mesh = ResourceManager::getResource_t<RMesh>(f);
        INFOLOG("f2?")

        addFrame(mesh, duration);
    }
}

Animation::~Animation()
{
    m_frames.clear();
}

void 
Animation::draw(glm::mat4 const& transform, RShader* const shader)
{
    if(!getFrameCount() || !shader) return;
    
    // Use our shader
    shader->bind();

    // Pass the model matrix
    shader->setMat4("model", transform);

    // Draw the mesh now that everything is setup
    auto& currentFrame { m_frames.at(m_index) };
    if(currentFrame.mesh)
        currentFrame.mesh->draw(shader);

    // Unbind shader, if other shader is being used for other models
    shader->unbind();
}

void 
Animation::update(double const& dt) noexcept
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

        auto const& numFrames { getFrameCount() }; 
        if(m_index >= numFrames)
        {
            if(m_looped)
            {
                m_index -= numFrames;
            }
            else
            {
                m_index = numFrames - 1;
                m_done = true;
            }
        }
    }
}

void 
Animation::addFrame(RMesh* const mesh, double const duration) noexcept
{
    Frame frame { .mesh {mesh}, .duration {duration} };
    m_duration += duration;
    m_frames.push_back(std::move(frame));
}

void 
Animation::setFrameIndex(uint32_t index, bool reset) noexcept
{ 
    uint32_t count { getFrameCount() - 1 };
    m_index = std::min(index, count);

    if(reset)
    {
        m_paused = m_done = false;
        m_elapsed = 0.0;
    }
}

std::size_t 
Animation::getFrameCount(void) const noexcept
{ 
    return m_frames.size();
}

bool 
Animation::getLooped(void) const noexcept
{
    return m_looped;
}

void 
Animation::setLooped(bool looped) noexcept
{
    m_looped = looped;
}

bool 
Animation::getPaused(void) const noexcept
{
    return m_paused;
}

void 
Animation::setPaused(bool paused) noexcept
{
    m_paused = paused;
}

bool 
Animation::getDone(void) const noexcept
{
    return m_done;
}

void 
Animation::setDone(bool done) noexcept
{
    m_index = getFrameCount() - 1;
    m_done = done;
}

} // namespace hyper
