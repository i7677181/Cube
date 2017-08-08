#include <QMouseEvent>
#include <QGuiApplication>
#include <QApplication>
#include <algorithm>
#include <iostream>

#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/NGLStream.h>
#include "NGLScene.h"
#include "EnemyState.h"

const static float INCREMENT=0.01;
const static float ZOOM=0.5;
float s_sphereUpdate=0.06; //player speed?
bool playerIsActive=true;
bool pauseEnemy=false;
static bool grp1=true;

///----------------------------------------------------------------------------------------------------------------------
/// Class constructor
///----------------------------------------------------------------------------------------------------------------------
Window::Window()
{
    ///for the mouse event
    m_rotate=false;
    m_spinXFace=0;
    m_spinYFace=0;
    setTitle("Oh no! Disaster!");

    m_updateSphereTimer=startTimer(2);
    m_redrawTimer=startTimer(4);
    m_keysPressed.clear();
    ///set light positions
    m_lightPosition1.set(-3,3,-3);
    m_lightPosition2.set(0,3,0);
    m_lightPosition3.set(0,-3,3);
    m_lightPosition4.set(3,-3,-3);

    m_hit=0;
    m_isAttacking2=false;
    m_isSafeFromEruption = false;
    m_safeFromEruptionUiTimer = 51;
    m_isSafeFromEruptionTimerTriggered = false;
}

///----------------------------------------------------------------------------------------------------------------------
/// Class destructor
///----------------------------------------------------------------------------------------------------------------------
Window::~Window()
{
    delete m_timer;
    std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}
