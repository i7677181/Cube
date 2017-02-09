#ifndef ENEMY_H
#define ENEMY_H
#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include "Sphere.h"

#include <memory>

class Enemy
{
public:
    Enemy(ngl::Vec3 _pos);
    Enemy() = default;
    void move(const ngl::Vec3 &m_spherePos);
    void draw();
    void setSteer();
    inline ngl::Vec3 getPosition() {return m_pos;}
    bool prey;
    void setPrey(const ngl::Vec3 &m_spherePos);
    float distanceCalc(ngl::Vec3 a ,ngl::Vec3 b);
    void updatePosition();



    void setHit(bool _h){m_hit=_h;}
    bool isHit() const {return m_hit;}
   bool m_hit=false;




private:
    ngl::Vec3 m_pos;
    ngl::Vec3 m_prey;
    ngl::Vec3 m_spherePos;
    ngl::Vec3 m_flee;
    ngl::Vec3 m_target;
    ngl::Vec3 m_steering;
    ngl::Vec3 m_velocity;
    ngl::Vec3 desiredVelocity;
    ngl::Vec3 m_goal;
    float m_prevDistance;
    float m_speed;
    float m_maxAccelerate;
    float m_randAccel;
    float m_xmax;
    float m_xmin;
    float m_zmax;
    float m_zmin;
   float m_maxSpeed;
    float   m_minSpeed;
    void setChase(ngl::Vec3 _pos, float _targetSpeed);
void setTarget();
void setRotation();
void kill();
float m_yaw,m_pitch;

ngl::Vec3 steerForSeek ();
void fleeWalls();


};

#endif // ENEMY_H
