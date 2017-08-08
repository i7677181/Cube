#include "Timer.h"
#include <iostream>
//----------------------------------------------------------------------------------------------------------------------
Timer::Timer()
{
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()), this ,SLOT(Tick()));
    m_hasErrupted = false;
    counterV=0;
    counterD=0;
    Dseconds=60;
    Vseconds=20;
    m_isActive=false;

}
//----------------------------------------------------------------------------------------------------------------------
void Timer::Tick()
{
    if (m_isActive==true)
    {
        counterV++;
        counterD++;
      //  std::cout<<"TIMER = "<<counterV<<std::endl;
        getTime();
        if (Dseconds>0)
        {
            Dseconds--;
        }
        else
        {
            Dseconds=60;
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Timer::getTime()
{
    if(counterV<20)
    {
        Vseconds--;
        m_hasErrupted=false;
    }
    if(counterV==20)
     {
        std::cout<<"Volcano Errupting!\n";
        m_hasErrupted=true;
        timer->start(1000);
        Vseconds=0;
    }
    if(counterV==23)
    {
        counterV=0;
        Vseconds=20;
    }


    if(counterD>=120) //if above time limit
    {
        m_endTimer=false;
        std::cout<<"Time for collecting plague cure OVER, removing enemies\n";
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Timer::timerStart()
{
    m_isActive=true;
    timer->start(1000);
}
//----------------------------------------------------------------------------------------------------------------------
void Timer::timerPause()
{
    m_isActive=false;
}
//----------------------------------------------------------------------------------------------------------------------

int Timer::minutesCountdown()
{
    if (counterD==0)
    {
        return 2;
    }
    else if (counterD>0 && counterD<=60)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Timer::secondsCountdown()
{
    if (counterD==0 || counterD>120)
    {
        return 0;
    }
    else
    {
        return Dseconds;
    }
}
