
#include "EnemyState.h"
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
//----------------------------------------------------------------------------------------------------------------------
EnemyState::EnemyState()
{
isReset=false;
}
//----------------------------------------------------------------------------------------------------------------------
void EnemyState::createEnemy()
{
    ngl::VAOPrimitives *enemy_prim=ngl::VAOPrimitives::instance();
    enemy_prim->createSphere("Enemy",0.2,50);

    //set random start positions for enemies
    enemyStartPos[0]=ngl::Vec3(-1.6,6.8, 0.59); ///x,y,z y=height
    enemyStartPos[1]=ngl::Vec3(2.3, 6.8, 0.60);
    enemyStartPos[2]=ngl::Vec3(-3.9, 6.8, 2.4);
    enemyStartPos[3]=ngl::Vec3(-2.9, 6.8, -1.4);
    enemyStartPos[4]=ngl::Vec3(1.4, 6.8, 1.8);
    //-------------------------------------enemyGarden rotated start pos
    enemyStartPos2[0]=ngl::Vec3(-1.6, 0.59, 6.8); //z,x,y=height
    enemyStartPos2[1]=ngl::Vec3(2.3, 0.60, 6.8);
    enemyStartPos2[2]=ngl::Vec3(-3.9, 2.4, 6.8);
    enemyStartPos2[3]=ngl::Vec3(-2.9,-1.4, 6.8);
    enemyStartPos2[4]=ngl::Vec3(1.4, 1.8, 6.8);

    //fill enemy vector with 5 enemy objects
    for (int i=0;i<5;i++){
    enemyForest.push_back(Enemy(enemyStartPos[i]));
    enemyGarden.push_back(Enemy(enemyStartPos2[i]));

    enemyForest[i].setPosition(enemyStartPos[i]);
    enemyGarden[i].setPosition(enemyStartPos2[i]);

    enemyForest[i].draw();
    enemyGarden[i].draw();

    //std::cout << "Enemy obj array size: " << enemyForest.size() << "\n";

    }

}
//----------------------------------------------------------------------------------------------------------------------
void EnemyState::deleteEnemy()
{
    enemyForest.clear();
    enemyGarden.clear();
}
//----------------------------------------------------------------------------------------------------------------------
void EnemyState::resetGarden()
{   isReset=true;
   for(unsigned int i=0;i<enemyGarden.size();i++)
    {
        enemyGarden[i].setPosition(enemyStartPos[i]); //reset to be on player plane
    }

}
//----------------------------------------------------------------------------------------------------------------------
void EnemyState::reset()
{
    for(unsigned int i=0;i<5;i++){
        deleteEnemy();
        createEnemy();
        enemyForest[i].setPosition(enemyStartPos[i]);
        enemyGarden[i].setPosition(enemyStartPos2[i]);
    }
}
//----------------------------------------------------------------------------------------------------------------------