///----------------------------------------------------------------------------------------------------------------------
/// resizing window
///----------------------------------------------------------------------------------------------------------------------
void Window::resizeGL(QResizeEvent *_event)
{
    m_width=_event->size().width()*devicePixelRatio();
    m_height=_event->size().height()*devicePixelRatio();
    m_cam->setShape(45.0f,(float)width()/height(),0.05f,350.0f);
}
///----------------------------------------------------------------------------------------------------------------------
/// resizing window
///----------------------------------------------------------------------------------------------------------------------
void Window::resizeGL(int _w , int _h)
{
    m_cam->setShape(45.0f,(float)_w/_h,0.05f,350.0f);
    m_width=_w*devicePixelRatio();
    m_height=_h*devicePixelRatio();
}
///----------------------------------------------------------------------------------------------------------------------
/// initialising function
///----------------------------------------------------------------------------------------------------------------------
void Window::initializeGL()
{
    ngl::NGLInit::instance();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    /// get an instance of the shader library
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    /// create the shader programs
    shader->createShaderProgram("Phong");
    shader->createShaderProgram("Texture");
    shader->createShaderProgram("UI");

    /// attach the shaders to the shader program
    shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
    shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
    shader->attachShader("TextureVertex",ngl::ShaderType::VERTEX);
    shader->attachShader("TextureFragment",ngl::ShaderType::FRAGMENT);
    shader->attachShader("UiVertex",ngl::ShaderType::VERTEX);
    shader->attachShader("UiFragment",ngl::ShaderType::FRAGMENT);

    /// Load the shader sources
    shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
    shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
    shader->loadShaderSource("TextureVertex","shaders/TextureVertex.glsl");
    shader->loadShaderSource("TextureFragment","shaders/TextureFragment.glsl");
    shader->loadShaderSource("UiVertex","shaders/uivertex.glsl");
    shader->loadShaderSource("UiFragment","shaders/uifragment.glsl");

    ///compile the shaders
    shader->compileShader("PhongVertex");
    shader->compileShader("PhongFragment");
    shader->compileShader("TextureVertex");
    shader->compileShader("TextureFragment");
    shader->compileShader("UiVertex");
    shader->compileShader("UiFragment");

    /// attach shader to main program
    shader->attachShaderToProgram("Phong","PhongVertex");
    shader->attachShaderToProgram("Phong","PhongFragment");
    shader->attachShaderToProgram("Texture","TextureVertex");
    shader->attachShaderToProgram("Texture","TextureFragment");
    shader->attachShaderToProgram("UI","UiVertex");
    shader->attachShaderToProgram("UI","UiFragment");

    /// bind shader attributes
    shader->bindAttribute("Phong",0,"inVert");
    shader->bindAttribute("Phong",1,"inUV");
    shader->bindAttribute("Phong",2,"inNormal");
    shader->bindAttribute("Texture",0,"inVert");
    shader->bindAttribute("Texture",1,"inUV");
    shader->bindAttribute("UI",0,"inVert");
    shader->bindAttribute("UI",1,"inUV");
    shader->linkProgramObject("Phong");
    shader->linkProgramObject("Texture");
    shader->linkProgramObject("UI");

    /// use the phong shader as the main shader
    (*shader)["Phong"]->use();
    shader->setShaderParam1i("Normalize",1);

    /// camera properties
    ngl::Vec3 from(5,18,20);
    ngl::Vec3 to(0,0,0);
    ngl::Vec3 up(0,1,0);

    /// set up the camera
    m_cam= new ngl::Camera(from,to,up);
    m_cam->setShape(45.0f,(float)720.0/576.0,0.05,350);

    ngl::Mat4 iv=m_cam->getViewMatrix();

    ///reset light 1 with colour and mode and set transform
    m_light.reset( new ngl::Light(m_lightPosition1,ngl::Colour(0.9,0.4,0.9,1.0),ngl::LightModes::POINTLIGHT));
    m_light->setTransform(iv);

    ///reset light 2 with colour and mode and set transform
    m_light2.reset( new ngl::Light(m_lightPosition2,ngl::Colour(0.8,1.5,0.8,1.0),ngl::LightModes::POINTLIGHT));
    m_light2->setTransform(iv);

    ///reset light 3 with colour and mode and set transform
    m_light3.reset( new ngl::Light(m_lightPosition3,ngl::Colour(0.8,1.0,1.0,1.0),ngl::LightModes::POINTLIGHT));
    m_light3->setTransform(iv);

    ///reset light 4 with colour and mode and set transform
    m_light4.reset( new ngl::Light(m_lightPosition4,ngl::Colour(1.0,0.0,1.0,1.0),ngl::LightModes::POINTLIGHT));
    m_light4->setTransform(iv);

    /// Set material properties
    ngl::Material m;
    /// set material diffuse
    m.setDiffuse(ngl::Colour(0.6,1.0,0.6,1.0));
    ///set material ambience
    m.setAmbient(ngl::Colour(1.0,1.0,1.0,1.0));
    ///set material specular
    m.setSpecular(ngl::Colour(1,1,1,1));
    ///set material specular component
    m.setSpecularExponent(80);
    ///load material properties to shader
    m.loadToShader("material");
    ///initialise new timer
    m_timer=new Timer();
    /// initialise emitter
    m_emitter.reset(new Emitter(ngl::Vec3(-1,10,-1),100,&m_spread));
    ///set emitter to be dependant on camera
    m_emitter->setCam(&*m_cam);
    /// start particle timer
    m_particleTimer=startTimer(50);

    /** player, enemy, collision, cube and texture initiations*/
    /// get an instance of VAO primitives
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    ///create player sphere
    prim->createSphere("sphere",0.2,50);
    m_sphere.reset(new Sphere(ngl::Vec3(0,5.5,0)));
    m_transform.setPosition(m_sphere->getPosition());

    m_enemy.reset(new EnemyState);
    m_enemy->createEnemy();
    ///new enemy function,creates multiple enemies
    bumpEnemy();
    /// initialise space background and load in texture
    m_space = new Space("textures/Galaxy.tif",m_cam);

    /// initialising all the objects to be printed on screen
    /// Objects on Village side of the world
    objectsVector.push_back(GeneralObj(ngl::Vec3 (5.0, 3.0, 3.4), ngl::Vec3 (0, 90, -90), ngl::Vec3 (0.6, 0.6, 0.6), "Tower", "assets/Tower.obj", "textures/village.jpg"));               //0
    objectsVector.push_back(GeneralObj(ngl::Vec3 (5.0, 0.0, 3.5), ngl::Vec3 (0, 100, -90), ngl::Vec3 (0.6, 0.6, 0.6), "Village", "assets/House.obj", "textures/house.jpg"));              //1
    objectsVector.push_back(GeneralObj(ngl::Vec3 (5.0, -3.0, 2.3), ngl::Vec3 (0, 170, -90), ngl::Vec3 (0.5, 0.6, 0.6), "Village", "assets/House.obj", "textures/house.jpg"));             //2
    objectsVector.push_back(GeneralObj(ngl::Vec3 (5.0, -1.8, -0.5), ngl::Vec3 (0, 240, -90), ngl::Vec3 (0.5, 0.5, 0.5), "Village", "assets/House.obj", "textures/house.jpg"));            //3
    objectsVector.push_back(GeneralObj(ngl::Vec3 (5.0, -3.2, 0.1), ngl::Vec3 (0, 170, -90), ngl::Vec3 (0.6, 0.6, 0.6), "Village Tree", "assets/Tree_1.obj", "textures/tree1.jpg"));       //4
    objectsVector.push_back(GeneralObj(ngl::Vec3 (5.0, 4.0, -2.0), ngl::Vec3 (0, 100, -90), ngl::Vec3 (0.6, 0.5, 0.6), "Village Tree", "assets/Tree_1.obj", "textures/tree1.jpg"));       //5
    objectsVector.push_back(GeneralObj(ngl::Vec3 (5.2, 3.0, 1.3), ngl::Vec3 (0, 0, -90), ngl::Vec3 (0.25, 0.25, 0.25), "Doctor", "assets/Doctor.obj", "textures/doctor.jpg"));            //6
    objectsVector.push_back(GeneralObj(ngl::Vec3 (5.0, 3.4,-3.4), ngl::Vec3 (0,-30, -90), ngl::Vec3 (0.5, 0.5, 0.5), "DocHouse", "assets/House.obj", "textures/house.jpg"));              //7
    objectsVector.push_back(GeneralObj(ngl::Vec3 (5.2, 1.4, -3.0), ngl::Vec3 (0, 170, -90), ngl::Vec3 (0.2, 0.2, 0.2), "Mercury", "assets/Mercury.obj", "textures/darkgrey.jpg"));        //8

    /// Objects on Volcano side of the world
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-5.4,-0.9,-0.7), ngl::Vec3 (0, 0, 90), ngl::Vec3 (4.6, 5.5, 4.6), "Volcano", "assets/Volcano.obj", "textures/volcano.jpg"));            //9
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-6.4, -4.0, 0), ngl::Vec3 (0, 0, 100), ngl::Vec3 (0.5, 0.5, 0.5), "Flowers", "assets/Flowers.obj", "textures/flowers.jpg"));            //10

    /// Objects on Starting side of the world
    objectsVector.push_back(GeneralObj(ngl::Vec3 (2.1, 6.0, -3.1), ngl::Vec3 (0,0,0), ngl::Vec3 (0.5, 0.5, 0.5), "RockClump", "assets/Rock_Clump.obj", "textures/grey.jpg"));             //11
    objectsVector.push_back(GeneralObj(ngl::Vec3 (0.0, 5.4, -4.0), ngl::Vec3 (0,0,0), ngl::Vec3 (0.5, 0.4, 0.5), "Forest", "assets/Tree_3.obj", "textures/tree3.jpg"));                   //12
    objectsVector.push_back(GeneralObj(ngl::Vec3 (2.6, 5.96, -2.0), ngl::Vec3 (0,30,0), ngl::Vec3 (0.5, 0.5, 0.5), "Forest", "assets/Tree_3.obj", "textures/tree3.jpg"));                 //13
    objectsVector.push_back(GeneralObj(ngl::Vec3 (4.0, 5.2, -3.9), ngl::Vec3 (0,0,0), ngl::Vec3 (0.5, 0.5, 0.5), "Forest addup", "assets/Tree_4.obj", "textures/tree4.jpg"));             //14
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-2.5, 5.6, -2.0), ngl::Vec3 (0,0,0), ngl::Vec3 (0.5, 0.5, 0.5), "Forest addup", "assets/Tree_4.obj", "textures/tree4.jpg"));            //15

    /// Objects on Dead side of the world
    objectsVector.push_back(GeneralObj(ngl::Vec3 (3.5, 1.5, -5.6), ngl::Vec3 (-90,0,0), ngl::Vec3 (0.5, 0.5, 0.5), "Rock Clump", "assets/Rock_Clump.obj", "textures/darkgrey.jpg"));      //16
    objectsVector.push_back(GeneralObj(ngl::Vec3 (0.2, 2.8, -5.7), ngl::Vec3 (-90,0,100), ngl::Vec3 (0.4, 0.4, 0.4), "Rock", "assets/Rock.obj", "textures/grey.jpg"));                    //17
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-1.8, -4.4, -5.6), ngl::Vec3 (-90,0,100), ngl::Vec3 (0.5, 0.5, 0.5), "Rock Pile", "assets/Rock.obj", "textures/darkgrey.jpg"));         //18
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-3.5, -3.3, -5.5), ngl::Vec3 (-90,0,45), ngl::Vec3 (0.6, 0.7, 0.8), "Rock Pile", "assets/Rock.obj", "textures/darkgrey.jpg"));          //19
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-4.4, -4.3, -5.3), ngl::Vec3 (-90,0,100), ngl::Vec3 (1.0, 1.0, 1.0), "Rock Pile", "assets/Rock_Clump.obj", "textures/grey.jpg"));       //20
    objectsVector.push_back(GeneralObj(ngl::Vec3 (0.8, -2.8, -5.8), ngl::Vec3 (-90,0,100), ngl::Vec3 (0.4, 0.4, 0.4), "Cloves", "assets/Clove.obj", "textures/cloves.tif"));              //21

    /// Objects on garden side of the world
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-2.3, -3.0, 5.5), ngl::Vec3 (90,0,100), ngl::Vec3 (0.6, 0.5, 0.6), "Tree", "assets/Tree_2.obj", "textures/tree2.jpg"));                 //22
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-3.5, -2.0, 5.7), ngl::Vec3 (90,0,150), ngl::Vec3 (0.6, 0.6, 0.6), "Tree", "assets/Tree_2.obj", "textures/tree2.jpg"));                 //23
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-1.2, -0.5, 6.0), ngl::Vec3 (90,0,180), ngl::Vec3 (0.6, 0.6, 0.6), "Tree", "assets/Tree_2.obj", "textures/tree2.jpg"));                 //24
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-2.2, -2.2, 5.5), ngl::Vec3 (90,0,100), ngl::Vec3 (0.4, 0.4, 0.4), "Lavender", "assets/Lavender.obj", "textures/lavender.jpg"));        //25
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-2.4, -1.0, 5.5), ngl::Vec3 (90,0,80), ngl::Vec3 (0.4, 0.4, 0.4), "Lavender", "assets/Lavender.obj", "textures/lavender.jpg"));         //26
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-0.7, -3.2, 5.5), ngl::Vec3 (90,0,90), ngl::Vec3 (0.4, 0.4, 0.4), "Lavender", "assets/Lavender.obj", "textures/lavender.jpg"));         //27
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-3.7, -3.9, 5.0), ngl::Vec3 (90,0,130), ngl::Vec3 (0.4, 0.4, 0.4), "Lavender", "assets/Lavender.obj", "textures/lavender.jpg"));        //28
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-0.8, -2.3, 6.4), ngl::Vec3 (90,0,0), ngl::Vec3 (0.3, 0.3, 0.3), "Rose", "assets/Rose.obj", "textures/rose.jpg"));                      //29
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-3.6, -3.0, 5.9), ngl::Vec3 (90,-30,20), ngl::Vec3 (0.3, 0.3, 0.3), "Rose", "assets/Rose.obj", "textures/rose.jpg"));                   //30
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-1.0, -1.0, 6.3), ngl::Vec3 (90, 0, 20), ngl::Vec3 (0.4, 0.4, 0.4), "Flowers", "assets/Flowers.obj", "textures/flowers.jpg"));          //31
    objectsVector.push_back(GeneralObj(ngl::Vec3 (-4.1, -0.4, 5.8), ngl::Vec3 (90, 0, 90), ngl::Vec3 (0.4, 0.4, 0.4), "Flowers", "assets/Flowers.obj", "textures/flowers.jpg"));          //32

    ///creates the vao for a quad to be used for the menu and UI
    m_menu.menuQuad();
    m_UI.initUI();

    /// start light timer
    m_lightTimer=startTimer(10);

    /// creating new terrain object
    m_terrain.reset(new Terrain);
    ///initializing terrain:
    m_terrain->initializeTerrain();

    /// initialising the texts for on screen printing
    m_text1.reset( new ngl::Text(QFont("Arial",20)));
    m_text2.reset( new ngl::Text(QFont("Arial",16)));
    m_printPtimer.reset( new ngl::Text(QFont("Arial",20)));
    m_printVtimer.reset( new ngl::Text(QFont("Arial",20)));
    m_printInstructions.reset( new ngl::Text(QFont("Arial",16)));
    m_printMenu.reset( new ngl::Text(QFont("Arial",16)));
    m_text1->setScreenSize(width(),height());
    m_text2->setScreenSize(width(),height());
    m_printPtimer->setScreenSize(width(),height());
    m_printVtimer->setScreenSize(width(),height());
    m_printMenu->setScreenSize(width(),height());
    m_printInstructions->setScreenSize(width(),height());
}

