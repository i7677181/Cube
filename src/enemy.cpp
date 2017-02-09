#include "enemy.h"
#include <vector>
#include <cmath>
#include <numeric>
#include <cstdlib>
#include <OpenSteer/SteerLibrary.h>

/* collision bump is not set to current enemy location but at origin of enemy sphere creation
    add obstacle avoidance
    collision check for enemy must be made in NGLScene
*/
Enemy::Enemy(ngl::Vec3 _pos)
{
    m_pos = _pos;
    m_pos=(5,5,5);
    prey=false;
    m_speed=0.3;
    m_maxAccelerate=0.01;

    //get random num ||
     float min = -1.0, max = 1.0;
     int r;
     float fraction;
     r = rand();
     fraction = ((float) r / RAND_MAX) * (max - min);
     m_randAccel=fraction;

    m_velocity.set(0.8,0.3,0.2);
    m_maxSpeed=0.5;
    m_minSpeed=0.1;


}
//call separetly from move
void Enemy::setPrey(const ngl::Vec3 &m_spherePos)//@param player pos
{
    m_prey=m_spherePos;
    m_prevDistance=distanceCalc(m_prey, m_pos);
}
//fix
void Enemy::setChase(ngl::Vec3 _pos, float _targetSpeed)
{
  float predict = distanceCalc(m_prey, m_pos)/20;
  m_goal=(_pos+_targetSpeed*predict)-m_pos;
  if(m_goal.length()!=0)
  {
    m_goal.normalize();
  }
}

void Enemy::move(const ngl::Vec3 &m_spherePos)
{
    //set player and enemy positions
    m_prey= m_spherePos; //player
    //calc distance between player and enemy
 float distance =   distanceCalc(m_prey,m_pos);
 //   std::cout<<"distance of enemy from player is:"<<d<<"\n";
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
setRotation();
}

void Enemy::draw()
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
       if(m_hit)
           shader->setUniform("Colour",1.0f,0.0f,0.0f);

       else
           shader->setUniform("Colour",0.0f,0.0f,1.0f);
           ngl::VAOPrimitives::instance()->draw("Enemy");



//ngl::VAOPrimitives::instance()->draw("Enemy");
}

/*
ngl::Vec3 Enemy::steerForSeek()
{
 // velocity=m_prey->getVelocity(); //crashes
    {
        desiredVelocity = target - m_pos;
        return  desiredVelocity - velocity;
        //return desiredVelocity;

    }

}
*/
float Enemy::distanceCalc(ngl::Vec3 a, ngl::Vec3 b)
{
    return sqrt(((a.m_x - b.m_x)*(a.m_x - b.m_x) + (a.m_z - b.m_z)*(a.m_z - b.m_z)));
}

void Enemy::fleeWalls()
{
    if(m_pos.m_x<=-100 || m_pos.m_x>=100)
    {
      m_flee.m_x-=(m_pos.m_x/2);
    }
    if(m_pos.m_z<=-100 || m_pos.m_z>=100)
    {
      m_flee.m_z-=(m_pos.m_z/2);
    }
}

void Enemy::setTarget()
{
  m_goal*=30;
  //m_avoid*=m_avoidWeight;
  m_target=m_goal+m_flee;//+m_avoid;
  if(m_target.length()!=0)
  {
    m_target.normalize();
  }
}

void Enemy::setSteer()
{
    {
      m_steering = m_target-m_velocity;
      if(m_steering.length()!=0)
      {
        m_steering.normalize();
      }
    }

}
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
  m_velocity*=m_speed*0.3;//m_randAccel/5;
  m_pos =( m_pos+m_velocity);
 // kill();
}
void Enemy::setRotation()
{
  m_yaw = atan2(m_velocity.m_x,m_velocity.m_z)*180/M_PI+180;
  m_pitch = atan2(m_velocity.m_y,sqrt(m_velocity.m_x*m_velocity.m_x+m_velocity.m_z*m_velocity.m_z))*180/M_PI;
}

void Enemy::kill()
{

    std::cout<<"enemy is at "<<m_pos.m_x<<"\n"<<"player is at"<<m_prey.m_x<<"\n";
    if(m_pos==m_prey)
    {
        std::cout<<"DEAD\n";
    }
}
