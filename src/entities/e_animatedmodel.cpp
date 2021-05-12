#include "e_animatedmodel.hpp"

namespace hyper {

EAnimatedModel::EAnimatedModel()
: Entity{}
{
}

EAnimatedModel::EAnimatedModel(std::string const& directory, double const& duration)
: Entity{}
{
    loadAnimations(directory, duration);
}

EAnimatedModel::~EAnimatedModel()
{
    for(auto iter = m_animations.begin(); iter != m_animations.end(); ++iter)
    {
		delete iter->second;
        iter->second = nullptr;
    }

    m_animation = nullptr;

    m_animations.clear();
}

void 
EAnimatedModel::draw(glm::mat4 const& transform)
{
    if(m_animation) 
        m_animation->draw(transform, m_shader);
}

void 
EAnimatedModel::loadAnimations(std::string const& directory, double const& duration)
{
    DIR*    dir; 
    dirent* diread;

    for(auto& p : std::filesystem::recursive_directory_iterator(directory))
    {
        if (p.is_directory())
        {
            Animation::Key key = hyper::util::getCurrentFolderName(p.path().string());
            std::vector<std::string> frames;
            auto const& path { p.path().string() };

            INFOLOG(path)
            INFOLOG(key)

            if ((dir = opendir(p.path().c_str())) != nullptr) {
                while ((diread = readdir(dir)) != nullptr) {
                    std::string filename(diread->d_name);

                    INFOLOG(filename);
                    // INFOLOG(filename.length());

                    auto extension = hyper::util::getCurrentFileExtension(diread->d_name);
                    // INFOLOG(extension);

                    if(filename.length() > 2 && extension == "obj")
                    {
                        INFOLOG("Path" << VAR( path + "/" + filename) );
                        frames.push_back(path + "/" + filename);
                    }
                }
                closedir(dir);
            } else {
                perror("opendir");
                return;
            }

            std::sort(frames.begin(), frames.end());

            for(auto& frame : frames)
            {
                // aquí crear cada Animation* y guardarla en el hashmap
                // INFOLOG(path + "/" + frame);
                INFOLOG("frame"  << VAR(frame) );
            }
            // INFOLOG( "*****************************" );
            auto it = m_animations.find(key);

            if(it == m_animations.end())
            {
                INFOLOG("llego?")
                auto* anim { new Animation(key, frames, duration) };
                m_animations[key] = anim;


                if(!m_animation)
                {
                    m_animation = anim; 
                }
            }
            // for(auto& frame : frames)
            // {
            //     // aquí crear cada Animation* y guardarla en el hashmap
            //     INFOLOG(path + "/" + frame);
            // }

            frames.clear();
        }

        // INFOLOG( p.path().string() );

    }
}

void 
EAnimatedModel::updateAnimation(double const& dt)
{
    if(m_animation)
        m_animation->update(dt);
}
    

Animation* const 
EAnimatedModel::getAnimation(Animation::Key key)
{
    auto it = m_animations.find(key);

    if(it != m_animations.end())
    {
        return it->second;
    }

    return nullptr;
}

void 
EAnimatedModel::setAnimation(Animation::Key key)
{
    auto it = m_animations.find(key);

    if(it != m_animations.end())
    {
        if(m_animation != it->second)
        {
            m_animation = it->second;
            m_animation->reset();
        }
    }
}

Animation* const 
EAnimatedModel::getCurrentAnimation(void) const noexcept
{
    return m_animation;
}

}