///----------------------------------------------------------------------------------------------------------------------
/// Load matrix function
///----------------------------------------------------------------------------------------------------------------------
void Window::loadMatricesToShader()
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M=m_transform.getMatrix()*m_mouseGlobalTX;
    ngl::Mat4 MV=M*m_cam->getViewMatrix() ;
    ngl::Mat4 MVP = m_transform.getMatrix()*m_mouseGlobalTX*m_cam->getViewMatrix()*m_cam->getProjectionMatrix();

    normalMatrix=MV;
    normalMatrix.inverse();
    shader->setShaderParamFromMat4("MVP",MVP);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
}

///----------------------------------------------------------------------------------------------------------------------
///This function rotates an object globally around the pivot, which is at 0,0,0
///It takes in the position, global rotation (which is always m_rotationCurrent), rotation of that object at the start
///and scale.
///This function returns the new transformation matrix of that object depending on the rotation of the cube.
///We can use this matrix to find the new position of the object
///----------------------------------------------------------------------------------------------------------------------
ngl::Mat4 Window::PivotRotation(ngl::Vec3 _position, ngl::Mat4 _rotation, ngl::Vec3 _initialRotation, ngl::Vec3 _scale)
{
    ngl::Mat4 new_transformation, position_mat, pivot, scale, r;

    scale.scale(_scale.m_x, _scale.m_y, _scale.m_z);                        //setting the scale
    position_mat.translate(_position.m_x, _position.m_y, _position.m_z);    //setting the translation

    ngl::Mat4 rx; rx.rotateX(_initialRotation.m_x);
    ngl::Mat4 ry; ry.rotateY(_initialRotation.m_y);
    ngl::Mat4 rz; rz.rotateZ(_initialRotation.m_z);
    r= rx*ry*rz;                                                            //setting the rotation
    new_transformation = scale*r*position_mat*_rotation*pivot;              //creating the transformation matrix
    return new_transformation;
}

