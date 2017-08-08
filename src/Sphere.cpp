#include "Sphere.h"
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/NGLStream.h>
#include <math.h>
#include <algorithm>
//sphere movement class

//---------------------------------------------------------------------------------
/// Initialising
//---------------------------------------------------------------------------------
Sphere::Sphere(ngl::Vec3 _pos)
{
    m_pos=_pos;
    m_hit=false;
    m_pos.m_y=5.3;
    //creating terrain object (made by Arnold Sule)
    m_terrain.reset(new Terrain);
    //initializing terrain heights for terrain 1 to make the player move on top of it (made by Arnold Sule)
    m_terrainHeights = m_terrain->terrainHeight(1,0,false);
}

///----------------------------------------------------------------------------------------------------------------------
/// Sets the position of the first and the last cube of each terrain (opposite corners)
/// This function was made by Arnold Sule
///----------------------------------------------------------------------------------------------------------------------

void Sphere::setFirstAndLastCubePositions(ngl::Vec3 _cubes[7][2])
{
    for (int j=1; j<7; j++)
    {
        for (int k=0; k<2; k++)
        {
            m_cubes[j][k]=_cubes[j][k];
        }
    }
}
//---------------------------------------------------------------------------------
/// Making the sphere move (input from NGLScene)
/// Every time the character gets to the edge of the cube its position is reset
/// When the edge is reached the cube rotates
//---------------------------------------------------------------------------------
void Sphere::move(float _x, float _z)
{
    m_lastPos=m_pos;
    m_hit=false;
    //increments the player's position in the x direction
    m_pos.m_x+=_x;
    //increments the player's position in the z direction
    m_pos.m_z+=_z;
    // cubeCounter increments while looping through each small cube in the array (made by Arnold Sule)
    float cubeCounter=0;
    // helps to calculate how high the player has to be moved on the y axis to move on top of the terrain (made by Arnold Sule)
    float elevationVar=6;
    //loops through the terrain and depending on the height of the cubes, moves the player on the y axis (made by Arnold Sule)
    for(float i=-5; i<5; i+=0.25f)
    {
        for(float j=-5; j<5; j+=0.25f)
        {
            //defining the area of one small cube unit (made by Arnold Sule)
            if(m_pos.m_x>j && m_pos.m_x<j+1 && m_pos.m_z>i && m_pos.m_z<i+1)
            {
                for (int t=1; t<9; t++)
                {
                //moves the player on top of the unit
                    if (m_terrainHeights[cubeCounter]==t)
                    {
                        elevationVar=(-0.8f)*t+5.1f;
                        m_pos.m_y= m_terrainHeights[cubeCounter]+elevationVar;
                    }
                }
            }
        cubeCounter++;
        }
    }

    if(m_pos.m_x<m_xmin)                                        // character reaches the left edge
    {
        m_pos.m_x = m_xmax-0.2;                                 // resets the player's position back to the other side of the cube
        m_rotation.identity();                                  // setting the rotation back to zero
        m_rotation.euler(-15,0,0,1);                            // rotation around the -ve z-axis
        rotateWorld=true;                                       // rotation is true
    }
    else if(m_pos.m_x>m_xmax)                                   // character reaches the right edge
    {
        m_pos.m_x=m_xmin+0.2;                                   // resets the player's position back to the other side of the cube
        m_rotation.identity();                                  // setting the rotation back to zero
        m_rotation.euler(15,0,0,1);                             // rotation around the +ve z-axis
        rotateWorld=true;                                       // rotation is true
    }
                                                                // character reaches the far edge
    else if(m_pos.m_z<m_zmin)
    {
        m_pos.m_z=m_zmax-0.2;                                   // resets the player's position back to the other side of the cube
        m_rotation.identity();                                  // setting the rotation back to zero
        m_rotation.euler(15,1,0,0);                             // rotation around the +ve x-axis
        rotateWorld=true;                                       // rotation is true
    }
                                                                // character reaches the front edge
    else if(m_pos.m_z>m_zmax)
    {
        m_pos.m_z=m_zmin+0.2;                                   // resets the player's position back to the other side of the cube
        m_rotation.identity();                                  // setting the rotation back to zero
        m_rotation.euler(-15,1,0,0);                            // rotation around the -ve x-axis
        rotateWorld=true;                                       // rotation is true
    }
}

