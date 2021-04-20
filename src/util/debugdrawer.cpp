#include "debugdrawer.hpp"
// Opengl
#include <GL/gl.h>

namespace hyper {

DebugDrawer::DebugDrawer()
: m_debugMode(DBG_NoDebug)
{
    // m_lines.reserve(default_max_lines_debug);

    // Generar un buffer, poner el resultado en el vertexbuffer que acabamos de crear
    // glGenBuffers(1, &m_vbo);
    // ...
}

DebugDrawer::~DebugDrawer()
{
}

void 
DebugDrawer::setMatrices(glm::mat4 const& view, glm::mat4 const& projection, RShader * const shader)
{
    // glUseProgram(0); // Use Fixed-function pipeline (no shaders)
    // glMatrixMode(GL_MODELVIEW);
    // glLoadMatrixf(&view[0][0]);
    // glMatrixMode(GL_PROJECTION);
    // glLoadMatrixf(&projection[0][0]);

    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
}

void 
DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    // glColor3f(color.x(), color.y(), color.z());

    // glBegin(GL_LINES);
    //     glVertex3f(from.x(), from.y(), from.z());
    //     glVertex3f(to.x(), to.y(), to.z());
    // glEnd();

    // Vertex data
    GLfloat points[12];

    points[0] = from.x();
    points[1] = from.y();
    points[2] = from.z();
    points[3] = color.x();
    points[4] = color.y();
    points[5] = color.z();

    points[6] = to.x();
    points[7] = to.y();
    points[8] = to.z();
    points[9] = color.x();
    points[10] = color.y();
    points[11] = color.z();

    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void 
DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
    // old opengl
    // btVector3 to= PointOnB+normalOnB*1;//distance;
    // const btVector3&from = PointOnB;
    // glColor4f(color.getX(), color.getY(), color.getZ(),1.f);
    // //glColor4f(0,0,0,1.f);
    // glBegin(GL_LINES);
    // glVertex3d(from.getX(), from.getY(), from.getZ());
    // glVertex3d(to.getX(), to.getY(), to.getZ());
    // glEnd();

    btVector3 const startPoint = PointOnB;
	btVector3 const endPoint = PointOnB + normalOnB * distance;
	drawLine( startPoint, endPoint, color );
}

void 
DebugDrawer::reportErrorWarning(const char* warningString)
{
    ERRLOG(warningString);
}

void 
DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
    // old opengl
    // glRasterPos3f(location.x(),  location.y(),  location.z());
}

void 
DebugDrawer::setDebugMode(int debugMode)
{
    m_debugMode = debugMode;
}

int 
DebugDrawer::getDebugMode() const
{
    return m_debugMode;
}

void
DebugDrawer::toggleDebugFlag(int flag)
{
    // checks if a flag is set and enables/disables it

	if (m_debugMode & flag) m_debugMode = m_debugMode & (~flag);
	else m_debugMode |= flag;
}

void 
DebugDrawer::drawAllLines(RShader * const shader)
{
    // shader->bind();

    // // // Los siguientes comandos le darán características especiales al 'vertexbuffer' 
    // // glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // // // Darle nuestros vértices a  OpenGL.
    // // glBufferData(GL_ARRAY_BUFFER, m_lines.size() * sizeof(line3d), &m_lines.front(), GL_STATIC_DRAW);

    // // //load the vertex data info
    // // glVertexAttribPointer(
    // //     this->positionLoc,  // the handle for the a_position shader attrib
    // //     ,   3,	// there are 3 values xyz
    // //     ,   GL_FLOAT, // they a float
    // //     ,   GL_FALSE, // don't need to be normalised
    // //     ,   4*sizeof(float),  // how many floats to the next one(be aware btVector3 uses 4 floats)
    // //     (GLfloat*)&this->TheLines[0]  // where do they start as an index); // use 3 values, but add stride each time to get to the next
    // // );

    // // glEnableVertexAttribArray(this->positionLoc);
    // // glDrawArrays(GL_LINES, 0, TheLines.size()*2);
    

    // m_lines.clear();

    // shader->unbind();
}

}
