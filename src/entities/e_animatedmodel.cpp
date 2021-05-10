#include "e_animatedmodel.hpp"

namespace hyper {

EAnimatedModel::EAnimatedModel(std::string const& directory)
: Entity{}
{
    loadAnimations(directory);
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
        m_animation->draw(transform);
}

void 
EAnimatedModel::loadAnimations(std::string const& directory)
{
    DIR*    dir; 
    dirent* diread;

    for(auto& p : std::filesystem::recursive_directory_iterator(directory))
    {
        if (p.is_directory())
        {
            Animation::Key key = hyper::util::getCurrentFolderName(p.path().string());
            std::vector<std::string> frames;

            INFOLOG(p.path().string())
            // INFOLOG(key)

            if ((dir = opendir(p.path().c_str())) != nullptr) {
                while ((diread = readdir(dir)) != nullptr) {
                    std::string filename(diread->d_name);

                    // INFOLOG(filename);
                    // INFOLOG(filename.length());

                    auto extension = hyper::util::getCurrentFileExtension(diread->d_name);
                    // INFOLOG(extension);

                    if(filename.length() > 2 && extension == "obj")
                        frames.emplace_back(filename);
                }
                closedir(dir);
            } else {
                perror("opendir");
                return;
            }

            std::sort(frames.begin(), frames.end());
            // INFOLOG( "*****************************" );
            auto const& path { p.path().string() };
            for(auto& frame : frames)
            {
                // aquí crear cada Animation* y guardarla en el hashmap
                INFOLOG(path + "/" + frame);
            }

        }

        // INFOLOG( p.path().string() );

    }
}

}
