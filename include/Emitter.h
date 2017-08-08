#ifndef EMITTER_H_
#define EMITTER_H_
//--------------------------------------------------------------------
/// @file Emitter.h
/// @brief Simple Emitter class
//--------------------------------------------------------------------
#include "Timer.h"
#include "Particle.h"
#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Mat3.h>
#include <vector>
//--------------------------------------------------------------------
/// @class Emitter
/// @brief This class is a simple Emitter for particles for the makeshift volcano
/// @author Tia-Louise Heeger
/// @version 1.0
/// @date 27/05/17///--------------------------------------------------------------------

class Emitter
{
public :
    //--------------------------------------------------------------------
    /// @brief ctor
    /// @param[in] _pos the position of the emitter
    /// @param[in] _numParticles the number of particles to create
    /// @param[in] _spread the spread of the particles
    //--------------------------------------------------------------------
    Emitter( ngl::Vec3 _pos, int _numParticles, ngl::Vec3 *_spread );
    //--------------------------------------------------------------------
    /// @brief a method to update each of the particles contained in the system
    /// @param[in] m_gravity to update each particle with gravity value added
    //--------------------------------------------------------------------
    void update(GLfloat m_gravity);
    //--------------------------------------------------------------------
    /// @brief a method to draw all the particles contained in the system
    //--------------------------------------------------------------------
    void draw(bool);
    //--------------------------------------------------------------------
    /// @brief a method to set the camera
    /// @param[in] pointer to NGL camera
    //--------------------------------------------------------------------
    inline void setCam(ngl::Camera *_cam){m_cam=_cam;}
    //--------------------------------------------------------------------
    /// @brief a method to get the camera
    /// @param[out] returns variable of m_cam
    //--------------------------------------------------------------------
    inline ngl::Camera * getCam()const {return m_cam;}
    //--------------------------------------------------------------------
    /// @brief a function which sets the position of the emitter
    /// @author Arielle Martin
    /// @param[in] _pos is the position of the emitter
    //--------------------------------------------------------------------
    void setPos(ngl::Vec3 _pos){m_pos=_pos;}
    //--------------------------------------------------------------------
    /// @brief a function which passes through the world rotation, taken from NGLscene
    /// @author Arielle Martin
    /// @param[in] _rot is the world's rotation
    //--------------------------------------------------------------------
    void setRot(ngl::Mat4 _rot){rotation=_rot;}
    //--------------------------------------------------------------------
    /// @brief a function which passes through the mouse position from NGLscene, so that the same rotation is applied
    /// @author Arielle Martin
    /// @param[in] _mouse is mouse position
    //--------------------------------------------------------------------
    void setMouse(ngl::Mat4 _mouse){mouse=_mouse;}

private :
    //--------------------------------------------------------------------
    /// @brief the position of the emitter
    //--------------------------------------------------------------------
    ngl::Vec3 m_pos;
    //--------------------------------------------------------------------
    /// @brief the number of particles
    //--------------------------------------------------------------------
    int m_numParticles;
    //--------------------------------------------------------------------
    /// @brief the container for the particles
    //--------------------------------------------------------------------
    std::vector <Particle> m_particles;
    //--------------------------------------------------------------------
    /// @brief a spread vector for the particles
    //--------------------------------------------------------------------
    ngl::Vec3 *m_spread;
    //--------------------------------------------------------------------
    /// @brief a pointer to the camera used for drawing
    //--------------------------------------------------------------------
    ngl::Camera *m_cam;
    //--------------------------------------------------------------------
    /// @brief mouse transformation
    //--------------------------------------------------------------------
    ngl::Mat4 mouse;
    //--------------------------------------------------------------------
    /// @brief world rotation
    //--------------------------------------------------------------------
    ngl::Mat4 rotation;

};


#endif