//---------------------------------------------------------------------------------
/// Draw function
//---------------------------------------------------------------------------------
void Sphere::draw()
{
    ngl::VAOPrimitives::instance()->draw("sphere");
    x+=0.1;
}

//---------------------------------------------------------------------------------
/// Setting the maximum and minimum values of the cube's edge
//---------------------------------------------------------------------------------
void Sphere::planeEdge(float _width, float _height)
{
    m_xmax = _width;
    m_xmin = -_width;
    m_zmax = _height;
    m_zmin = -_height;
}
//---------------------------------------------------------------------------------
/// Setting the current position to the last position
//---------------------------------------------------------------------------------
void Sphere::sendBack()
{
    m_pos = m_lastPos;
}

//----------------------------------------------------------------------------------------------------------------------
/// The positioning of the terrain has to be tracked as the player is switching sides
/// When entering a terrain from different sides of the world, the terrain information has to rotate with it
/// Function written by Arnold Sule
//----------------------------------------------------------------------------------------------------------------------
void Sphere::trackTerrain()
{
    // rotation angle of the terrain image
    int terrainRotation;
    // vectors to store the first and the last cube position (opposite corners on one side) of all six sides of the terrain
    // this helps to calculate which where are the different sides of the world are facing
    ngl::Vec3 t1_1=m_cubes[1][1];
    ngl::Vec3 t2_1=m_cubes[2][1];
    ngl::Vec3 t3_1=m_cubes[3][1];
    ngl::Vec3 t4_1=m_cubes[4][1];
    ngl::Vec3 t5_1=m_cubes[5][1];
    ngl::Vec3 t6_1=m_cubes[6][1];
    ngl::Vec3 t1_2=m_cubes[1][0];
    ngl::Vec3 t2_2=m_cubes[2][0];
    ngl::Vec3 t3_2=m_cubes[3][0];
    ngl::Vec3 t4_2=m_cubes[4][0];
    ngl::Vec3 t5_2=m_cubes[5][0];
    ngl::Vec3 t6_2=m_cubes[6][0];
    //checks if terrain 1 is facing upwards
    if (t1_1.m_y > 0 && t1_2.m_y > 0)
    {
        //checks how the terrain is rotated and stores the angle
        if (t1_1.m_x > 0 && t1_1.m_z > 0) {terrainRotation=0;}
        else if (t1_1.m_x > 0 && t1_1.m_z < 0) {terrainRotation=90;}
        else if (t1_1.m_x < 0 && t1_1.m_z < 0) {terrainRotation=180;}
        else if (t1_1.m_x < 0 && t1_1.m_z > 0) {terrainRotation=270;}
        // passing the rotation angle to the terrainHeight function to recalculate the heights depending on the angle
        m_terrainHeights = m_terrain->terrainHeight(1,terrainRotation, false);
        isForest=true;
        isGarden=false;
        m_volcanoEruption=false;
    }
    //checks if terrain 2 is facing upwards
    else if (t2_1.m_y > 0 && t2_2.m_y > 0)
    {
        //checks how the terrain is rotated and stores the angle
        if (t2_1.m_x > 0 && t2_1.m_z > 0) {terrainRotation=270;}
        else if (t2_1.m_x > 0 && t2_1.m_z < 0) {terrainRotation=0;}
        else if (t2_1.m_x < 0 && t2_1.m_z < 0) {terrainRotation=90;}
        else if (t2_1.m_x < 0 && t2_1.m_z > 0) {terrainRotation=180;}
        // passing the rotation angle to the terrainHeight function to recalculate the heights depending on the angle
        m_terrainHeights = m_terrain->terrainHeight(2,terrainRotation, true);
        isForest=false;
        isGarden=true;
        m_volcanoEruption=false;
    }
    //checks if terrain 3 is facing upwards
    else if (t3_1.m_y > 0 && t3_2.m_y > 0)
    {
        //checks how the terrain is rotated and stores the angle
        if (t3_1.m_x > 0 && t3_1.m_z > 0) {terrainRotation=270;}
        else if (t3_1.m_x > 0 && t3_1.m_z < 0) {terrainRotation=0;}
        else if (t3_1.m_x < 0 && t3_1.m_z < 0) {terrainRotation=90;}
        else if (t3_1.m_x < 0 && t3_1.m_z > 0) {terrainRotation=180;}
        // passing the rotation angle to the terrainHeight function to recalculate the heights depending on the angle
        m_terrainHeights = m_terrain->terrainHeight(3,terrainRotation, true);
        isForest=false;
        isGarden=false;
        m_volcanoEruption=true;
    }
    //checks if terrain 4 is facing upwards
    else if (t4_1.m_y > 0 && t4_2.m_y > 0)
    {
        //checks how the terrain is rotated and stores the angle
        if (t4_1.m_x > 0 && t4_1.m_z > 0) {terrainRotation=0;}
        else if (t4_1.m_x > 0 && t4_1.m_z < 0) {terrainRotation=90;}
        else if (t4_1.m_x < 0 && t4_1.m_z < 0) {terrainRotation=180;}
        else if (t4_1.m_x < 0 && t4_1.m_z > 0) {terrainRotation=270;}
        // passing the rotation angle to the terrainHeight function to recalculate the heights depending on the angle
        m_terrainHeights = m_terrain->terrainHeight(4,terrainRotation, false);
        isForest=false;
        isGarden=false;
        m_volcanoEruption=false;
    }
    //checks if terrain 5 is facing upwards
    else if (t5_1.m_y > 0 && t5_2.m_y > 0)
    {
        //checks how the terrain is rotated and stores the angle
        if (t5_1.m_x > 0 && t5_1.m_z > 0) {terrainRotation=270;}
        else if (t5_1.m_x > 0 && t5_1.m_z < 0) {terrainRotation=0;}
        else if (t5_1.m_x < 0 && t5_1.m_z < 0) {terrainRotation=90;}
        else if (t5_1.m_x < 0 && t5_1.m_z > 0) {terrainRotation=180;}
        // passing the rotation angle to the terrainHeight function to recalculate the heights depending on the angle
        m_terrainHeights = m_terrain->terrainHeight(5,terrainRotation, true);
        isForest=false;
        isGarden=false;
        m_volcanoEruption=false;
    }
    //checks if terrain 6 is facing upwards
    else if (t6_1.m_y > 0 && t6_2.m_y > 0)
    {
        //checks how the terrain is rotated and stores the angle
        if (t6_1.m_x > 0 && t6_1.m_z > 0){terrainRotation=0;}
        else if (t6_1.m_x > 0 && t6_1.m_z < 0) {terrainRotation=90;}
        else if (t6_1.m_x < 0 && t6_1.m_z < 0) {terrainRotation=180;}
        else if (t6_1.m_x < 0 && t6_1.m_z > 0) {terrainRotation=270;}
        // passing the rotation angle to the terrainHeight function to recalculate the heights depending on the angle
        m_terrainHeights = m_terrain->terrainHeight(6,terrainRotation, false);
        isForest=false;
        isGarden=false;
        m_volcanoEruption=false;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/// when the game resets, the player is going to be placed on terrain 1
/// the array has to be changed back to store the heights of terrain 1
/// Function written by Arnold Sule
//----------------------------------------------------------------------------------------------------------------------
void Sphere::resetPlayerOnTerrain()
{
    m_terrainHeights = m_terrain->terrainHeight(1,0,false);
}



