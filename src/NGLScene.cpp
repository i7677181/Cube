#include <QMouseEvent>
#include <QGuiApplication>
#include <QApplication>

#include "NGLScene.h"
//#include "Windows.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/NGLStream.h>
#include <iostream>
#include <enemy.h>

const static float INCREMENT=0.01;
const static float ZOOM=0.1;
constexpr static float s_sphereUpdate=0.05;
bool enemy=false;

//----------------------------------------------------------------------------------------------------------------------

Window::Window()
{
  m_rotate=false;
  m_spinXFace=0;
  m_spinYFace=0;
  setTitle("Oh no! Disaster!");

  m_updateSphereTimer=startTimer(2);
  m_redrawTimer=startTimer(4);
  m_keysPressed.clear();

}

//----------------------------------------------------------------------------------------------------------------------

Window::~Window()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

//----------------------------------------------------------------------------------------------------------------------

void Window::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();
  m_cam.setShape(45.0f,(float)width()/height(),0.05f,350.0f);
}

//----------------------------------------------------------------------------------------------------------------------

void Window::resizeGL(int _w , int _h)
{
  m_cam.setShape(45.0f,(float)_w/_h,0.05f,350.0f);
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
}

//----------------------------------------------------------------------------------------------------------------------
void Window::Cube()
{
      // create a vao as a series of GL_TRIANGLES
      m_vao.reset( ngl::VertexArrayObject::createVOA(GL_TRIANGLES));
      m_vao->bind();


     const static GLubyte indices[]=  {
                                          0,1,5,0,4,5, // back
                                          3,2,6,7,6,3, // front
                                          0,1,2,3,2,0, // top
                                          4,5,6,7,6,4, // bottom
                                          0,3,4,4,7,3,
                                          1,5,2,2,6,5
                                       }; //shared vertices

   GLfloat vertices[] = {-1,1,-1,
                             1,1,-1,
                             1,1,1,
                             -1,1,1,
                             -1,-1,-1,
                             1,-1,-1,
                             1,-1,1,
                             -1,-1,1
                            };

       GLfloat colours[]={
                            1,0,0,
                            0,1,0,
                            0,0,1,
                            1,1,1,
                            0,0,1,
                            0,1,0,
                            1,0,0,
                            1,1,1
                          }; //uvs


       // in this case we are going to set our data as the vertices above
       m_vao->setIndexedData(24*sizeof(GLfloat),vertices[0],sizeof(indices),&indices[0],GL_UNSIGNED_BYTE,GL_STATIC_DRAW);
       // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
       m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
       m_vao->setIndexedData(24*sizeof(GLfloat),colours[0],sizeof(indices),&indices[0],GL_UNSIGNED_BYTE,GL_STATIC_DRAW);
       // now we set the attribute pointer to be 0 (as this matches vertIn in our shader)
       m_vao->setVertexAttributePointer(3,3,GL_FLOAT,0,0); //bind verts to shader
       m_vao->setNumIndices(sizeof(indices));
     // now unbind
      m_vao->unbind();

 }

void Window::initializeGL()
{

  ngl::NGLInit::instance();
  glClearColor(0.8f, 0.9f, 1.0f, 1.0f);//Baby blue background-like the sky!
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  shader->createShaderProgram("Phong");

  shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);

  shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");

  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");

  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");

  shader->bindAttribute("Phong",0,"inVert");
  shader->bindAttribute("Phong",1,"inUV");
  shader->bindAttribute("Phong",2,"inNormal");

  shader->linkProgramObject("Phong");

  (*shader)["Phong"]->use();
    shader->setShaderParam1i("Normalize",1);

  ngl::Vec3 from(0,10,30);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);

  //setting the camera
  m_cam.set(from,to,up);
  m_cam.setShape(45,(float)720.0/576.0,0.05,350);
  shader->setShaderParam3f("viewerPos",m_cam.getEye().m_x,m_cam.getEye().m_y,m_cam.getEye().m_z);

  m_lightAngle=0.0;
  m_light.reset( new ngl::Light(ngl::Vec3(sin(m_lightAngle),200,cos(m_lightAngle)),ngl::Colour(0,1,0.3,1),ngl::Colour(1,1,1,1),ngl::LightModes::POINTLIGHT));//Cube is green, light is white.

  ngl::Mat4 iv=m_cam.getViewMatrix();
  iv.transpose();
  m_light->setTransform(iv);
  m_light->loadToShader("light");

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

//create player sphere
prim->createSphere("sphere",0.5,50);
m_sphere.reset(new Sphere(ngl::Vec3(0,5.5,0)));
m_transform.setPosition(m_sphere->getPosition());

//-------------------------enemy
//  create and
ngl::VAOPrimitives *enemy_prim=ngl::VAOPrimitives::instance();
enemy_prim->createSphere("Enemy",0.9,50);
//init enemy with default position
m_enemy.reset(new Enemy());

//create enemy bump,keep this line otherwise it crashes on collision
m_sphereBump.reset(new SphereBump(m_enemy->getPosition()));
//create BBOX what's this for?
//m_bbox.reset( new ngl::BBox(ngl::Vec3(),1.0f,1.0f,1.0f));
//m_bbox->setDrawMode(GL_LINE);

    Cube();
}

