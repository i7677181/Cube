#ifndef SPHERE_H__
#define SPHERE_H__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

class Sphere
{
public:

    Sphere(ngl::Vec3 _pos);

    void move(float _x, float _z);
    void rotate (float _rot);
    void draw();
    void yMovement();
    void rotateCube();

    void planeEdge(float _width, float _height);

    inline ngl::Vec3 getPosition() {return m_pos;}
    inline ngl::Vec3 setCubeRotation() {return m_rotcube;}

    ngl::Vec3 m_velocity;
    ngl::Vec3 getVelocity() const {return m_velocity;}
    void setVelocity(float _x, float _y, float _z);

private:


    ngl::Vec3 m_pos;
    ngl::Vec3 m_rotcube;

    const static float s_increment;

    ngl::Transformation m_transform;
    float m_sphereRotation;
    const ngl::Camera *m_camera;

    int timer=0;

    ngl::Vec3 aimRotation;
    int totalZrotation=0;

    bool rotation;



    float m_rotation;
    float m_xmax;
    float m_xmin;
    float m_zmax;
    float m_zmin;

   float x=0;
};

#endif
