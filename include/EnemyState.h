#ifndef ENEMYSTATE_H
#define ENEMYSTATE_H

#include "Sphere.h"
#include "Enemy.h"
#include <iostream>
#include <vector>
#include <memory>
#include <QGuiApplication>
#include <QApplication>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/NGLStream.h>
#include <ngl/VertexArrayObject.h>
///--------------------------------------------------------------------
/// @class EnemyState
/// @brief This class is in charge of the state of the enemies before movement calculations are made in the Enemy class
/// @author Stiliana Gospodinova
/// @version 1.0
/// @date 09/10/17
///--------------------------------------------------------------------
class EnemyState
{
public:
///--------------------------------------------------------------------
///@brief default ctr
///--------------------------------------------------------------------
    EnemyState();
///--------------------------------------------------------------------
///@brief populates a vector with Enemy objects and sets their
/// start positions
///--------------------------------------------------------------------
    void createEnemy();
///--------------------------------------------------------------------
///@brief clears enemy vector
///--------------------------------------------------------------------
    void deleteEnemy();
///--------------------------------------------------------------------
///@brief resets all enemies to their start positions(no rotations made)
///--------------------------------------------------------------------
    void reset();
///--------------------------------------------------------------------
///@brief resets enemies in Garden to their start positions
///--------------------------------------------------------------------
    void resetGarden();
///--------------------------------------------------------------------
///@brief vector containing enemy group on Forest plane
///--------------------------------------------------------------------
    std::vector <Enemy> enemyForest;
///--------------------------------------------------------------------
///@brief vector containing enemy group on Garden plane
///--------------------------------------------------------------------
    std::vector <Enemy> enemyGarden;
///--------------------------------------------------------------------
///@brief vector containing starting positions for enemies in Forest
///--------------------------------------------------------------------
    ngl::Vec3 enemyStartPos[5];
///--------------------------------------------------------------------
///@brief vector containing rotated starting positions for enemies
/// in Garden
///--------------------------------------------------------------------
    ngl::Vec3 enemyStartPos2[5];
///--------------------------------------------------------------------
///@brief bool to hold reset state of enemies
///--------------------------------------------------------------------
    bool isReset;
};

#endif // ENEMYSTATE_H
