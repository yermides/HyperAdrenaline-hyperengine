#include "debugdrawer.hpp"
// Opengl
#include <GL/gl.h>

namespace hyper {

DebugDrawer::DebugDrawer()
: m_debugMode(DBG_NoDebug)
{
    m_lines.reserve(default_max_lines_debug);

    // Generar un buffer, poner el resultado en el vertexbuffer que acabamos de crear
    // glGenBuffers(1, &m_vbo);
    // ...
}

DebugDrawer::~DebugDrawer()
{
}

void 
DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
}

void 
DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    line3d line{from, to/*, color */};
    m_lines.push_back(std::move(line));
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
}

void 
DebugDrawer::reportErrorWarning(const char* warningString)
{
    ERRLOG(warningString)
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
	if (m_debugMode & flag)
		// flag is enabled, so disable it
		m_debugMode = m_debugMode & (~flag);
	else
		// flag is disabled, so enable it
		m_debugMode |= flag;
}

void 
DebugDrawer::drawAllLines(RShader * const shader)
{
    shader->bind();

    // // Los siguientes comandos le darán características especiales al 'vertexbuffer' 
    // glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // // Darle nuestros vértices a  OpenGL.
    // glBufferData(GL_ARRAY_BUFFER, m_lines.size() * sizeof(line3d), &m_lines.front(), GL_STATIC_DRAW);

    // //load the vertex data info
    // glVertexAttribPointer(
    //     this->positionLoc,  // the handle for the a_position shader attrib
    //     ,   3,	// there are 3 values xyz
    //     ,   GL_FLOAT, // they a float
    //     ,   GL_FALSE, // don't need to be normalised
    //     ,   4*sizeof(float),  // how many floats to the next one(be aware btVector3 uses 4 floats)
    //     (GLfloat*)&this->TheLines[0]  // where do they start as an index); // use 3 values, but add stride each time to get to the next
    // );

    // glEnableVertexAttribArray(this->positionLoc);
    // glDrawArrays(GL_LINES, 0, TheLines.size()*2);
    

    m_lines.clear();

    shader->unbind();
}

}
