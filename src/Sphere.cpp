#include "Sphere.h"
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/NGLStream.h>
//sphere movement class

//---------------------------------------------------------------------------------

Sphere::Sphere(ngl::Vec3 _pos):
    m_pos(_pos),

    m_sphereRotation(0),
    m_rotation(0)


{
    m_pos.m_y=5.5;
    m_velocity.set(1.0, 0.0, 0.0);
}
//---------------------------------------------------------------------------------

//Making the sphere move (input from controls in NGLScene)
void Sphere::move(float _x, float _z)
{
    m_pos.m_x+=_x;
    m_pos.m_z+=_z;

    //Change the position of the sphere when it is at maximum value (cube edge)
    if(m_pos.m_x<m_xmin)
    {
        m_pos.m_x=m_xmax;
        //cube must rotate around the z-axis +vely
        aimRotation= ngl::Vec3(0,0,-90);
        rotation=true;
        timer=0;
        totalZrotation+=-90;
    }
    ///------------------------------------------------------------------------------
    if(m_pos.m_x>m_xmax)
    {
        m_pos.m_x=m_xmin;
        //cube must rotate around the z-axis -vely
        aimRotation=ngl::Vec3(0,0,90);
        rotation=true;
        timer=0;
        totalZrotation+=90;
    }
    ///------------------------------------------------------------------------------

    if(m_pos.m_z<m_zmin)
    {
        m_pos.m_z=m_zmax;
        rotation=true;
        timer=0;

        if(totalZrotation==0){aimRotation=ngl::Vec3(90,0,0);}

        if(totalZrotation==270){aimRotation=ngl::Vec3(0,90,0);}
        if(totalZrotation==-270){aimRotation=ngl::Vec3(0,-90,0);}

        if(totalZrotation==-180){aimRotation=ngl::Vec3(-90,0,0);}
        if(totalZrotation==180){aimRotation=ngl::Vec3(-90,0,0);}

        if(totalZrotation==90){aimRotation=ngl::Vec3(0,-90,0);}
        if(totalZrotation==-90){aimRotation=ngl::Vec3(0,90,0);}

    }
    ///------------------------------------------------------------------------------

    if(m_pos.m_z>m_zmax)
    {
        m_pos.m_z=m_zmin;
        rotation=true;
        timer=0;

        if(totalZrotation==0){aimRotation=ngl::Vec3(-90,0,0);}

        if(totalZrotation==270){aimRotation=ngl::Vec3(0,-90,0);}
        if(totalZrotation==-270){aimRotation=ngl::Vec3(0,90,0);}

        if(totalZrotation==180){aimRotation=ngl::Vec3(90,0,0);}
        if(totalZrotation==-180){aimRotation=ngl::Vec3(90,0,0);}

        if(totalZrotation==90){aimRotation=ngl::Vec3(0,90,0);}
        if(totalZrotation==-90){aimRotation=ngl::Vec3(0,-90,0);}

    }
}

//---------------------------------------------------------------------------------

void Sphere::rotateCube()
{
    int speed = 30;
    ngl::Vec3 rotationPace=aimRotation/speed;

    if (rotation==true)
    {
        m_rotcube+=rotationPace;
        timer++;

        if (m_rotcube.m_x==360||m_rotcube.m_x==-360){m_rotcube.m_x=0;}
        if (m_rotcube.m_y==360||m_rotcube.m_y==-360){m_rotcube.m_y=0;}
        if (m_rotcube.m_z==360||m_rotcube.m_z==-360){m_rotcube.m_z=0;totalZrotation=0;}
    }

    if (timer%speed==0)
    {
        rotation = false;
    }
}


//---------------------------------------------------------------------------------
void Sphere::yMovement()
{
    x+=0.1;
    m_pos.m_y+=0.02*sin(x);
}

//---------------------------------------------------------------------------------

void Sphere::rotate(float _rot)
{
    //sphere rotation (to none for the moment)
    m_sphereRotation+=_rot;
}

//---------------------------------------------------------------------------------

//draw function
void Sphere::draw()
{
    ngl::VAOPrimitives::instance()->draw("sphere");
    yMovement();

    if (rotation==true)
    {
        rotateCube();
    }

}

//---------------------------------------------------------------------------------
//taking the cube dimentions to set the maximum and minimum values

void Sphere::planeEdge(float _width, float _height)
{
    m_xmax = _width;
    m_xmin = -_width;
    m_zmax = _height;
    m_zmin = -_height;

}

void Sphere::setVelocity(float _x, float _y, float _z)
{
    getVelocity();
    m_velocity.set(_x,_y,_z);

}