///----------------------------------------------------------------------------------------------------------------------
/// draw function
///----------------------------------------------------------------------------------------------------------------------
void Window::drawScene(const std::string &_shader)
{
    ///if the timer for the UI is needed
    if(m_UI.getTriggerTimer() == true)
    {
        m_UI.decrementUITimer();
    }

    ///if the timer goes over 130 frames, it will stop incrementing
    if(m_UI.getUiTimer() > 130)
    {
        m_UI.setTriggerTimer(false);
    }


    ///triggering volcano eruption screen when you're safe
    if(m_isSafeFromEruption == true)
    {
        //trigger timer
        m_isSafeFromEruptionTimerTriggered = true;
    }

    /// if the timer for a safe by volcano screen is triggered, start incrementing value
    if(m_isSafeFromEruptionTimerTriggered == true)
    {
         m_safeFromEruptionUiTimer = m_safeFromEruptionUiTimer + 1;
         std::cout<<m_safeFromEruptionUiTimer<<std::endl;
    }


    ///while the timer is less than 50 draw the icon to indicate that player is safe from eruption
    if((m_safeFromEruptionUiTimer < 35) && (m_isSafeFromEruptionTimerTriggered == true) && m_menu.getUiOpen())
    {
        m_UI.SafeFromVolcanoScreen();
    }
    else if(m_safeFromEruptionUiTimer >= 35)
    {
        m_isSafeFromEruption = false;
        m_isSafeFromEruptionTimerTriggered = false;
        m_safeFromEruptionUiTimer = 0;
    }

    ///getting the position of the tree to figure out if the player is on the right face
    if(m_treePosition.m_y>5.3 && m_treePosition.m_y<5.5)
    {
        m_enemyGlobalRotation.identity();
    }
    ///getting the position of the lavender to figure out if the player is on the right face
    if(m_lavenderPosition.m_y>5.4 && m_lavenderPosition.m_y<5.5)
    {
        m_enemyGlobalRotation2.identity();
    }
    ///getting the matrix from sphere class to set the new rotation
    if (m_sphere->rotateWorld==true)
    {
        m_globalRotation = m_sphere->getRotation();
        m_rotationCurrent=m_rotationCurrent*m_globalRotation;
        m_enemyGlobalRotation=m_enemyGlobalRotation*m_globalRotation;
        m_enemyGlobalRotation2=m_enemyGlobalRotation2*m_globalRotation;
        Rotationtimer++;
        s_sphereUpdate=0;
    }
    ///stops rotation and enables player movement again
    if (Rotationtimer==6)
    {
        Rotationtimer=0;
        m_sphere->rotateWorld=false;
        // updates first and last cubes in when rotating the world
        m_sphere->setFirstAndLastCubePositions(m_cubesFirstAndLast);
        // when rotating the world, the terrains have to be tracked to make the player move on top of them
        m_sphere->trackTerrain();
        s_sphereUpdate=0.04;
    }
    /// get an instance of the shader library
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    ///set phong as primary shader
    (*shader)["Phong"]->use();
    ngl::Mat4 rotX;
    ngl::Mat4 rotY;
    rotX.rotateX(m_spinXFace);
    rotY.rotateY(m_spinYFace);
    m_mouseGlobalTX=rotY*rotX;
    m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
    m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
    m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

    rotX.rotateX(m_spinXFace);
    rotY.rotateY(m_spinYFace);
    m_mouseGlobalTX=rotY*rotX;
    m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
    m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
    m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

    bool menu = m_menu.getMenuBool();
    if(menu == true )
    {
        ///pause the game
        Pause();
        ///draw menu
        m_menu.drawMenu();
        ///pauses the timer for the volcano and plague spread
        m_timer->setActive(false);
        ///sets the variable to start the game timer to 0
        m_gameTimerStarterCounter = 0;

    }

    bool m_gameOn = m_menu.getGamePlayBool();

    if (m_gameOn  == true)
    {
        ///set phong shader as primary shader
        (*shader)["Phong"]->use();

        ///starts the timer, it doesn't get set back to 0 elsewhere so it is used only to start the main game timer when you press play game for the first time.
        if(!menu && m_gameTimerStarterCounter == 0)
        {
            m_timer->timerStart();
            m_gameTimerStarterCounter++;
            Reset();
        }

        Resume();

        m_transform.reset();
        {
            m_transform.setPosition(m_sphere->getPosition());
            loadMatricesToShader();
            ///set shader to flat diffuse and assign colour
            shader->setUniform("Colour", 1.0f,0.0f,0.8f);//Set colour for flat diffuse colour shader
            shader->setUniform("myflag",1);//Flag enables the flat diffuse colour shader
            m_sphere->draw();
        }

        if(m_timer->getPlagueTimer()==true)setEnemy(); //if plague timer is active start doing enemy stuff

        /// get width and height of plane edge here;
        float height=5;
        float width=5;
        m_sphere->planeEdge(width,height);
        m_transform.reset();
        {
            ///scaling of cube
            m_transform.setMatrix(PivotRotation(ngl::Vec3(0,0,0), m_rotationCurrent, ngl::Vec3(0,0,0), ngl::Vec3(width*2,width*2,width*2)));
            loadMatricesToShader();
            ///set shader flag to 0 to enable phong calculations
            shader->setUniform("myflag",0);//Flag enables the phong shader
            ngl::VAOPrimitives::instance()->draw("cube");
        }
        /// Draw background
        m_space->draw();

        ///get an instance of VAE primitives
        ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

        /// the position and the scale for the small cubes on each terrain
        std::vector<ngl::Transformation> cubePosScale1;
        std::vector<ngl::Transformation> cubePosScale2;
        std::vector<ngl::Transformation> cubePosScale3;
        std::vector<ngl::Transformation> cubePosScale4;
        std::vector<ngl::Transformation> cubePosScale5;
        std::vector<ngl::Transformation> cubePosScale6;
        /// receiving the scales and positions of the small cubes in each terrain
        cubePosScale1=m_terrain->getCubePosScale1();
        cubePosScale2=m_terrain->getCubePosScale2();
        cubePosScale3=m_terrain->getCubePosScale3();
        cubePosScale4=m_terrain->getCubePosScale4();
        cubePosScale5=m_terrain->getCubePosScale5();
        cubePosScale6=m_terrain->getCubePosScale6();
        /// storing the rotated terrain positions
        ngl::Mat4 newPosition;
        /// temporary transformation variable storing the scales and positions of each small cube
        ngl::Transformation Tr1;
        /// looping through each side and each small cube to draw them
        for (int sideCounter=1; sideCounter<7; sideCounter++)
        {
            for (int loop=0; loop<1600; loop++)
            {
                if (sideCounter==1) {Tr1=cubePosScale1.at(loop);}
                else if (sideCounter==2) {Tr1=cubePosScale2.at(loop);}
                else if (sideCounter==3) {Tr1=cubePosScale3.at(loop);}
                else if (sideCounter==4) {Tr1=cubePosScale4.at(loop);}
                else if (sideCounter==5) {Tr1=cubePosScale5.at(loop);}
                else if (sideCounter==6) {Tr1=cubePosScale6.at(loop);}
                /// updating the positions of the small cubes
                newPosition = PivotRotation(ngl::Vec3(Tr1.getPosition()),m_rotationCurrent, ngl::Vec3(0,0,0),ngl::Vec3(Tr1.getScale()));
                /// applying the transformations depending on the new positions
                m_transform.setMatrix(newPosition);
                /// updating the first and the last small cube on each terrain
                if (loop==0) {m_cubesFirstAndLast[sideCounter][0]=ngl::Vec3(newPosition.m_30,newPosition.m_31,newPosition.m_32);}
                if (loop==1599) {m_cubesFirstAndLast[sideCounter][1]=ngl::Vec3(newPosition.m_30,newPosition.m_31,newPosition.m_32);}
                /// loading shader
                (*shader)["Phong"]->use();
                loadMatricesToShader();
                /// drawing one small cube
                prim->draw("cube");
            }
        }

        m_transform.reset();

        {
            ngl::Mat4 new_transformations;
            new_transformations=PivotRotation(ngl::Vec3(0,5,0),m_rotationCurrent, ngl::Vec3(0,0,0),ngl::Vec3(1,1,1));

            m_emitter->setRot(m_rotationCurrent);
            m_emitter->setMouse(m_mouseGlobalTX);
            m_emitter->draw(m_timer->isErrupting());

            /// If volcano is upside down and erupts you survie
            if(m_sphere->getVolcanoStatus()==true && m_timer->isErrupting()==true)
            {
                m_gravity=-0.1;
                m_spread.set(0.1,1,0.1);
                m_isSafeFromEruption = true;

            }

            /// if volcano is upside down yet doesnt erupt you surive
            if(m_sphere->getVolcanoStatus()==true && m_timer->isErrupting()==false)
            {
                m_gravity=-0.1;
                m_spread.set(0.1,1,0.1);
            }

            /// if volcano is positioned anywhere else and doesnt erupt you survive
            if(m_sphere->getVolcanoStatus()==false && m_timer->isErrupting()==false)                m_sphere->setHit();

            {
                m_gravity=-0.1;
                m_spread.set(0.1,1,0.1);

            }

            /// if volcano is positioned anywhere else and erupts you lose
            if(m_sphere->getVolcanoStatus()==false && m_timer->isErrupting()==true)
            {
                m_gravity=-9.8;
                m_spread.set(1,1,1);

                ///tells the game that the volcano has erupted
                m_gamePlay.setGameErupted(true);
                m_gamePlay.setGameLost(true);
            }
        }
///----------------------------------------- draw meshes -----------------------------------------//
        for (unsigned int i=0; i < objectsVector.size(); ++i)
        {
            /// Set use of texture shader
            (*shader)["Texture"]->use();
            m_transform.reset();
            /// Set the position of the object to the position after the rotation of the cube has been taken into account
            ngl::Vec3 new_position;
            ngl::Mat4 transformation = PivotRotation(objectsVector[i].getPos(),m_rotationCurrent,objectsVector[i].getRot(),objectsVector[i].getScale());
            new_position = ngl::Vec3(transformation.m_30, transformation.m_31, transformation.m_32);
            objectsVector[i].setRotatedPos(new_position);
            m_transform.setMatrix(transformation);
            loadMatricesToShader();
            /// Draw the objects in the vector to the scene
            objectsVector[i].drawObj();

            if(i==12) {m_treePosition=new_position;}
            else if (i==22){m_lavenderPosition=new_position;}
        }

        ///draws the UI if set to true
        if(m_menu.getUiOpen() == true)
        {
            ///opens the menu, inputs the heartcount and cure stage of the game
            m_UI.drawUI(m_gamePlay.heartcount(),cureStage);
        }

        if((m_timer->counterD > 120) && (m_gamePlay.gameLost() == false))
        {
            m_gamePlay.setGameDisease(true);
            m_gamePlay.setGameLost(true);
        }

        if((m_timer->counterD > 120) && (m_gamePlay.gameLost() == false))
        {
            m_gamePlay.setGameDisease(true);
            m_gamePlay.setGameLost(true);
        }

        ///if the player has won the game
        if(m_gamePlay.gameWon() == true)
        {
            m_menu.setUiOpen(false);
            m_UI.winScreen();
            m_UI.setExitQuestionFlag(true);
            Pause();
        }

        ///if the player loses
        if(m_gamePlay.gameLost() == true)
        {
            m_menu.setUiOpen(false);

            ///if statements that have multiple condiitons to prevent several loss screens drawn at the same time
            if((m_gamePlay.heartcount() == 0) && (m_gamePlay.gameErupted() == false))
            {
                ///draws death screen
                m_UI.deathByEnemiesScreen();
                ///pause game
                Pause();
                ///sets the exit flag to true, allowing the player to choose whether or not to restart game
                m_UI.setExitQuestionFlag(true);
            }

            ///if volcano has erupted
           else if((m_gamePlay.gameErupted() == true) && (m_gamePlay.heartcount() > 0))
            {
                ///death by volcano screen
                m_UI.deathByVolcanoScreen();
                ///pause
                Pause();
                ///draw exit options, allow for user to exit/restart game
                m_UI.setExitQuestionFlag(true);
            }

           else if((m_gamePlay.gameErupted() == false) && (m_gamePlay.heartcount() > 0) && (m_gamePlay.gameDisease() == true))
            {
                ///death by volcano screen
                m_UI.deathByDiseaseScreen();
                ///pause
                Pause();
                ///draw exit options, allow for user to exit/restart game
                m_UI.setExitQuestionFlag(true);
            }

        }

   }

   ///instruction page
   bool _instructionMenu = m_menu.getInstructionBool();
   if(_instructionMenu == true )
   {
       m_menu.drawInstructions();
   }
}

