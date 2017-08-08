#ifndef ENEMY_H
#define ENEMY_H

#include <ngl/Camera.h>
#include <ngl/Vec3.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include "Sphere.h"
///--------------------------------------------------------------------
/// @class Enemy
/// @brief This class is in charge of calculating the enemy movement using predetor behaviour models similar to Craig Reynold's'Boids' algorithms
/// @author Stiliana Gospodinova
/// @version 1.0
/// @date 09/10/17
///--------------------------------------------------------------------
class Enemy
{
public:
///--------------------------------------------------------------------
///@brief ctr
///@param [in] _pos initial enemy position passed in
///--------------------------------------------------------------------
    Enemy(ngl::Vec3 _pos);
///--------------------------------------------------------------------
///@brief methods to initially draw the enemy at creation time
///@param [in] _playerPos player position passed in
///--------------------------------------------------------------------
    void move(const ngl::Vec3 &_playerPos);
///--------------------------------------------------------------------
///@brief methods to draw the enemies
///--------------------------------------------------------------------
    void draw();
///--------------------------------------------------------------------
///@brief inline functions to get the position of enemy
/// @return returns enemy pos
///--------------------------------------------------------------------
    inline ngl::Vec3 getPosition() {return m_pos;}
///--------------------------------------------------------------------
///@brief inline functions to set the position of enemy
///@param [in] _pos enemy position to be set
///--------------------------------------------------------------------
    inline void setPosition(ngl::Vec3 _pos) {m_pos=_pos;}
///--------------------------------------------------------------------
///@brief method to calculate the distance between the prey and enemy
///@param [in] _playerPos player position reference passed in as prey
///--------------------------------------------------------------------
    void setPrey(const ngl::Vec3 &_playerPos);
///--------------------------------------------------------------------
///@brief methods to check if enemy has collided with player
///@param [in] _h if a collision's made
///@param [in] _hitCount amounts of hits
///--------------------------------------------------------------------
    void setHit(bool _h, int _hitCount){m_hit=_h,_hitCount=m_hitCount;}
///--------------------------------------------------------------------
///@brief method to set avoidance force whenever enemies are about to
/// overlap each other
///@param [in] _dir direction of player
///--------------------------------------------------------------------
    void setAvoid(ngl::Vec3 _dir);
///--------------------------------------------------------------------
///@brief forces enemy to stop chasing player whenever out of bounds
///--------------------------------------------------------------------
    void freezeEnemy();
///--------------------------------------------------------------------
///@brief checks if current enemy is inactive/frozen
/// @param [out] m_isFrozen returns state of enemy
///--------------------------------------------------------------------
    inline bool isFrozen(){return m_isFrozen;}
///--------------------------------------------------------------------
private:
///--------------------------------------------------------------------
///@brief flag hit
///--------------------------------------------------------------------
    bool m_hit=false;
    int m_hitCount;
///--------------------------------------------------------------------
///@brief flag if enemy is using avoidance force
///--------------------------------------------------------------------
    bool isAvoiding=true;
///--------------------------------------------------------------------
///@brief flag if enemy is using behaviour forces
///--------------------------------------------------------------------
    bool m_behaviour=true;
///--------------------------------------------------------------------
///@brief flag if enemy has gone over bounds and is not moving
///--------------------------------------------------------------------
    bool m_isFrozen;
///--------------------------------------------------------------------
///@brief flag check if prey is set
///--------------------------------------------------------------------
    bool preyIsActive;
///----------------------------------------------------------
/// @brief forces to calculate behaviors for enemies
/// ---------------------------------------------------------
    ngl::Vec3 m_dir;
    ngl::Vec3 m_prey;
    ngl::Vec3 m_flee;
    ngl::Vec3 m_target;
    ngl::Vec3 m_steering;
    ngl::Vec3 m_velocity;
    ngl::Vec3 desiredVelocity;
    ngl::Vec3 m_goal;
    ngl::Vec3 m_avoid;
    ngl::Vec3 m_separate;
///----------------------------------------------------------
/// @brief enemy position
/// ---------------------------------------------------------
    ngl::Vec3 m_pos;
///----------------------------------------------------------
///@brief relevant weights applied to forces
/// ---------------------------------------------------------
    float m_prevDistance;
    float m_speed;
    float m_maxAccelerate;
    float m_randAccel;
    float m_xmax;
    float m_xmin;
    float m_zmax;
    float m_zmin;
    float m_maxSpeed;
    float m_minSpeed;
    float m_avoidWeight;
///--------------------------------------------------------------------
/// @brief function to calculate distance between player and enemy
/// @param [in] _a position of player
/// @param [in] _b position of enemy
///--------------------------------------------------------------------
    float distanceCalc(ngl::Vec3 _a ,ngl::Vec3 _b);
///--------------------------------------------------------------------
/// @brief combines all the behaviours and updates the enemy positions
///--------------------------------------------------------------------
    void updatePosition();
///--------------------------------------------------------------------
/// @brief function to set the goal of the enemies
/// @param [in] _pos position of player
/// @param[in] _targetSpeed player speed
///--------------------------------------------------------------------
    void setChase(ngl::Vec3 _pos, float _targetSpeed);
///--------------------------------------------------------------------
/// @brief function to set the target of the enemies
///--------------------------------------------------------------------
    void setTarget();
///--------------------------------------------------------------------
/// @brief function to freeze the movement of the enemies
///--------------------------------------------------------------------
    void enemyFreeze();
///--------------------------------------------------------------------
/// @brief function to stop the enemies from going out of bounds
///--------------------------------------------------------------------
    void fleeWalls();
///--------------------------------------------------------------------
/// @brief function to set the steering behavour of the enemies
///--------------------------------------------------------------------
    void setSteer();
///--------------------------------------------------------------------
/// @brief function to check the current positions of the enemies
/// used for debugging
///--------------------------------------------------------------------
    void printPos();
};

#endif // ENEMY_H
