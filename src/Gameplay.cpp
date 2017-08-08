#include "Gameplay.h"

gamePlay::gamePlay()
{
    //set variables ready for game
    m_heartcount = 5;
    m_cureStage = 0;
    m_ingredients = 0;
    m_gameWon = false;
    m_gameErupted = false;
    m_gameLost = false;
    m_gameDisease = false;
    m_lavenderFlag = false;
    m_mercuryFlag = false;
    m_clovesFlag = false;
    m_rosesFlag = false;
}

//--------------------------------------------------------------------------------------------------------------------------

bool gamePlay::gameLost() const
{
    return m_gameLost;
}

//--------------------------------------------------------------------------------------------------------------------------

void gamePlay::setGameLost(bool _gameLost)
{
    m_gameLost = _gameLost;
}

//--------------------------------------------------------------------------------------------------------------------------

bool gamePlay::gameDisease() const
{
    return m_gameDisease;
}

//--------------------------------------------------------------------------------------------------------------------------

void gamePlay::setGameDisease(bool _gameDisease)
{
    m_gameDisease = _gameDisease;
}

//--------------------------------------------------------------------------------------------------------------------------

bool gamePlay::gameErupted() const
{
    return m_gameErupted;
}

//--------------------------------------------------------------------------------------------------------------------------

void gamePlay::setGameErupted(bool _gameErupted)
{
    m_gameErupted = _gameErupted;
}

//--------------------------------------------------------------------------------------------------------------------------

bool gamePlay::gameWon() const
{
    return m_gameWon;
}

//--------------------------------------------------------------------------------------------------------------------------

void gamePlay::setGameWon(bool _gameWon)
{
    m_gameWon = _gameWon;
}

//--------------------------------------------------------------------------------------------------------------------------

bool gamePlay::mercuryFlag() const
{
    return m_mercuryFlag;
}

//--------------------------------------------------------------------------------------------------------------------------

void gamePlay::setMercuryFlag(bool _mercuryFlag)
{
    m_mercuryFlag = _mercuryFlag;
}

//--------------------------------------------------------------------------------------------------------------------------

bool gamePlay::clovesFlag() const
{
    return m_clovesFlag;
}

//--------------------------------------------------------------------------------------------------------------------------

void gamePlay::setClovesFlag(bool _clovesFlag)
{
    m_clovesFlag = _clovesFlag;
}

//--------------------------------------------------------------------------------------------------------------------------

bool gamePlay::rosesFlag() const
{
    return m_rosesFlag;
}

//--------------------------------------------------------------------------------------------------------------------------

void gamePlay::setRosesFlag(bool _rosesFlag)
{
    m_rosesFlag = _rosesFlag;
}

//--------------------------------------------------------------------------------------------------------------------------

bool gamePlay::lavenderFlag() const
{
    return m_lavenderFlag;
}

//--------------------------------------------------------------------------------------------------------------------------

void gamePlay::setLavenderFlag(bool _lavenderFlag)
{
    m_lavenderFlag = _lavenderFlag;
}

//--------------------------------------------------------------------------------------------------------------------------

int gamePlay::heartcount() const
{
    return m_heartcount;
}

//--------------------------------------------------------------------------------------------------------------------------


void gamePlay::setHeartcount(int _heartcount)
{
    m_heartcount = _heartcount;
}

//--------------------------------------------------------------------------------------------------------------------------

int gamePlay::ingredients() const
{
    return m_ingredients;
}

//--------------------------------------------------------------------------------------------------------------------------

void gamePlay::setIngredients(int _ingredients)
{
    m_ingredients = _ingredients;
}

//--------------------------------------------------------------------------------------------------------------------------

int gamePlay::cureStage() const
{
    return m_cureStage;
}

//--------------------------------------------------------------------------------------------------------------------------

void gamePlay::setCureStage(int cureStage)
{
    m_cureStage = cureStage;
}

//--------------------------------------------------------------------------------------------------------------------------
