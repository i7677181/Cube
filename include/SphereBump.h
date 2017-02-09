#ifndef SPHEREBUMP_H__
#define SPHEREBUMP_H__

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

class SphereBump
{
public:

    SphereBump(ngl::Vec3 _pos);
   void draw();

    ngl::Vec3 getPosition() {return m_pos;}
    void setHit(bool _h){m_hit=_h;}
    bool isHit() const {return m_hit;}

private:
    ngl::Vec3 m_pos;

    const static float s_increment;
    ngl::Transformation m_transform;
    const ngl::Camera *m_camera;
    bool m_hit=false;

};

#endif
