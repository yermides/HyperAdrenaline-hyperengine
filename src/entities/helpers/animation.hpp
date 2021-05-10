#pragma once
#include <vector>
#include <string>
#include <glm/matrix.hpp>

namespace hyper {

struct Animation
{
    using Key = std::string;

    Animation();
    Animation(Key key, std::vector<std::string> const& frames); // rutas de los modelos
    ~Animation();

    void draw(glm::mat4 const& transform);

private:
    Key m_key;
};

}
