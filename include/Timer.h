#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QDebug>
///--------------------------------------------------------------------
/// @class Timer
/// @brief this class is managing timers for the volcano and plague
/// @author Stiliana Gospodinova
/// @version 1.0
/// @date 09/10/17
///--------------------------------------------------------------------
class Timer:public QObject
{ Q_OBJECT
public:
///--------------------------------------------------------------------
///@brief default ctr
///--------------------------------------------------------------------
    Timer();
///--------------------------------------------------------------------
///@brief Qtimer object
///--------------------------------------------------------------------
    QTimer *timer;
///--------------------------------------------------------------------
///@brief inactive state of timer
///--------------------------------------------------------------------
    bool m_endTimer=true;
///--------------------------------------------------------------------
///@brief check if timer is active
///@return returns state of timer
///--------------------------------------------------------------------
    inline bool isActive()const{return m_isActive;}
///--------------------------------------------------------------------
///@brief sets timer state
///@param [in] _state state passed in
///@author Michael Woods
///--------------------------------------------------------------------
    inline void setActive(bool _state) {m_isActive = _state;}
///--------------------------------------------------------------------
///@brief checks state of volcano
///@return returns state of volcano
///--------------------------------------------------------------------
    const bool &isErrupting() const{return m_hasErrupted;}
///--------------------------------------------------------------------
///@brief current time
///--------------------------------------------------------------------
    void getTime();
///--------------------------------------------------------------------
///@brief checks Plague timer for enemies being active
///@return timer state
///--------------------------------------------------------------------
    inline bool getPlagueTimer(){return m_endTimer;}
///--------------------------------------------------------------------
///@brief reset counters
///@return reset counters
///@author Michael Woods
///--------------------------------------------------------------------
    inline void reset() {counterV = 0; counterD = 0; m_hasErrupted = false; Vseconds = 20, Dseconds = 60; m_endTimer = true;}
///--------------------------------------------------------------------
///@brief starts timer
///--------------------------------------------------------------------
    void timerStart();
///--------------------------------------------------------------------
///@brief pauses timer
///--------------------------------------------------------------------
    void timerPause();

    int minutesCountdown();
    int secondsCountdown();   
    bool m_isActive;
    bool m_hasErrupted;
    int64_t counterV;
    int64_t counterD;
    int Vseconds;
    int Dseconds;

public slots:
///--------------------------------------------------------------------
///@brief ticking function to update timer
///--------------------------------------------------------------------
    void Tick();    
};

#endif // TIMER_H
