#ifndef SPHERE_H__
#define SPHERE_H__
/// @file Sphere.h
/// @brief a class in which the player's position is updated, the world's rotation is set and the terrain collides with the player
#include "Terrain.h"
#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Mat3.h>
#include <ngl/NGLStream.h>

//----------------------------------------------------------------------------------------------------------------------
///  @class Sphere  "include/Sphere.h"
///  @brief This class updates the player's position, rotates the world and makes the player collide on top of terrain.
///  @author Arielle Martin
///  @version 1.0
///  @date Last Revision 27/05/17 \n
//----------------------------------------------------------------------------------------------------------------------

class Sphere
{
public:

    //---------------------------------------------------------------------------------
    /// @brief class ctor initialises the player's position and the terrain
    /// @param[in] _pos the position of the player
    //---------------------------------------------------------------------------------
    Sphere(ngl::Vec3 _pos);
    //---------------------------------------------------------------------------------
    /// @brief moves the player and applies a rotation baed on the position
    /// @param[in] _x the distance the player moves in the x direction
    /// @param[in] _z the distance the player moves in the z direction
    //---------------------------------------------------------------------------------
    void move(float _x, float _z);
    //---------------------------------------------------------------------------------
    /// @brief function that draws the character
    //---------------------------------------------------------------------------------
    void draw();
    //---------------------------------------------------------------------------------
    /// @brief function that sets the dimensions of the edge of the cube's face
    /// @param[in] _width is the dimension in the x direction
    /// @param[in] _height is the dimension in the z direction
    //---------------------------------------------------------------------------------
    void planeEdge(float _width, float _height);
    //---------------------------------------------------------------------------------
    /// @brief sets whether a collision with an object has been made
    //---------------------------------------------------------------------------------
    inline void setHit(){m_hit=true;}
    //---------------------------------------------------------------------------------
    /// @brief sets whether a collision with an object has been made
    //---------------------------------------------------------------------------------
    inline void setNotHit(){m_hit=false;}
    //---------------------------------------------------------------------------------
    /// @brief returns whether a collision with an object has been made
    /// @param[out] m_hit will be true is a collision with an object has been made
    //---------------------------------------------------------------------------------
    inline bool isHit()const {return m_hit;}
    //---------------------------------------------------------------------------------
    /// @brief returns whether the volcano is erupting or not
    /// @param[out] m_volcanoEruption identifies whether the volcano is erupting or not
    //---------------------------------------------------------------------------------
    bool getVolcanoStatus(){return m_volcanoEruption;}
    //---------------------------------------------------------------------------------
    /// @brief sets the player position from within NGLScene
    /// @param[in] _pos is the new position of the player
    //---------------------------------------------------------------------------------
    void setPosition(ngl::Vec3 _pos){m_pos=_pos;}
    //---------------------------------------------------------------------------------
    /// @brief gets the position of the player
    /// @param[out] m_pos returns the current position of the player
    //---------------------------------------------------------------------------------
    const ngl::Vec3 &getPosition() const { return m_pos;}
    //---------------------------------------------------------------------------------
    /// @brief gets the rotation of the world
    /// @param[out] m_rotation is the returned rotation
    //---------------------------------------------------------------------------------
    const ngl::Mat4 &getRotation() const { return m_rotation;}
    //---------------------------------------------------------------------------------
    /// @brief function that sets the player's current position to its last
    /// @author Ruth Hutton
    //---------------------------------------------------------------------------------
    void sendBack();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief tracks which terrain the player is on and rotates the terrain information when entering on different sides
    //----------------------------------------------------------------------------------------------------------------------
    void trackTerrain();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief resets the terrain information to terrain1
    //----------------------------------------------------------------------------------------------------------------------
    void resetPlayerOnTerrain();
    //----------------------------------------------------------------------------------------------------------------------
    /// sets the position of the first and the last cube on one side (opposite corners)
    /// @param[in] m_cubes is the position of the first and the last small cube on one terrain (opposite corners)
    //----------------------------------------------------------------------------------------------------------------------
    void setFirstAndLastCubePositions(ngl::Vec3 m_cubes[7][2]);

    bool isForest=true;
    bool isGarden=false;

    //---------------------------------------------------------------------------------
    /// @brief the value from which the world should rotate
    //---------------------------------------------------------------------------------
    ngl::Mat4 m_rotation;
    //---------------------------------------------------------------------------------
    /// @brief boolean which identifies whether a rotation is occurring or not
    //---------------------------------------------------------------------------------
    bool rotateWorld=false;
    //---------------------------------------------------------------------------------
    /// @brief the position of the player
    //---------------------------------------------------------------------------------
    ngl::Vec3 m_pos;

private:

    //---------------------------------------------------------------------------------
    /// @brief value at which the position should move
    //---------------------------------------------------------------------------------
    float x=0;
    //---------------------------------------------------------------------------------
    /// @brief the max x value of the cube's edge
    //---------------------------------------------------------------------------------
    float m_xmax;
    //---------------------------------------------------------------------------------
    /// @brief the min x value of the cube's edge
    //---------------------------------------------------------------------------------
    float m_xmin;
    //---------------------------------------------------------------------------------
    /// @brief the max z value of the cube's edge
    //---------------------------------------------------------------------------------
    float m_zmax;
    //---------------------------------------------------------------------------------
    /// @brief the min z value of the cube's edge
    //---------------------------------------------------------------------------------
    float m_zmin;
    //---------------------------------------------------------------------------------
    /// @brief stores the last position of the player
    //---------------------------------------------------------------------------------
    ngl::Vec3 m_lastPos;
    //---------------------------------------------------------------------------------
    /// @brief is true if the player is colliding with an object
    //---------------------------------------------------------------------------------
    bool m_hit;
    //---------------------------------------------------------------------------------
    /// @brief is true if the volcano is erupting
    //---------------------------------------------------------------------------------
    bool m_volcanoEruption=false;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the position of the first and the last small cube on one terrain (opposite corners)
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_cubes[7][2];
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief height information of the terrain that the player moves on
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<int> m_terrainHeights;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief pointer to the terrain class
    //----------------------------------------------------------------------------------------------------------------------
    std::unique_ptr<Terrain> m_terrain;
};

#endif
