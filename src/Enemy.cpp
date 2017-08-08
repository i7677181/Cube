#include "Enemy.h"
#include <vector>
#include <cmath>
#include <numeric>
#include <cstdlib>
///---------------------------------------------------------------------------------------------------------------------
/// @brief Enemy class handling calculations for enemy behaviour forces
///---------------------------------------------------------------------------------------------------------------------
Enemy::Enemy(ngl::Vec3 _pos)
{
    m_pos=_pos;
    preyIsActive=false;
    m_speed=.5;
    m_maxAccelerate=0.03;
    m_velocity.set(1,0.3,1.2);
    m_maxSpeed=1.0;
    m_minSpeed=0.2;
    m_avoidWeight=0.2;
}
//----------------------------------------------------------------------------------------------------------------------
void Enemy::setPrey(const ngl::Vec3 &_playerPos)
{
    m_prey=_playerPos;
    m_prevDistance=distanceCalc(m_prey, m_pos);
}
//----------------------------------------------------------------------------------------------------------------------
void Enemy::setChase(ngl::Vec3 _pos, float _targetSpeed)
{
    float predict = distanceCalc(m_prey, m_pos)/10;
    //  m_goal=(_pos+_targetSpeed*predict)-m_pos; //MVC  compiler
    //for each x/y/z component
    //gcc compiler
    m_goal.m_x=(_pos.m_x+_targetSpeed*predict)-m_pos.m_x;
    m_goal.m_y=(_pos.m_y+_targetSpeed*predict)-m_pos.m_y;
    m_goal.m_z=(_pos.m_z+_targetSpeed*predict)-m_pos.m_z;


  if(m_goal.length()!=0)
  {
    m_goal.normalize();
  }
}
//----------------------------------------------------------------------------------------------------------------------
void Enemy::move(const ngl::Vec3 &_playerPos)
{
    if(m_behaviour==true){
         m_isFrozen=false;

        //set prey to be player
        m_prey= _playerPos; //player

        //calc distance between player and enemy
        float distance =  distanceCalc(m_prey,m_pos);

        if(distance<m_prevDistance)
        {
            m_speed+=m_maxAccelerate;
        }
        else
        {
            m_speed-=m_maxAccelerate;
        }

        m_prevDistance = distance;
        setChase(m_prey, 0.2);
        fleeWalls();
        setTarget();
        setSteer();
        updatePosition();
    }

}
//----------------------------------------------------------------------------------------------------------------------
void Enemy::draw()
{

    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    if(m_hit)
    shader->setUniform("Colour",1.0f,0.0f,0.0f);
    else
    shader->setUniform("Colour",0.0f,0.0f,1.0f);
    ngl::VAOPrimitives::instance()->draw("Enemy");

}
//----------------------------------------------------------------------------------------------------------------------
float Enemy::distanceCalc(ngl::Vec3 a, ngl::Vec3 b)
{
    return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x) + (a.m_z - b.m_z)*(a.m_z - b.m_z)));
}
//----------------------------------------------------------------------------------------------------------------------
void Enemy::fleeWalls()
{
    m_behaviour=true;
    //edit
    if(m_pos.m_x<=-4.8 || m_pos.m_x>=4.8)
    {
        std::cout<<"OOBx";
        m_isFrozen=true;
        m_behaviour=false;
        freezeEnemy();
        printPos();
    }
    if(m_pos.m_z<=-4.8 || m_pos.m_z>=4.8)
    {
        std::cout<<"OOBy";
        m_isFrozen=true;
        m_behaviour=false;
        freezeEnemy();
        printPos();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Enemy::setTarget()
{
  m_goal*=30;
  m_avoid*=m_avoidWeight;
  m_target=m_goal+m_flee+m_avoid;
  if(m_target.length()!=0)
  {
    m_target.normalize();
  }
}
//----------------------------------------------------------------------------------------------------------------------
void Enemy::setSteer()
{
      m_steering = m_target-m_velocity;
      if(m_steering.length()!=0)
      {
        m_steering.normalize();
      }
}
//----------------------------------------------------------------------------------------------------------------------
void Enemy::updatePosition()
{
  m_velocity +=(m_steering/0.5);
  if(m_speed>m_maxSpeed)
  {
    m_speed=m_maxSpeed;
  }
  else if(m_speed<m_minSpeed)
  {
    m_speed=m_minSpeed;
  }
  if(m_velocity.length()!=0)
  {
    m_velocity.normalize();
  }
  m_velocity*=m_speed*0.3;
  m_pos =( m_pos+m_velocity);

}
//----------------------------------------------------------------------------------------------------------------------
void Enemy::setAvoid(ngl::Vec3 _dir)
{
    m_dir=_dir;
    if(isAvoiding==true)
    {
        if(m_velocity.length()!=0)
        {
          m_velocity.normalize();
        }
        ngl::Vec3 ahead=m_pos+m_velocity*90; //avoid weight 999999

        if(m_separate.length()!=0)
        {
          m_separate.normalize();
        }
    m_avoid-=(m_pos+m_velocity*m_speed*20)-ahead;
    m_steering=m_dir-m_velocity;
    m_velocity=m_avoid+m_steering+m_separate;//m_separate;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Enemy::freezeEnemy()
{
   // m_pos=m_pos;
    std::cout<<"frozen\n";
    m_isFrozen=true;
}
//----------------------------------------------------------------------------------------------------------------------
void Enemy::printPos()
{
    std::cout<< m_pos.m_x<<"\n";
    std::cout<< m_pos.m_z<<"\n";
    //std::cout<< m_pos.m_z<<"\n";
}
//----------------------------------------------------------------------------------------------------------------------
