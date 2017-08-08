#include "UI.h"
#include <vector>
#include <ngl/Texture.h>
#include <ngl/ShaderLib.h>

//----------------------------------------------------------------------------------------------------------------------

UI::UI()
{
    //initialising variables
    m_triggerTimer = false;
    m_uiTimer = 80;
    exitQuestionFlag = false;
    m_exitFlag = false;
    m_restartFlag = false;
    m_exitState = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void UI::initUI()
{
    //creating a vector of strings to store IDs of textures
    std::vector<std::string> m_UiTextures;
    m_UiTextures.push_back("textures/menu_UI_Textures/doctor_render.tif");
    m_UiTextures.push_back("textures/menu_UI_Textures/queststarted.png");
    m_UiTextures.push_back("textures/menu_UI_Textures/gettingingredients.png");
    m_UiTextures.push_back("textures/menu_UI_Textures/finished.png");;
    m_UiTextures.push_back("textures/menu_UI_Textures/livestest_1.png");
    m_UiTextures.push_back("textures/menu_UI_Textures/livestest_2.png");
    m_UiTextures.push_back("textures/menu_UI_Textures/livestest_3.png");
    m_UiTextures.push_back("textures/menu_UI_Textures/livestest_4.png");
    m_UiTextures.push_back("textures/menu_UI_Textures/livestest_5.png");
    m_UiTextures.push_back("textures/menu_UI_Textures/pouch.tif");
    m_UiTextures.push_back("textures/menu_UI_Textures/yes.tif");
    m_UiTextures.push_back("textures/menu_UI_Textures/No.tif");
    m_UiTextures.push_back("textures/menu_UI_Textures/deathbyenemy.tif");
    m_UiTextures.push_back("textures/menu_UI_Textures/deathbyvolcano.tif");
    m_UiTextures.push_back("textures/menu_UI_Textures/Won.png");
    m_UiTextures.push_back("textures/menu_UI_Textures/Volcano_Timer.png");
    m_UiTextures.push_back("textures/menu_UI_Textures/death_timer.png");
    m_UiTextures.push_back("textures/menu_UI_Textures/DeathByPlague.png");
    m_UiTextures.push_back("textures/menu_UI_Textures/safeFromEruption.tif");

    //storing the IDs into vector
    for (unsigned int i = 0; i < m_UiTextures.size(); i++)
    {
        ngl::Texture m_UiTex(m_UiTextures[i]);
        //ID for texture
        GLint UitexID = m_UiTex.setTextureGL();
        m_menuTextureId.push_back(UitexID);
    }

    //creating the quad for the UI using a vertex array object
    m_UI.reset(ngl::VertexArrayObject::createVOA(GL_TRIANGLES));
    m_UI->bind();

    //shared vertices
    const static GLubyte indices[]=  {

                                         0, 1, 3,   //first triangle
                                         1, 2, 3   //second triangle
                                      };
    //Vertices of the quad
    GLfloat vertices[] = {
                              0.5f,  0.5f, 0.0f,
                              0.5f, -0.5f, 0.0f,
                              -0.5f, -0.5f, 0.0f,
                              -0.5f,  0.5f, 0.0f,
                          };

    //Uvs of the quad
     GLfloat uv[]={
                       1.0f, 1.0f,
                       1.0f, 0.0f,
                       0.0f, 0.0f,
                       0.0f, 1.0f,

                   };

    // in this case we are going to set our data as the vertices above
    m_UI->setIndexedData(12*sizeof(GLfloat),vertices[0],sizeof(indices),&indices[0],GL_UNSIGNED_BYTE,GL_STATIC_DRAW);
    // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
    m_UI->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_UI->setIndexedData(8*sizeof(GLfloat),uv[0],sizeof(indices),&indices[0],GL_UNSIGNED_BYTE,GL_STATIC_DRAW);
    // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
    m_UI->setVertexAttributePointer(1,2,GL_FLOAT,0,0); //bind verts to shader
    m_UI->setNumIndices(sizeof(indices));

    // now unbind
    m_UI->unbind();


}

//----------------------------------------------------------------------------------------------------------------------


void UI::drawQuad(int _UItexID){
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    glEnable (GL_BLEND);
    glEnable(GL_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    (*shader)["UI"]->use();
    glBindTexture(GL_TEXTURE_2D,m_menuTextureId[_UItexID]);

    m_UI->bind();
    m_UI->draw();
    m_UI->unbind();

    glDisable(GL_BLEND);

}

//----------------------------------------------------------------------------------------------------------------------


void UI::loadMatricesToUIShader()
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["UI"]->use();
    ngl::Vec3 position = m_UITransform.getPosition();
    ngl::Vec3 scale = m_UITransform.getScale();
    shader->setRegisteredUniform("pos", position);
    shader->setRegisteredUniform("scale", scale);
}

//----------------------------------------------------------------------------------------------------------------------

void UI::drawDialogue(int _cureStage)
{

    //drawing the plague doctor
    if(_cureStage > 0)
    {
        m_UITransform.reset();
        m_UITransform.setPosition(-0.8,-0.8,0.0);
        m_UITransform.setScale(0.4,0.4,1.0);
        loadMatricesToUIShader();
        drawQuad(0);
    }

    //drawing first set of dialogue, this is when you start the quest
    if(_cureStage == 1)
    {
        m_UITransform.reset();
        m_UITransform.setPosition(-0.4,-0.7,-0.1);
        m_UITransform.setScale(0.6,0.6,1.0);
        loadMatricesToUIShader();
        drawQuad(1);
    }

    //drawing second set of dialogue, this is when you don't have all the items
    else if(_cureStage == 2)
    {
        m_UITransform.reset();
        m_UITransform.setPosition(-0.4,-0.7,-0.1);
        m_UITransform.setScale(0.6,0.6,1.0);
        loadMatricesToUIShader();
        drawQuad(2);
    }

    //third set, finished.
    else if(_cureStage == 3)
    {
        m_UITransform.reset();
        m_UITransform.setPosition(-0.4,-0.7,-0.1);
        m_UITransform.setScale(0.6,0.6,1.0);
        loadMatricesToUIShader();
        drawQuad(3);
    }


}

//----------------------------------------------------------------------------------------------------------------------

void UI::drawLifeCount(int _heartCount)
{
    //drawing of the life count, takes in th value _heartCount which is adjusted by how many times the enemies collide with the player

    if(_heartCount == 1)
    {
        m_UITransform.reset();
        m_UITransform.setPosition(-0.8,0.9,-0.4);
        m_UITransform.setScale(0.3,0.2,0.6);
        loadMatricesToUIShader();
        drawQuad(4);
    }

    else if(_heartCount == 2)
    {
        m_UITransform.reset();
        m_UITransform.setPosition(-0.8,0.9,0.0);
        m_UITransform.setScale(0.3,0.2,0.6);
        loadMatricesToUIShader();
        drawQuad(5);
    }

    else if(_heartCount == 3)
    {
        m_UITransform.reset();
        m_UITransform.setPosition(-0.8,0.9,0.0);
        m_UITransform.setScale(0.3,0.2,0.6);
        loadMatricesToUIShader();
        drawQuad(6);
    }

    else if(_heartCount == 4)
    {
        m_UITransform.reset();
        m_UITransform.setPosition(-0.8,0.9,0.0);
        m_UITransform.setScale(0.3,0.2,0.6);
        loadMatricesToUIShader();
        drawQuad(7);
    }

    else if(_heartCount == 5)
    {
        m_UITransform.reset();
        m_UITransform.setPosition(-0.8,0.9,0.0);
        m_UITransform.setScale(0.3,0.2,0.6);
        loadMatricesToUIShader();
        drawQuad(8);
    }
}

//----------------------------------------------------------------------------------------------------------------------

void UI::drawUI(int _heartCount, int _cureStage)
{
    //putting everything together

    drawLifeCount( _heartCount);
    //if the ui timer is less than 160, the dialogue will be drawn.
    if(m_triggerTimer == true)
    {
        if(m_uiTimer > 0)
        {
            drawDialogue( _cureStage);
        }
        else
        {
            m_triggerTimer= false;
        }
    }


    m_UITransform.reset();
    m_UITransform.setPosition(0.8,0.8,0.0);
    m_UITransform.setScale(0.4,0.4,1);
    loadMatricesToUIShader();
    drawQuad(9);

    m_UITransform.reset();
    m_UITransform.setPosition(0.6,-0.9,0.0);
    m_UITransform.setScale(0.3,0.3,1);
    loadMatricesToUIShader();
    drawQuad(15);

    m_UITransform.reset();
    m_UITransform.setPosition(0.87,-0.85,0.0);
    m_UITransform.setScale(0.3,0.3,1);
    loadMatricesToUIShader();
    drawQuad(16);

}

//----------------------------------------------------------------------------------------------------------------------

void UI::timerReset()
{
    // force the timer to reset back to 0 once the dialogue has stopped drawing, this allows you to go back and talk to him
    m_uiTimer = 100;
}

//----------------------------------------------------------------------------------------------------------------------

void UI::decrementUITimer()
{
    //called to imcrement the Ui timer
    m_uiTimer --;
}

//----------------------------------------------------------------------------------------------------------------------

void UI::deathByEnemiesScreen()
{

    m_UITransform.reset();
    m_UITransform.setPosition(0.0,0.3,0.0);
    m_UITransform.setScale(1.0,1.0,1);
    loadMatricesToUIShader();
    drawQuad(12);
    drawExitOptions();

}

//----------------------------------------------------------------------------------------------------------------------

void UI::deathByVolcanoScreen()
{
    m_UITransform.reset();
    m_UITransform.setPosition(0.0,0.0,0.0);
    m_UITransform.setScale(1.0,1.0,1);
    loadMatricesToUIShader();
    drawQuad(13);
    drawExitOptions();

}

//----------------------------------------------------------------------------------------------------------------------

void UI::winScreen()
{
    m_UITransform.reset();
    m_UITransform.setPosition(0.0,0.0,0.0);
    m_UITransform.setScale(1.0,1.0,1);
    loadMatricesToUIShader();
    drawQuad(14);
    drawExitOptions();
}

//----------------------------------------------------------------------------------------------------------------------

void UI::exitButtonScaling()
{
    //scaling for the exit button

    m_buttonScale1 = 1;
    m_buttonScale2 = 1;

    //depending on the exit state, the buttons will vary in size
    switch(m_exitState)
    {

    case 1:
        //make sure the button doesn't exceed in size as you increment the exit state
        if (m_buttonScale2 < 2)
        {
            m_buttonScale2 = m_buttonScale2 + 1;
        }

        //make sure the other button doesn't decrease too much
        else if(m_buttonScale1 >= 2)
        {
           m_buttonScale1 = m_buttonScale1- 1;
        }

        break;

    case 0:

         if (m_buttonScale2 >= 2)
        {
            m_buttonScale2 = m_buttonScale2 - 1;
        }

        else if(m_buttonScale1 < 2 )
        {
          m_buttonScale1 = m_buttonScale1 + 1;
        }
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void UI::drawExitOptions()
{

    m_UITransform.reset();
    m_UITransform.setPosition(0.0, -0.7, -0.2);
    m_UITransform.setScale(0.2 * m_buttonScale1, 0.2 * m_buttonScale1, 1.0);
    //draws the menu from the menu class
    loadMatricesToUIShader();
    drawQuad(11);

    exitButtonScaling();
    m_UITransform.reset();
    m_UITransform.setPosition(0, -0.4, -0.2);
    m_UITransform.setScale(0.2 * m_buttonScale2, 0.2 * m_buttonScale2, 1.0);
    loadMatricesToUIShader();
    drawQuad(10);

}

//----------------------------------------------------------------------------------------------------------------------

void UI::increaseExitState()
{
    //limit on how much the exit state will increase in size
    if(m_exitState < 1)
    {
        //incremement
        m_exitState++;
    }
}
//----------------------------------------------------------------------------------------------------------------------


void UI::decreaseExitState()
{
    //prevent the exit state from reducing below 0
    if(m_exitState > 0)
    {
        m_exitState--;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void UI::exitStateSwitch()
{
    switch(m_exitState)
    {

    //if m_exitState = 0, then game will exit, else it will restart the game
        case 1 :
        m_restartFlag = true;
        break;

        case 0 :
        m_exitFlag = true;
        break;
     }
}

//----------------------------------------------------------------------------------------------------------------------

void UI::deathByDiseaseScreen()
{
    m_UITransform.reset();
    m_UITransform.setPosition(0.0,0.1,0.0);
    m_UITransform.setScale(1.0,1.0,1);
    loadMatricesToUIShader();
    drawQuad(17);
    drawExitOptions();
}

//----------------------------------------------------------------------------------------------------------------------

bool UI::getRestartFlag() const
{
    return m_restartFlag;
}

//----------------------------------------------------------------------------------------------------------------------

void UI::setRestartFlag(bool _restartFlag)
{
    m_restartFlag = _restartFlag;
}

//----------------------------------------------------------------------------------------------------------------------

bool UI::getExitQuestionFlag() const
{
    return exitQuestionFlag;
}

//----------------------------------------------------------------------------------------------------------------------

void UI::setExitQuestionFlag(bool _exitQuestion)
{
    exitQuestionFlag = _exitQuestion;
}

//----------------------------------------------------------------------------------------------------------------------

void UI::setExitFlag(bool _exitFlag)
{
    m_exitFlag = _exitFlag;
}

//----------------------------------------------------------------------------------------------------------------------

bool UI::getExitFlag() const
{
    return m_exitFlag;
}

//----------------------------------------------------------------------------------------------------------------------

int UI::getUiTimer() const
{
    return m_uiTimer;
}

//----------------------------------------------------------------------------------------------------------------------

bool UI::getTriggerTimer() const
{
    return m_triggerTimer;
}

//----------------------------------------------------------------------------------------------------------------------

void UI::setTriggerTimer(bool _triggerTimer)
{
    m_triggerTimer = _triggerTimer;
}

//----------------------------------------------------------------------------------------------------------------------

void UI::SafeFromVolcanoScreen()
{
    m_UITransform.reset();
    m_UITransform.setPosition(-0.85,0.0,0.0);
    m_UITransform.setScale(0.3,0.3,1);
    loadMatricesToUIShader();
    drawQuad(18);
}