///----------------------------------------------------------------------------------------------------------------------
void Window::setEnemy()
{
//while grp1 is not active displays enemies inForest at their init pos while player is on other planes
  for(unsigned int i=0;i<m_enemy->enemyForest.size();i++)
  {
    if(grp1==false){
        for(unsigned int i=0;i<m_enemy->enemyForest.size();i++) //for each enemy check if player in attack radius
        {
            m_enemy->enemyForest[i].setPosition(m_enemy->enemyStartPos[i]);
            loadMatricesToShader();
            setEnemyRotForest();
        }
    }
  //while grp1 is not active displays enemies inForest at their init pos while player is on other planes
   else if(grp1==true)
   {
        m_isAttacking=false;//prevents from moving

         if(m_isAttacking==false)
         {
              loadMatricesToShader();
              setEnemyRotForest();

         }
         if(sphereSphereCollision(m_sphere->getPosition(),1.0,
                                    m_enemy->enemyForest[i].getPosition(),1.0)==true){
              m_isAttacking=true;
              if(m_enemy->enemyForest[i].isFrozen()==false && m_isAttacking==true){
              loadMatricesToShader();
              if(pauseEnemy==false){
              m_enemy->enemyForest[i].setPrey(m_sphere->getPosition());
              m_enemy->enemyForest[i].move(m_sphere->getPosition());
              m_enemy->enemyForest[i].draw();
              }

              }

             bool enemiesOverlapping=sphereSphereCollision(m_enemy->enemyForest[i].getPosition(),.9,
                                                             m_enemy->enemyForest[i+1].getPosition(),.9);
             if(enemiesOverlapping){ngl::Vec3 dir = (m_enemy->enemyForest[i].getPosition() -
                                                        m_enemy->enemyForest[i+1].getPosition());
                        m_enemy->enemyForest[i].setAvoid(dir);
            }

         }
         m_enemy->isReset=false;
     }

   }
//----------------------------------------------------------------------------------------------------------------------
   for(unsigned int i=0;i<m_enemy->enemyGarden.size();i++) //for each enemy check if player in attack radius
   {
      m_isAttacking2=false;

      if(m_sphere->isGarden == false && m_isAttacking2 == false) //if player is not on garden plane
      {
          if(m_sphere->isForest==true)grp1=true;else grp1=false; //if player is on Forest plane, forestEnemy active
          m_enemy->isReset=false; //flag reset each time player is away from garden
          setEnemyRotGarden();
       }

       if(m_sphere->isGarden==true) //if player on garden plane
        {
            grp1=false;//deactivate forest grp, so they don't chase or display when player is not inForest
            if(m_enemy->isReset==false && grp1==false)for(int j=0;j<5;j++){
                /**Resets positions of enemies to be on the player plane.Reset rotation matrix                                                          so Garden enemies get correct position of player**/
                m_enemy->resetGarden();
            }
            if(sphereSphereCollision(m_sphere->getPosition(),1.0,
                                               m_enemy->enemyGarden[i].getPosition(),1.0) == true){ //attack radius
                m_isAttacking2=true;
                if (m_isAttacking2==true)
                    m_enemy->enemyGarden[i].setPrey(m_sphere->getPosition());
                    m_enemy->enemyGarden[i].move(m_sphere->getPosition());
                    setEnemyRotGarden();

                      }
             else{
                  m_isAttacking2=false;
                  m_transform.setPosition(m_enemy->enemyGarden[i].getPosition());
                  loadMatricesToShader();
                  m_enemy->enemyGarden[i].draw();
             }
              bool enemiesOverlapping=sphereSphereCollision(m_enemy->enemyGarden[i].getPosition(),.9,
                                                            m_enemy->enemyGarden[i+1].getPosition(),.9);
              if(enemiesOverlapping){ngl::Vec3 dir = (m_enemy->enemyGarden[i].getPosition() -
                                                       m_enemy->enemyGarden[i+1].getPosition());
                       m_enemy->enemyGarden[i].setAvoid(dir);
              }

          }//for plane

   }//for all

}
//----------------------------------------------------------------------------------------------------------------------
void Window::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,m_width,m_height);
    /// get an instance of the shader library
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    /// set use of phong shader
    (*shader)["Phong"]->use();
    ///set position of lights
    m_light->setPosition(m_lightPosition1);
    m_light->loadToShader("light1");
    m_light2->loadToShader("light2");
    m_light3->loadToShader("light3");
    m_light4->loadToShader("light4");
    ///draw scene with phong shader
    drawScene("Phong");

    bool menu = m_menu.getMenuBool();
    bool instructions = m_menu.getInstructionBool();
    /// prints how many items the player has picked up on the top right corner of the screen
    if (cureStage > 0 && (!menu) && (!instructions) && (m_menu.getUiOpen() == true))
    {
        m_text1->setColour(1,1,1);
        QString ingCount=QString("%1 / 4").arg(ingredients);
        m_text1->renderText(885,75,ingCount);
    }

    /// prints a little hint on the top of the screen and the timer countdowns for the volcano eruption and plague spreading
    if((!menu) && (!instructions) && (m_menu.getUiOpen() == true))
    {
        m_text2->setColour(1,1,1);
        QString hint=QString("The plague spreads fast in such a small world...");
        m_text2->renderText(320,22,hint);
        m_printPtimer->setColour(1,1,1);
        if (m_timer->Dseconds<10 || m_timer->counterD==0 || m_timer->counterD>120)
        {
            QString plagueTimer=QString("%1 : 0%2").arg(m_timer->minutesCountdown()).arg(m_timer->secondsCountdown());
            m_printPtimer->renderText(922,690,plagueTimer);
        }
        else
        {
            QString plagueTimer=QString("%1 : %2").arg(m_timer->minutesCountdown()).arg(m_timer->secondsCountdown());
            m_printPtimer->renderText(922,690,plagueTimer);
        }
        m_printVtimer->setColour(1,1,1);
        QString volcanoTimer=QString("%1").arg(m_timer->Vseconds);
        m_printVtimer->renderText(809,690,volcanoTimer);
    }

    /// prints in the instructions page how to get back to the menu
    if((instructions))
    {
        m_printInstructions->setColour(1,1,1);
        QString instructions=QString("Press esc to go back to menu");
        m_printInstructions->renderText(360,22,instructions);
    }

    /// prints in the menu page how to navigate around it
    if ((menu))
    {
        m_printMenu->setColour(1,1,1);
        QString menu=QString("Use the arrowkeys to navigate the menu, press enter to select");
        m_printMenu->renderText(320,22,menu);
    }
}

