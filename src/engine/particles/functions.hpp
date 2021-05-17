#pragma once
#include <glm/vec3.hpp>

namespace hyen {

using RandomDirFunc = void(*)(glm::vec3&);
void randomDirFunc_Outburst(glm::vec3& v);
void randomDirFunc_Snowwarning(glm::vec3& v);
void randomDirFunc_Static(glm::vec3& v);

using RandomColorFunc = void(*)(uint8_t&);
void randomColorFunc_random255(uint8_t& i);
void randomColorFunc_setToZero(uint8_t& i);
void randomColorFunc_setTo255(uint8_t& i);

using RandomPosFunc = void(*)(uint8_t&);
void randomPosFunc_spawnInsideBox(glm::vec3& output, glm::vec3 const& offset, glm::vec3 const& boxBounds);
// quizá un spawnInsideSphere
    
} // namespace hyen


