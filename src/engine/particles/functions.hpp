#pragma once
#include <glm/vec3.hpp>

namespace hyen {

struct Particle;
struct ParticleGenerator;

struct ParticleGeneratorFunctions {
    static void generateRandomColors(Particle& p);
};

using PGF = ParticleGeneratorFunctions;


using RandomDirFunc = void(*)(glm::vec3&);
void randomDirFunc_Outburst(glm::vec3& v);
void randomDirFunc_Snowwarning(glm::vec3& v);
void randomDirFunc_Static(glm::vec3& v);

using RandomColorFunc = void(*)(uint8_t&);
void randomColorFunc_random255(uint8_t& i);
void randomColorFunc_setToZero(uint8_t& i);
void randomColorFunc_setTo255(uint8_t& i);

using RandomPosFunc = void(*)(glm::vec3&, glm::vec3 const&, glm::vec3 const&);
void randomPosFunc_spawnInsideBox(glm::vec3& output, glm::vec3 const& offset, glm::vec3 const& boxBounds);
// quizá un spawnInsideSphere

using MainDirFunc = void(*)(ParticleGenerator&, glm::vec3 const&);
void mainDirFunc_followCameraTarget(ParticleGenerator& generator, glm::vec3 const& dir = {0,0,0});
// void mainDirFunc_followStaticDirection(ParticleGenerator& generator, glm::vec3 const& dir = {0,0,0});

} // namespace hyen