///----------------------------------------------------------------------------------------------------------------------

bool Window::sphereSphereCollision(ngl::Vec3 _pos1, GLfloat _radius1, ngl::Vec3 _pos2, GLfloat _radius2 )
{
    ngl::Vec3 relPos;
    GLfloat dist;
    GLfloat minDist;
    GLfloat len;
    relPos =_pos1-_pos2;
    len=relPos.length();
    dist=len*len;
    minDist =_radius1+_radius2;
    if(dist <=(minDist * minDist))
    {
        return true;
    }
    else
    {
        return false;
    }
}

///----------------------------------------------------------------------------------------------------------------------

void Window::checkEnemyPlayerCollisions()
{
    for(unsigned int i=0;i<m_enemy->enemyForest.size();i++)
    {
    bool collide=sphereSphereCollision(m_sphere->getPosition(),0.1,m_enemy->enemyForest[i].getPosition(),0.1);

        if(collide)
        {
           m_enemy->enemyForest[i].setHit(collide,++m_hit);
           std::cout<<m_hit<<std::endl;
        }
    }
    for(unsigned int i=0;i<m_enemy->enemyGarden.size();i++)
    {
    bool collide=sphereSphereCollision(m_sphere->getPosition(),0.1,m_enemy->enemyGarden[i].getPosition(),0.1);

        if(collide)
        {
           m_enemy->enemyGarden[i].setHit(collide,++m_hit);
           std::cout<<m_hit<<std::endl;
        }

    }
   checkHit();

}

///----------------------------------------------------------------------------------------------------------------------

void Window::checkHit()
{
    //converting how many times the enemies collide with the player into heartcount
    if(m_hit <= 0)
    {
        m_gamePlay.setHeartcount(5);
    }

    else if((0 < m_hit)  && (m_hit <= 6))
    {
        m_gamePlay.setHeartcount(4);
    }

    else if((6 < m_hit)  && (m_hit <= 12))
    {
        m_gamePlay.setHeartcount(3);
    }

    else if((12< m_hit)  && (m_hit <= 18))
    {
        m_gamePlay.setHeartcount(2);
    }

    else if((18 < m_hit)  && (m_hit <= 24))
    {
        m_gamePlay.setHeartcount(1);
    }

    else if (m_hit>=30)
    {
        //set heartcount and gamelost to represent the game being over
        m_gamePlay.setHeartcount(0);
        m_gamePlay.setGameLost(true);
    }

}

///----------------------------------------------------------------------------------------------------------------------

void Window::mouseMoveEvent (QMouseEvent * _event)
{
    //rotation and translation with mouse movements
    if(m_rotate && _event->buttons() == Qt::LeftButton)
    {
        int diffx=_event->x()-m_origX;
        int diffy=_event->y()-m_origY;
        m_spinXFace += (float) 0.5f * diffy;
        m_spinYFace += (float) 0.5f * diffx;
        m_origX = _event->x();
        m_origY = _event->y();
        update();
    }

    else if(m_translate && _event->buttons() == Qt::RightButton)
    {
        int diffX = (int)(_event->x() - m_origXPos);
        int diffY = (int)(_event->y() - m_origYPos);
        m_origXPos=_event->x();
        m_origYPos=_event->y();
        m_modelPos.m_x += INCREMENT * diffX;
        m_modelPos.m_y -= INCREMENT * diffY;
        update();
    }
}

///----------------------------------------------------------------------------------------------------------------------

void Window::mousePressEvent ( QMouseEvent * _event)
{
    if(_event->button() == Qt::LeftButton)
    {
        m_origX = _event->x();
        m_origY = _event->y();
        m_rotate =true;
    }

    else if(_event->button() == Qt::RightButton)
    {
        m_origXPos = _event->x();
        m_origYPos = _event->y();
        m_translate=true;
    }

}

///----------------------------------------------------------------------------------------------------------------------

void Window::mouseReleaseEvent ( QMouseEvent * _event )
{

    if (_event->button() == Qt::LeftButton)
    {
        m_rotate=false;
    }

    if (_event->button() == Qt::RightButton)
    {
        m_translate=false;
    }
}

