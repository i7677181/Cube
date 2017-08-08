#include "GameMenu.h"
#include <vector>
#include <QGuiApplication>
#include <QApplication>
#include <ngl/Texture.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>

//----------------------------------------------------------------------------------------------------------------------

gamemenu::gamemenu()
{
    //initiate variables
    m_menuFlag = true;
    m_gameFlag = true;
    m_instructionsFlag = false;
    m_mstate = 0;
    m_UiOpen = false;
}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::menuQuad(){

    //creating a vector of strings to store IDs of textures
    std::vector<std::string> m_menuTextures;
    m_menuTextures.push_back("textures/Galaxy.tif");
    m_menuTextures.push_back("textures/menu_UI_Textures/newGame.png");
    m_menuTextures.push_back("textures/menu_UI_Textures/playgame2.png");
    m_menuTextures.push_back("textures/menu_UI_Textures/instructions2.png");
    m_menuTextures.push_back("textures/menu_UI_Textures/ExitGame2.png");
    m_menuTextures.push_back("textures/menu_UI_Textures/instructions_Page.png");

    //storing the IDs into vector
    for (unsigned int i = 0; i < m_menuTextures.size(); i++)
    {
        ngl::Texture m_menuTex(m_menuTextures[i]);
        //ID for texture
        GLint texID = m_menuTex.setTextureGL();
        m_menuTextureId.push_back(texID);
    }

    //creating the quad for the UI using a vertex array object
    m_UIQuad.reset(ngl::VertexArrayObject::createVOA(GL_TRIANGLES));
    m_UIQuad->bind();

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
    m_UIQuad->setIndexedData(12*sizeof(GLfloat),vertices[0],sizeof(indices),&indices[0],GL_UNSIGNED_BYTE,GL_STATIC_DRAW);
    // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
    m_UIQuad->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
    m_UIQuad->setIndexedData(8*sizeof(GLfloat),uv[0],sizeof(indices),&indices[0],GL_UNSIGNED_BYTE,GL_STATIC_DRAW);
    // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
    m_UIQuad->setVertexAttributePointer(1,2,GL_FLOAT,0,0); //bind verts to shader
    m_UIQuad->setNumIndices(sizeof(indices));

    // now unbind
    m_UIQuad->unbind();

}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::drawQuad(int _texID)
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    glEnable(GL_BLEND);
    glEnable(GL_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    (*shader)["UI"]->use();
    glBindTexture(GL_TEXTURE_2D,m_menuTextureId[_texID]);

    m_UIQuad->bind();
    m_UIQuad->draw();
    m_UIQuad->unbind();

    glDisable(GL_BLEND);

}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::menuButtonScaling()
{
    //set menu buttons to a scale of 1
    m_buttonScale1 = 1;
    m_buttonScale2 = 1;
    m_buttonScale3 = 1;
    m_buttonScale4 = 1;

    //switch statement, changed when the user wants to go up and down the menu buttons
    switch(m_mstate)
    {
    case 3:

        //adjusting the scaling by adding and subtracting to the values, only if theyre within certain bounds
        //to prevent the scaling to increase/decrease too much or at the wrong times
        if (m_buttonScale1 >= 2)
       {
           m_buttonScale1 = m_buttonScale1 - 1;
       }

       else if (m_buttonScale4 < 2)
       {
           m_buttonScale4 = m_buttonScale4 + 1;
       }

       break;

    case 2:
        if(m_buttonScale1 < 2)
        {
            m_buttonScale1 = m_buttonScale1 + 1;
        }

        else if (m_buttonScale2 >= 2)
        {
            m_buttonScale2 = m_buttonScale2 - 1;
        }

        else if( m_buttonScale4 >= 2)
        {
            m_buttonScale4 = m_buttonScale4 -1;
        }

        break;

    case 1:

        if (m_buttonScale2 < 2)
        {
            m_buttonScale2 = m_buttonScale2 + 1;
        }

        else if (m_buttonScale3 >= 2)
        {
            m_buttonScale3 = m_buttonScale3 - 1;
        }

        else if (m_buttonScale1 >= 2)
        {
            m_buttonScale1 = m_buttonScale1 - 1;
        }

        break;

    case 0:

         if (m_buttonScale2 >= 2)
        {
            m_buttonScale2 = m_buttonScale2 - 1;
        }

        else if (m_buttonScale3 < 2)
        {
            m_buttonScale3 = m_buttonScale3 + 1;
        }
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::drawMenu()
{
     //calls function to adjust the scaling sizes of buttons
     menuButtonScaling();

     //Creating the menu
     //transforms the position onto the screen
     m_menuTransform.reset();
     m_menuTransform.setPosition(0.0,0.5,0.0);
     m_menuTransform.setScale(5.0, 5.0, 1.0);
     //load matrices to the shader
     loadMatricesToMenuShader();
     //draw the quad
     drawQuad(0);

     m_menuTransform.reset();
     m_menuTransform.setPosition(-0.7, 0.8, -0.1);
     m_menuTransform.setScale(0.3 * m_buttonScale4, 0.3 * m_buttonScale4, 1.0);
     loadMatricesToMenuShader();
     drawQuad(1);

     m_menuTransform.reset();
     m_menuTransform.setPosition(-0.7, 0.3, -0.1);
     m_menuTransform.setScale(0.3 * m_buttonScale1, 0.3 * m_buttonScale1, 1.0);
     loadMatricesToMenuShader();
     drawQuad(2);

     m_menuTransform.reset();
     m_menuTransform.setPosition(-0.7, -0.3, -0.1);
     m_menuTransform.setScale(0.3 * m_buttonScale2, 0.3 * m_buttonScale2, 0.0);
     loadMatricesToMenuShader();
     drawQuad(3);

     m_menuTransform.reset();
     m_menuTransform.setPosition(-0.7, -0.8,-0.1);
     m_menuTransform.setScale(0.3 * m_buttonScale3, 0.3 * m_buttonScale3, 0.0);
     loadMatricesToMenuShader();
     drawQuad(4);

}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::playGame()
{
    //sets the booleans to control what is drawn to the screen
    m_gameFlag = true;
    m_menuFlag = false;
    m_UiOpen = true;
}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::newGame()
{
    m_gameFlag = true;
    m_UiOpen = true;
    m_menuFlag = false;
}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::instructions(int _texID){

    //loads up a screen showing the instructions for the game

    //setting variables
    m_gameFlag = false;
    m_menuFlag = false;
    m_UiOpen = false;
    m_instructionsFlag = true;

    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //choose shader
    (*shader)["UI"]->use();
    glBindTexture(GL_TEXTURE_2D,m_menuTextureId[_texID]);

    m_UIQuad->bind();
    m_UIQuad->draw();
    m_UIQuad->unbind();

}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::exit(){
    //terminates the game.
    QGuiApplication::exit(EXIT_SUCCESS);

}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::loadMatricesToMenuShader()
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["UI"]->use();
    ngl::Vec3 position = m_menuTransform.getPosition();
    ngl::Vec3 scale = m_menuTransform.getScale();
    shader->setRegisteredUniform("pos", position);
    shader->setRegisteredUniform("scale", scale);
}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::increaseMenuState()
{
    if (m_mstate < 3)
    {
        m_mstate ++;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::decreaseMenuState()
{
    if (m_mstate> 0)
    {
        m_mstate --;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::menuStateSwitch()
{
    //when the player presses enter, this will be called and depending on the menu state a function will be called.
    switch(m_mstate)
    {
        case 3 :
        newGame();
        break;

        case 2 :
        playGame();
        break;

        case 1 :
        instructions(5);
        break;

        case 0 :
        exit();
        break;
     }
}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::drawInstructions()
{
    m_menuTransform.reset();
    m_menuTransform.setPosition(0.0,0.5,0.0);
    m_menuTransform.setScale(5.0, 5.0, 1.0);
    loadMatricesToMenuShader();
    drawQuad(0);

    m_menuTransform.reset();
    m_menuTransform.setPosition(0.0,0.0,-0.1);
    m_menuTransform.setScale(1.5, 1.5, 1.0);
    loadMatricesToMenuShader();
    instructions(5);
}

//----------------------------------------------------------------------------------------------------------------------

bool gamemenu::getUiOpen() const
{
    return m_UiOpen;
}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::setUiOpen(bool _UiOpen)
{
    m_UiOpen = _UiOpen;
}

//----------------------------------------------------------------------------------------------------------------------

int gamemenu::getMenuState() const
{
    return m_mstate;
}

//----------------------------------------------------------------------------------------------------------------------

void gamemenu::setMenuState(int _menustate)
{
    m_mstate = _menustate;
}

//----------------------------------------------------------------------------------------------------------------------
