#pragma once
// C++
#include <vector>
// Bullet physics
#include <bullet/LinearMath/btIDebugDraw.h>
// HyperEngine
#include <resources/r_shader.hpp>
#include <util/macros.hpp>

#define default_max_lines_debug 1000000

namespace hyen {

struct Line3d {
    glm::vec3 from, to, color;
};

struct DebugDrawer : public btIDebugDraw
{
    explicit DebugDrawer();
    ~DebugDrawer();

    void setMatrices(glm::mat4 const& view, glm::mat4 const& projection, RShader * const shader);

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) final;

    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) final;

    void reportErrorWarning(const char* warningString) final;

    void draw3dText(const btVector3& location, const char* textString) final;

    // funciona con un sistema de flags, comprobar btIDebugDraw.h
    void setDebugMode(int debugMode) final;

    int getDebugMode(void) const final;

    void toggleDebugFlag(int flag);

    void drawAllLines(RShader * const shader);

private:
    struct line3d { btVector3 from, to /*, color */; };

    int m_debugMode;
    uint32_t m_vao, m_vbo; // para los datos de las lineas
    
    // std::vector<line3d> m_lines;
};

}
