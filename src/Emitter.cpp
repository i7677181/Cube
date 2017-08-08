#include "Emitter.h"

Emitter::Emitter(ngl::Vec3 _pos, int _numParticles, ngl::Vec3 *_spread )
{
    /// position of the emitter
    m_pos=_pos;

    /// number of particles to be created
    m_numParticles=_numParticles;

    /// the spread variable to be added to the partilces
    m_spread=_spread;

    /// for loop to create diversity in the partilces with gravity
    for (int i=0; i< m_numParticles; ++i)
    {
        m_particles.push_back(Particle(_pos,m_spread,this));
    }
}

void Emitter::update(GLfloat m_gravity)
{
    ///for loop to update the number of particles with the neccesary values
    for(int i=0; i<m_numParticles; ++i)
    {
        m_particles[i].getMouse(mouse);
        m_particles[i].getRot(rotation);
        m_particles[i].getPos(m_pos);
        m_particles[i].update(m_gravity);
    }
}

void Emitter::draw(bool errupted)
{
    for(int i=0; i<m_numParticles; ++i)
    {
        m_particles[i].draw(errupted);
    }
}
