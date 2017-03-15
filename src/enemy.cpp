#include "enemy.h"
#include <vector>
#include <cmath>
#include <numeric>
#include <cstdlib>

Enemy::Enemy(ngl::Vec3 _pos)
{
m_pos=_pos;
std::cout<< m_pos.m_x<<"\n";
std::cout<< m_pos.m_y<<"\n";
std::cout<< m_pos.m_z<<"\n";
prey=false;
m_speed=0.4;
m_maxAccelerate=0.02;
m_velocity.set(1,1.3,1.2);
m_maxSpeed=0.6;
m_minSpeed=0.2;
m_avoidWeight=0.2;
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
    float predict = distanceCalc(m_prey, m_pos)/10;
  //  m_goal=(_pos+_targetSpeed*predict)-m_pos; //MVC  compiler
    //for each x/y/z component
    //gcc compiler
    m_goal.m_x=(_pos.m_x+_targetSpeed*predict)-m_pos.m_x;
    m_goal.m_y=(_pos.m_y+_targetSpeed*predict)-m_pos.m_y;
    m_goal.m_z=  (_pos.m_z+_targetSpeed*predict)-m_pos.m_z;

  if(m_goal.length()!=0)
  {
    m_goal.normalize();
  }
}

void Enemy::move(const ngl::Vec3 &m_spherePos)
{
/*  //get random num ||
float min = -2.0, max = 50.5;
int r;
float fraction;
r = rand();
fraction = ((float) r / RAND_MAX) * (max - min);
*/
//set prey to be player
m_prey= m_spherePos; //player
//calc distance between player and enemy
std::cout<<"enemy pos is"<<m_pos.m_x<<"\n";
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
setRotation();
kill();
//if far away from player,avoid neighbours, else don't
}

void Enemy::draw(float i)
{

    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    if(m_hit)
    shader->setUniform("Colour",1.0f*i,0.0f*i,0.0f);
    else
    shader->setUniform("Colour",0.0f*i,0.0f,1.0f);
    ngl::VAOPrimitives::instance()->draw("Enemy");
}


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
  m_avoid*=m_avoidWeight;
  m_target=m_goal+m_flee+m_avoid;
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
  m_velocity*=m_speed*0.3;
  m_pos =( m_pos+m_velocity);
}
void Enemy::setRotation()
{
  m_yaw = atan2(m_velocity.m_x,m_velocity.m_z)*180/M_PI+180;
  m_pitch = atan2(m_velocity.m_y,sqrt(m_velocity.m_x*m_velocity.m_x+m_velocity.m_z*m_velocity.m_z))*180/M_PI;
}

void Enemy::kill()
{
    if(m_pos==m_prey)
    {
        std::cout<<"DEAD "<<d<<"\n";
        d++;
    }
}

void Enemy::drawInit(ngl::Vec3 _offset)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    shader->setUniform("Colour", 1.0f,1.0f, 1.0f);//Set colour for flat diffuse colour shader
      shader->setUniform("myflag",1);//Flag enables the flat diffuse colour shader
    //set pos to draw only once in beginning
    m_pos=_offset;

    if(m_hit)
    shader->setUniform("Colour",1.0f,0.0f,0.0f);
    else
    shader->setUniform("Colour",0.0f,0.0f,1.0f);
    ngl::VAOPrimitives::instance()->draw("Enemy");
    std::cout<<"ENEMY DRAWN \n";


}
/*
void Enemy::Avoid()
{
  // first check neighbours for collisions, obstacle collisions are
  // checked in World.cpp
  for(int i=0; i<m_neighbours.size();++i)
  {
    findObstacle(m_neighbours[i]->getPosition(), m_neighbours[i]->getRadius());
  }
  if(m_velocity.length()!=0)
  {
    m_velocity.normalize();
  }
  // if there is a collision, calculate the avoidance force as a vector
  // from the collision position to the ahead vector
  m_avoid=(0,0,0);
  ngl::Vec3 ahead(m_position + m_velocity * m_seeAhead * m_speed);
  if(m_collisionPos != 0)
  {
    m_avoid=(ahead-m_collisionPos);
    if(m_avoid.length()!=0)
    {
      m_avoid.normalize();
    }
  }
  else
  {
    m_avoid*=0;
  }
  m_collisionPos = 0;
}*/