///----------------------------------------------------------------------------------------------------------------------

void Window::wheelEvent(QWheelEvent *_event)
{
    //zooming with the mouse wheel

    if(_event->delta() > 0)
    {
        m_modelPos.m_z+=ZOOM;
    }
    else if(_event->delta() <0 )
    {
        m_modelPos.m_z-=ZOOM;
    }
    update();
}

//----------------------------------------------------------------------------------------------------------------------

void Window::keyPressEvent(QKeyEvent *_event)
{
    m_keysPressed += (Qt::Key)_event->key();
    //allows us to access the value of the menuOnOff variable, which tells me when the menu is on the screen and off the screen.
    bool m_menuOpen = m_menu.getMenuBool();

    switch (_event->key())
    {
    //escape closes the application
    case Qt::Key_Escape :m_menu.setMenuBool(true);
    m_menu.setGamePlayBool(false);
    m_menu.setInstructionBool(false); break;
    m_menu.setUiOpen(false);
    m_menu.setMenuState(0);
    //w sets to wireframe
    case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
    // S remove wireframe (back to shaded)
    case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;      //Full screen mode when F key pressed
    case Qt::Key_F : showFullScreen(); break;
    //bring back from full screen mode
    case Qt::Key_P : Pause(); break;
    case Qt::Key_R : Resume(); break;
    case Qt::Key_N : Reset(); break;


    default : break;
    }
    update();


    //controls for menu
    if (m_menuOpen == true)
    {
        switch(_event->key())
        {
            case Qt::Key_Up :
            {
                //up arrow key increases menu state, thus you go up the menu
                m_menu.increaseMenuState();
                break;
            }
            case Qt::Key_Down :
            {
                //down arrow key decreases the menu state so you go down the menu
                m_menu.decreaseMenuState();
                break;
            }
            case Qt::Key_Return:
            {
                //pressing return activates the menu state you have chosen, calling the function assigned to that state.
                m_menu.menuStateSwitch();
                if(m_menu.getMenuState() == 3)
                {
                    //if new game is pressed, reset the game
                    Reset();
                }
                break;
            }
            default : break;
        }
    }

    //controls for when you lose the game and are asked to restart, same technique as the menu
    if(m_UI.getExitQuestionFlag() == true)
    {
    switch(_event->key())
    {
        case Qt::Key_Up:
        {
            m_UI.increaseExitState();
            break;
        }
        case Qt::Key_Down :
        {
            m_UI.decreaseExitState();
            break;
        }
        case Qt::Key_Return:
        {
            m_UI.exitStateSwitch();
            if(m_UI.getExitFlag() == true)
            {
                //terminates the game.
                QGuiApplication::exit(EXIT_SUCCESS);
            }

            if(m_UI.getRestartFlag() == true)
            {
            Reset();
            }
            break;
        }
        default : break;
        }
    }
}

///----------------------------------------------------------------------------------------------------------------------

void Window::timerEvent(QTimerEvent *_event)
{


    if (_event->timerId() == m_updateSphereTimer)
    {
        moveSphere();
    }

    if (_event->timerId() == m_redrawTimer)
    {
        update();
    }
    ///Timer event to update the position of light 1 to give it some movement
    if(_event->timerId() == m_lightTimer)
    {
        if (lightMovementAB==true)
        {
            m_lightPosition1.m_x+=0.02;
            m_lightPosition1.m_z+=0.02;
            if (m_lightPosition1.m_x>=7){lightMovementBC= true ; lightMovementAB=false;}
        }
        if (lightMovementBC==true)
        {
            m_lightPosition1.m_y-=0.02;
            if (m_lightPosition1.m_y<=-7){lightMovementCD= true ; lightMovementBC=false;}
        }
        if (lightMovementCD==true)
        {
            m_lightPosition1.m_x-=0.02;
            m_lightPosition1.m_z-=0.02;
            if (m_lightPosition1.m_x<=-7){lightMovementDA= true ; lightMovementCD=false;}
        }
        if (lightMovementDA==true)
        {
            m_lightPosition1.m_y+=0.02;
            if (m_lightPosition1.m_y>=7){lightMovementAB= true ; lightMovementDA=false;}
        }
      update();
    }

    /// timer event to update the particles with gravity
    if(_event->timerId() ==   m_particleTimer)
    {
        m_emitter->update(m_gravity);
    }
}

///----------------------------------------------------------------------------------------------------------------------

void Window::BoundingBoxCollisions()
{
    for(unsigned int i = 0; i < objectsVector.size(); ++i)
    {
        /// Access the extents of the bounding boxes
        BBExtent objExtents = objectsVector[i].getExtents();

        /// Determine whether the position of the sphere plus the radius is within the extents of the bounding boxes
        if((m_sphere->getPosition().m_x + 0.2)>= objExtents.minX && (m_sphere->getPosition().m_x - 0.2)<= objExtents.maxX &&
           (m_sphere->getPosition().m_y + 0.2)>= objExtents.minY && (m_sphere->getPosition().m_y - 0.2)<= objExtents.maxY &&
           (m_sphere->getPosition().m_z + 0.2)>= objExtents.minZ && (m_sphere->getPosition().m_z - 0.2)<= objExtents.maxZ)
        {
            /// objects interactions: depending on what the player is hitting and at what point of the game it is,
            /// the player can interact with certain objects (speak to doctor, pick up ingredients to cure the plague)
            if (i==6)       //if colliding with the doctor
            {
                //start timer for the dialogue to draw
                m_UI.setTriggerTimer(true);
                if (cureStage == 0)     //if quest not started yet
                {
                    m_sphere->setHit();
                    cureStage=1;     //start quest
                }
                else if (cureStage == 2)    //player hasn't picked up all the ingredients for the plague cure
                {
                    m_sphere->setHit();
                }
                else if (cureStage == 3)    //player has picked up all the ingredients
                {
                    m_sphere->setHit();
                    m_gamePlay.setGameWon(true);
                }
            }

            else if (i>=25 && i<=28)       //if colliding with one of the lavender plants
            {
                if (cureStage>0 && lavender==false)
                {
                    lavender=true;
                    objectsVector[i].pickup();
                    ingredients += 1;
                    if (ingredients == 4)   //if all the ingredients have been picked up
                    {
                        cureStage=3;
                    }
                    else
                    {
                        cureStage=2;
                    }
                }
                m_sphere->setHit();
            }

            else if (i==29 || i==30)       //if colliding with the roses
            {
                if (cureStage>0 && roses==false)
                {
                    roses=true;
                    objectsVector[i].pickup();
                    ingredients += 1;
                    if (ingredients == 4)   //if all the ingredients have been picked up
                    {
                        cureStage=3;

                    }
                    else
                    {
                        cureStage=2;
                    }
                }
                m_sphere->setHit();
            }

            else if (i==8)       //if colliding with the mercury
            {
                if (cureStage>0 && mercury==false)
                {
                    mercury=true;
                    objectsVector[i].pickup();
                    ingredients += 1;
                    if (ingredients == 4)   //if all the ingredients have been picked up
                    {
                         cureStage=3;
                    }
                    else
                    {
                        cureStage=2;
                    }
                }
                m_sphere->setHit();
            }

            else if (i==21)       //if colliding with the cloves
            {
                if (cureStage>0 && cloves==false)
                {
                    cloves=true;
                    objectsVector[i].pickup();
                    ingredients += 1;
                    if (ingredients == 4)   //if all the ingredients have been picked up
                    {
                         cureStage=3;
                    }
                    else
                    {
                        cureStage=2;
                    }
                }
                m_sphere->setHit();
            }

            /// There is a collision
            else
            {
                m_sphere->setHit();
            }
            break;

        }

        else
        {
            /// There is not a collision
            m_sphere->setNotHit();

            ///turns off the timer for speaking to the plague doctor once its ran out of time, so icon undraws
            if(m_UI.getUiTimer() == 0 )
            {
                 m_UI.setTriggerTimer(false);
                 m_UI.timerReset();
            }
        }
    }
}

