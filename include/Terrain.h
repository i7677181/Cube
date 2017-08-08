#ifndef TERRAIN_H
#define TERRAIN_H

/// @file Terrain.h
/// @brief a class for creating the terrain on each side of the cube
#include <qimage.h>
#include <ngl/Transformation.h>

//----------------------------------------------------------------------------------------------------------------------
///  @class Terrain  "include/Terrain.h"
///  @brief a class for creating the terrain on each side of the world
///  @author Arnold Sule
///  @version 1.0
///  @date Last Revision 27/05/17
//----------------------------------------------------------------------------------------------------------------------

class Terrain
{
public:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Default Terrain constructor
    //----------------------------------------------------------------------------------------------------------------------
    Terrain();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief terrainHeight method calculates and stores relative height information of terrain
    /// @param[in]  _side referes to one side of the central cube
    /// @param[in]  _rotation_angle the angle of rotation of the loaded height map images
    /// @param[in]  _mirrored whether or not the loaded height map images have to be mirrored
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<int> terrainHeight(int _side, int _rotation_angle, bool _mirrored);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief getCubePosScale1 method returns the position and the scale of the small cubes on terrain 1
    /// @param[out] m_CubePosScale1 position and the scale of the small cubes on terrain 1
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> const getCubePosScale1(){return m_CubePosScale1;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief getCubePosScale2 method returns the position and the scale of the small cubes on terrain 2
    /// @param[out] m_CubePosScale2 position and the scale of the small cubes on terrain 2
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> const getCubePosScale2(){return m_CubePosScale2;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief getCubePosScale3 method returns the position and the scale of the small cubes on terrain 3
    /// @param[out] m_CubePosScale2 position and the scale of the small cubes on terrain 3
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> const getCubePosScale3(){return m_CubePosScale3;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief getCubePosScale4 method returns the position and the scale of the small cubes on terrain 4
    /// @param[out] m_CubePosScale2 position and the scale of the small cubes on terrain 4
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> const getCubePosScale4(){return m_CubePosScale4;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief getCubePosScale5 method returns the position and the scale of the small cubes on terrain 5
    /// @param[out] m_CubePosScale2 position and the scale of the small cubes on terrain 5
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> const getCubePosScale5(){return m_CubePosScale5;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief getCubePosScale6 method returns the position and the scale of the small cubes on terrain 6
    /// @param[out] m_CubePosScale2 position and the scale of the small cubes on terrain 6
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> const getCubePosScale6(){return m_CubePosScale6;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief initializeTerrain method initializes terrain for each side
    //----------------------------------------------------------------------------------------------------------------------
    void initializeTerrain();

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the positions and the scales of the small cubes the terrain is made out of for the first side of the world
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> m_CubePosScale1;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the positions and the scales of the small cubes the terrain is made out of for the second side of the world
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> m_CubePosScale2;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the positions and the scales of the small cubes the terrain is made out of for the third side of the world
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> m_CubePosScale3;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the positions and the scales of the small cubes the terrain is made out of for the fourth side of the world
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> m_CubePosScale4;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the positions and the scales of the small cubes the terrain is made out of for the fifth side of the world
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> m_CubePosScale5;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the positions and the scales of the small cubes the terrain is made out of for the sixth side of the world
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Transformation> m_CubePosScale6;
};

#endif // TERRAIN_H
