#include "Space.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/Texture.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>

Space::Space(std::string _texture, ngl::Camera *_cam)
{

  ngl::Texture t(_texture);
  m_texID=t.setTextureGL();


  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("space",500,50);
  m_camera=_cam;
}

void Space::draw()
{
  static float rot=0.0;

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

  glEnable (GL_BLEND);

  (*shader)["Texture"]->use();

  ngl::Transformation t;
  t.setRotation(0,rot,0);
  rot+=0.02;
  ngl::Mat4 MVP;

  MVP=t.getMatrix()*m_camera->getVPMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);
  glBindTexture(GL_TEXTURE_2D,m_texID);
  prim->draw("space");

  glDisable(GL_BLEND);

}