///----------------------------------------------------------------------------------------------------------------------
/// function that updates the sphere position based on keys pressed
///----------------------------------------------------------------------------------------------------------------------
void Window::moveSphere()
{
    float xDirection=0.0; // x direction for the player movmement
    float zDirection=0.0; // z direction for the player movmement

    BoundingBoxCollisions();;
    //until pause works, this will stop you moving the sphere as you navigate menu

    bool m_gamePlay = m_menu.getGamePlayBool();
    if (m_gamePlay == true)
    {

        foreach(Qt::Key key, m_keysPressed)
        {
            switch (key)
            {
            //Setting the keys to the sphere controls
                case Qt::Key_Left :  { xDirection=-s_sphereUpdate; break;}
                case Qt::Key_Right : { xDirection=s_sphereUpdate; break;}
                case Qt::Key_Up :    { zDirection=-s_sphereUpdate; break;}
                case Qt::Key_Down :  { zDirection=s_sphereUpdate; break;}
                default : break;
            }
        }

        if(m_keysPressed.size() !=0 && playerIsActive)
        {
            if (m_sphere->rotateWorld==true)
            {
                s_sphereUpdate=0; //disables the player if a rotation is occurring

            }
            /// if the sphere is not colliding it moves as normal
            checkEnemyPlayerCollisions();
            if(m_sphere->isHit()==false)
            {
            m_sphere->move(xDirection,zDirection);
            }
            else
            {
            /// If it is colliding return the sphere to its previous position and then continue moving normally
            m_sphere->sendBack();
            m_sphere->move(xDirection,zDirection);
        }
    }
}

}

///----------------------------------------------------------------------------------------------------------------------
void Window::bumpEnemy()
{  
      for (unsigned int i=0;i<5;i++)
    {
        //create enemy bump for each enemy at init positions
        m_sphereBump.reset(new SphereBump(m_enemy->enemyForest[i].getPosition()));
        m_sphereBump.reset(new SphereBump(m_enemy->enemyGarden[i].getPosition()));
     }
}
///----------------------------------------------------------------------------------------------------------------------
void Window::setEnemyRotForest()
{
    ///apply new rotation matrix to enemy positions

    for (unsigned int i =0;i<m_enemy->enemyForest.size();i++){
        ngl::Mat4 newEnemyMat = PivotRotation(m_enemy->enemyForest[i].getPosition(),m_enemyGlobalRotation, ngl::Vec3(0,0,0),ngl::Vec3(1,1,1));

        newEnemyRotPos.m_x = newEnemyMat.m_30;
        newEnemyRotPos.m_y = newEnemyMat.m_31;
        newEnemyRotPos.m_z = newEnemyMat.m_32;
        m_transform.setPosition(newEnemyRotPos);
        loadMatricesToShader();
        m_enemy->enemyForest[i].draw();
    }

}

///----------------------------------------------------------------------------------------------------------------------
void Window::setEnemyRotGarden()
{//rotate for both enemy vectors, does not draw if not on same plane?
    for (unsigned int i =0;i<m_enemy->enemyGarden.size();i++){
        ngl::Mat4 newEnemyMat = PivotRotation(m_enemy->enemyGarden[i].getPosition(),m_enemyGlobalRotation2, ngl::Vec3(-90,0,0),ngl::Vec3(1,1,1));

        newEnemyRotPos2.m_x = newEnemyMat.m_30;
        newEnemyRotPos2.m_y = newEnemyMat.m_31;
        newEnemyRotPos2.m_z = newEnemyMat.m_32;
        m_transform.setPosition(newEnemyRotPos2);
        loadMatricesToShader();
        m_enemy->enemyGarden[i].draw();
    }

}
///----------------------------------------------------------------------------------------------------------------------

void Window::keyReleaseEvent(QKeyEvent *_event)
{
    m_keysPressed -= (Qt::Key)_event->key();
}

///----------------------------------------------------------------------------------------------------------------------
///called when 'P' pressed
///pauses the game
///----------------------------------------------------------------------------------------------------------------------
void Window::Pause()
{
    s_sphereUpdate=0;
    pauseEnemy=true;
    m_timer->setActive(false);
}
///----------------------------------------------------------------------------------------------------------------------
///called when 'R' pressed
///resumes game
///----------------------------------------------------------------------------------------------------------------------
void Window::Resume()
{
    s_sphereUpdate=0.04;
    pauseEnemy=false;
    m_timer->setActive(true);

}
///----------------------------------------------------------------------------------------------------------------------
///called when 'N' pressed
///resets game
///----------------------------------------------------------------------------------------------------------------------
void Window::Reset()
{
    m_sphere->setPosition(ngl::Vec3(0,5.5,0));
    ngl::Mat4 normal;
    m_rotationCurrent=normal;
    m_enemyGlobalRotation=normal;
    m_enemyGlobalRotation2=normal;
    m_hit=0;
    playerIsActive=true;

//-------enemy reset-----------------------------------
    m_isAttacking=false;
    m_isAttacking2=false;
    m_enemy->isReset=false; //not reset matrix yet
    m_enemy->reset();
    m_sphere->isForest=true;
    m_sphere->isGarden=false;
    bumpEnemy();
//------------------------------------------------------

    //reset the gameloop
    m_menu.setGamePlayBool(true);
    m_menu.setMenuBool(false);
    m_menu.setInstructionBool(false);
    m_menu.setMenuState(0);
    m_menu.setUiOpen(true);
    m_gamePlay.setGameErupted(false);
    m_gamePlay.setGameWon(false);
    m_gamePlay.setGameLost(false);
    m_UI.setTriggerTimer(false);
    m_UI.setRestartFlag(false);
    m_UI.setExitQuestionFlag(false);
    m_UI.setExitFlag(false);

    // reset the volcano - timer
    m_timer->reset();
    m_isSafeFromEruption = false;
    m_safeFromEruptionUiTimer = 0;


    // reset  health
    m_gamePlay.setHeartcount(5);
    m_hit = 0;

    // reset all inventory to initial state
    cloves = false;
    lavender = false;
    mercury = false;
    roses = false;
    cureStage = 0;
    ingredients = 0;

    // reset the items' scales and redraw bounding boxes
    for (unsigned int i=0; i < objectsVector.size(); ++i)
    {
        objectsVector[i].replace();
    }

}
///----------------------------------------------------------------------------------------------------------------------
