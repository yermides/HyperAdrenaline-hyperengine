#include "e_animation.hpp"

namespace hyper {

EAnimation::EAnimation() 
:   Entity{}
{
    m_frames.reserve(10);
}

EAnimation::~EAnimation()
{
}

void 
EAnimation::draw(glm::mat4 const& tranform)
{
    if(!m_shader) return;
    
    // Use our shader
    m_shader->bind();

    // Pass the model matrix
    m_shader->setMat4("model", tranform);

    // Draw the mesh now that everything is setup
    if(m_currentFrame)
        m_currentFrame->draw(m_shader);

    // Unbind shader, if other shader is being used for other models
    m_shader->unbind();
}

void 
EAnimation::addFrame(Frame frame) noexcept
{
    if(!m_currentFrame)
        m_currentFrame = frame;

    m_frames.push_back(frame);
}

void 
EAnimation::setFrameIndex(uint32_t index) noexcept
{ 
    uint32_t count = getFrameCount()-1;
    if(!count) return;
    index = std::min(index, count);
    m_currentFrame = m_frames.at(index);
}

std::size_t 
EAnimation::getFrameCount(void) const noexcept
{ 
    return m_frames.size();
}
    
} // namespace hyper