//----------------------------------------------------------------------------------------------------------------------

void Window::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat3 normalMatrix;
  ngl::Mat4 M=m_transform.getMatrix()*m_mouseGlobalTX;
  ngl::Mat4 MV=  M*m_cam.getViewMatrix();
  ngl::Mat4 MVP=  MV*m_cam.getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);

}

//----------------------------------------------------------------------------------------------------------------------

void Window::drawScene(const std::string &_shader)
{

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shader]->use();
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  m_mouseGlobalTX=rotY*rotX;
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

//draw sphere at current pos;
  m_transform.reset();
  {
    m_transform.setPosition(m_sphere->getPosition());
   loadMatricesToShader();
   shader->setUniform("Colour", 0.5f,0.0f, 0.0f);
    m_sphere->draw();

  }

m_transform.reset();
{
m_transform.setPosition(m_enemy->getPosition()); //set to 5,5,5
loadMatricesToShader();
m_enemy->draw();


    if(sphereSphereCollision(m_sphere->getPosition(),2.5, m_enemy->getPosition(),2.5) == true) //trigger area
    enemy=true; //trigger flag
    if(enemy)
    {
        m_transform.setPosition(m_enemy->getPosition());//1?

        m_enemy->setPrey(m_sphere->getPosition());
        moveEnemy();
        std::cout<<"enemy triggered\n";

    }
}

  // get width and height of plane edge here;

  {
      float height=5;
      float width=5;

     m_sphere->planeEdge(width,height);
     m_transform.reset();
     {
     //Rotation of the cube
     //calling the rotation from the sphere class
     m_transform.setRotation(m_sphere->setCubeRotation());
     //scaling of cube
     m_transform.setScale(width,height,width);

     loadMatricesToShader();

     shader->setUniform("Colour", 0.0f,0.5f, 0.0f);

     m_vao->bind();
     m_vao->draw();
     m_vao->unbind();


     }
  }
}

//----------------------------------------------------------------------------------------------------------------------
void Window::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,m_width,m_height);
    drawScene("Phong");


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);
  drawScene("Phong");


}
//----------------------------------------------------------------------------------------------------------------------

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
//----------------------------------------------------------------------------------------------------------------------

void Window::checkCollisions()
{
    bool collide=sphereSphereCollision(m_sphere->getPosition(),0.5,m_enemy->getPosition(),0.8);
    m_enemy->setHit(collide);
}
//----------------------------------------------------------------------------------------------------------------------
void Window::triggerEnemy()
{
    //if player in enemy attack radius,aggro
    bool attackable=sphereSphereCollision(m_sphere->getPosition(),2.5,m_enemy->getPosition(),2.5);
}
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

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

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

  switch (_event->key())
  {
  //escape closes the application
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  //w sets to wireframe
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
      // S remove wireframe (back to shaded)
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
      //Full screen mode when F key pressed
  case Qt::Key_F : showFullScreen(); break;
      //bring back from full screen mode
  case Qt::Key_N : showNormal(); break;

  default : break;
  }

    update();

}

//----------------------------------------------------------------------------------------------------------------------

void Window::timerEvent(QTimerEvent *_event)
{
    //setting the timer

    if (_event->timerId() == m_updateSphereTimer)
    {
        moveSphere();
    }

    if (_event->timerId() == m_redrawTimer)
    {
        update();
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Window::moveSphere()
{
    float xDirection=0.0;
    float zDirection=0.0;
    float rotation=0.0;

    foreach(Qt::Key key, m_keysPressed)
    {
        switch (key)
        {
        //Setting the keys to the sphere controls
            case Qt::Key_Left :  { xDirection=-s_sphereUpdate; break;}
            case Qt::Key_Right : { xDirection=s_sphereUpdate; break;}
            case Qt::Key_Up :    { zDirection=-s_sphereUpdate; break;}
            case Qt::Key_Down :  { zDirection=s_sphereUpdate; break;}
            case Qt::Key_R :     { rotation=1.0; break;}
            default : break;
        }
    }

    if(m_keysPressed.size() !=0)
    {
       checkCollisions();

        //feeding in the key movements into the positions of the sphere
        if(sphereSphereCollision(m_sphere->getPosition(),0.5, m_sphereBump->getPosition(),1.0f) == false)           
        {
            m_sphere->move(xDirection,zDirection);
            m_sphere->rotate(rotation);
        }
        else
       {
            m_sphere->move((xDirection-s_sphereUpdate), (zDirection-s_sphereUpdate));
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Window::moveEnemy()
{
    //pass position of player to enemy class
 m_enemy->move(m_sphere->getPosition()); //0.75f

}
void Window::keyReleaseEvent(QKeyEvent *_event)
{
    m_keysPressed -= (Qt::Key)_event->key();
}

void Window::updateLight()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  (*shader)["Phong"]->use();
  m_lightAngle+=0.1;

  m_light->setPosition(ngl::Vec3(4.0*cos(m_lightAngle),2,4.0*sin(m_lightAngle)));
  m_light->loadToShader("light");
}
