#pragma once
#include <glm/vec3.hpp>

namespace hyen {

struct Particle;
struct ParticleGenerator;

struct ParticleGeneratorFunctions {
    // Generar color de las partículas
    static void generateParticleColorsStandard(Particle& p);                // "blanca" y opaca
    static void generateParticleColorsRandomly(Particle& p);                // color blendeado random y opaca
    static void generateParticleTransparencyRandomly(Particle& p);          // "blanca"y transparencia variable
    static void generateParticleColorsAndTransparencyRandomly(Particle& p); // Todo variable
    using ParticleColor = void(*)(Particle&);

    // Generar tamaño de partículas
    static void generateParticleSizeBetween(Particle& p, float min, float max); // Todo variable
    using ParticleSize = void(*)(Particle&, float, float);

    // Generar direcciones random que influyen a la principal
    static glm::vec3 generateRandomDirectionStandard();          // Cero influencia
    static glm::vec3 generateRandomDirectionSoftInfluence();     // Fountain
    static glm::vec3 generateRandomDirectionHeavyInfluence();    // Outburst
    using GeneratorRandomDir = glm::vec3(*)();

    // Generar distribución de la posición
    static void generateRandomPositionBoxShape(ParticleGenerator& generator, Particle& p, glm::vec3 const& mainDir);
    static void generateRandomPositionChaoticSphere(ParticleGenerator& generator, Particle& p, glm::vec3 const& mainDir);
    static void generatePositionStatic(ParticleGenerator& generator, Particle& p, glm::vec3 const& mainDir); 
    static void generatePositionCameraTarget(ParticleGenerator& generator, Particle& p, glm::vec3 const& mainDir);
    using ParticleStartingPosition = void(*)(ParticleGenerator&, Particle&, glm::vec3 const&);


    // Generar main direction
    static glm::vec3 generateMainDirectionStandard(ParticleGenerator& generator);
    static glm::vec3 generateMainDirectionCameraTarget(ParticleGenerator& generator);
    using GeneratorMainDir = glm::vec3(*)(ParticleGenerator& generator);
};

using PGF = ParticleGeneratorFunctions;

} // namespace hyen
