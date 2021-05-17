#pragma once
#include <unordered_map>
#include <entities/entity.hpp>
#include <entities/helpers/animation.hpp>
#include <util/functions.hpp>
#include <util/macros.hpp>
// Para que pueda usar la librería de filesystem
#define __cplusplus 201703L
#include <filesystem>
#include <dirent.h>

template<typename T1, typename T2>
using Hashmap = std::unordered_map<T1, T2>;

namespace hyen {

struct EAnimatedModel : public Entity
{
    EAnimatedModel();
    EAnimatedModel(std::string const& directory, double const& duration = 0.1);
    ~EAnimatedModel();

    void draw(glm::mat4 const& transform) final;

    void loadAnimations(std::string const& directory, double const& duration = 0.1);

    void updateAnimation(double const& dt);

    Animation* const getAnimation(Animation::Key key);
    void setAnimation(Animation::Key key);

    Animation* const getCurrentAnimation(void) const noexcept;

private:
    Hashmap<Animation::Key, Animation*> m_animations;
    Animation* m_animation { nullptr };                 // Puntero a la animación actual
};

}

