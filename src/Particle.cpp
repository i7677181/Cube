#include "Particle.h"
#include "Emitter.h"
#include <ngl/Camera.h>
#include <ngl/Random.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>


Particle::Particle(ngl::Vec3 _pos, ngl::Vec3 *_spread,  Emitter *_emitter   )
{
    ///position of the particles
    m_pos=_pos;

    ///origin of the partilces
    m_origin=_pos;

    ///spread values of the partilces
    m_spread=_spread;

    ///random value for colour attribute
    ngl::Random *rand=ngl::Random::instance();

    ///random value to x direction of the particles
    m_dir.m_x=rand->randomNumber(5)+0.5;

    ///random value to y direction of the particles
    m_dir.m_y=rand->randomPositiveNumber(10)+0.5;

    ///random value to x direction of the particles
    m_dir.m_z=rand->randomNumber(5)+0.5;

    ///current life of the particles
    m_currentLife=0.0;

    ///emitter variable
    m_emitter=_emitter;

    initial_rotation.euler(90,0,0,1);
}

void Particle::update(GLfloat _gravity)
{
    ///current life value of the particles
    m_currentLife+=0.05;

    ///gravity to be added to the particles
    m_gravity=_gravity;

    ///calculation of movement of the particles with the spread,xDirection and current life
    m_pos.m_x=m_origin.m_x+(m_spread->m_x*m_dir.m_x*m_currentLife);

    ///calculation of movement of the particles with the spread,yDirectio, gravity and current life
    m_pos.m_y= m_origin.m_y+(m_spread->m_y*m_dir.m_y*m_currentLife)+m_gravity*(m_currentLife*m_currentLife);

    ///calculation of movement of the particles with the spread,zDirection and current life
    m_pos.m_z=m_origin.m_z+(m_spread->m_z*m_dir.m_z*m_currentLife);

    /// if the y position goes below the origin position reset
    if(m_pos.m_y <= m_origin.m_y-0.01)
    {
        m_pos=m_origin;
        m_currentLife=0.0;
        ngl::Random *rand=ngl::Random::instance();
        m_dir.m_x=rand->randomNumber(5)+0.5;
        m_dir.m_y=rand->randomPositiveNumber(10)+0.5;
        m_dir.m_z=rand->randomNumber(5)+0.5;

    }
}

void Particle::draw(bool errupted)
{
    /// instance of the VAO primitives
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

    /// Transformation
    ngl::Transformation transform;

    /// instance of the shader library
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    /// use phong shader
    (*shader)["Phong"]->use();

    /// transformation with position
    transform.setPosition(m_pos);
    /// adding the rotation to the MVP matrix
    ngl::Mat4 MVP=transform.getMatrix()*initial_rotation*rotation*mouse*m_emitter->getCam()->getViewMatrix()*m_emitter->getCam()->getProjectionMatrix();
    shader->setShaderParamFromMat4("MVP",MVP);
    /// instance of random
    ngl::Random *rand=ngl::Random::instance();

    /// set value of red to a random number
    red = rand->randomNumber()
            ;
    /// set shader parameter to flag 1 for flat diffuse
    shader->setUniform("myflag",1);

    /// set shader paramter for a grey colour
    shader->setUniform("Colour",0.5,0.5f,0.5f);

    /// if statement to change colour if the volcano has errupted
    if(errupted==true)
    {
        /// shader parameter set to red
        shader->setUniform("Colour", red,0.2f, 0.2f);//Set colour for flat diffuse colour shader
    }
    /// draw sphere for particles
    prim->draw("sphere");

}

