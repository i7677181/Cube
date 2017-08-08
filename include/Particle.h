#ifndef PARTICLE_H__
#define PARTICLE_H__
//--------------------------------------------------------------------
/// @file Particle.h
/// @brief Simple Particle class
//--------------------------------------------------------------------
#include <ngl/Vec3.h>
#include <ngl/Colour.h>
#include <stdio.h>
#include <ngl/NGLStream.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <ngl/VertexArrayObject.h>
#include <QSet>
#include <memory>
#include <QOpenGLWindow>
//--------------------------------------------------------------------
/// @class Paricle
/// @brief This class is to define attributes for the particles for the makeshift volcano
/// @author Tia-Louise Heeger
/// @version 1.0
/// @date 27/05/17
//--------------------------------------------------------------------

class Emitter;
class Timer;
class Particle
{
public :
    //--------------------------------------------------------------------
    /// @brief ctor
    /// @param[in] _pos the position of the emitter
    /// @param[in] _spread the spread of the particles
    /// @param[in] _emitter the attributes from the emitter
    //--------------------------------------------------------------------
    Particle(ngl::Vec3 _pos,ngl::Vec3 *_spread, Emitter *_emitter	);
    //--------------------------------------------------------------------
    /// @brief a method to update each of the particles contained in the system
    /// @param[in] m_gravity to update each particle with gravity value added
    //--------------------------------------------------------------------
    void update(GLfloat _gravity);
    //--------------------------------------------------------------------
    /// @brief a method to draw the particle
    //--------------------------------------------------------------------
    void draw(bool);
    //--------------------------------------------------------------------
    /// @brief function which gets the position of the emitter
    /// @author Arielle Martin
    /// @param [in] _pos is the emitter's position
    //--------------------------------------------------------------------
    void getPos(ngl::Vec3 _pos){m_origin=_pos;}
    //--------------------------------------------------------------------
    /// @brief function which gets the rotation of the world
    /// @author Arielle Martin
    /// @param [in] _rot is the rotation of the world
    //--------------------------------------------------------------------
    void getRot(ngl::Mat4 _rot){rotation=_rot;}
    //--------------------------------------------------------------------
    /// @brief function which gets the mouse transformation
    /// @author Arielle Martin
    /// @param [in] _mouse is the mouse transformation
    //--------------------------------------------------------------------
    void getMouse(ngl::Mat4 _mouse){mouse=_mouse;}

private :
    //--------------------------------------------------------------------
    /// @brief the position of the partilces
    //--------------------------------------------------------------------
    ngl::Vec3 m_pos;
    //--------------------------------------------------------------------
    /// @brief the origin of the partilce
    //--------------------------------------------------------------------
    ngl::Vec3 m_origin;
    //--------------------------------------------------------------------
    /// @brief the direction of the partilce
    //--------------------------------------------------------------------
    ngl::Vec3 m_dir;
    //--------------------------------------------------------------------
    /// @brief pointer to the spread value for the particles
    //--------------------------------------------------------------------
    ngl::Vec3 *m_spread;
    //--------------------------------------------------------------------
    /// @brief the position of the emitter
    //--------------------------------------------------------------------
    ngl::Vec3 m_emitterPos;
    //--------------------------------------------------------------------
    ///@brief the world's rotation
    //--------------------------------------------------------------------
    ngl::Mat4 rotation;
    //--------------------------------------------------------------------
    ///@brief the mouse transformation matrix
    //--------------------------------------------------------------------
    ngl::Mat4 mouse;
    //--------------------------------------------------------------------
    ///@brief the rotation of the emitter
    //--------------------------------------------------------------------
    ngl::Mat4 initial_rotation;
    //--------------------------------------------------------------------
    /// @brief attribute for the colour of the particle
    //--------------------------------------------------------------------
    ngl::Real red;
    //--------------------------------------------------------------------
    /// @brief gravity to be added to the particles
    //--------------------------------------------------------------------
    GLfloat m_gravity;
    //--------------------------------------------------------------------
    /// @brief the lifespan of the particles
    //--------------------------------------------------------------------
    float m_currentLife;
    //--------------------------------------------------------------------
    /// @brief pointer to the emitter
    //--------------------------------------------------------------------
    const Emitter *m_emitter;
};

#endif
