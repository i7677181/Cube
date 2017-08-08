#include "Space.h"
#include <ngl/VAOPrimitives.h>
#include <ngl/Texture.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>


Space::Space(std::string _texture, ngl::Camera *_cam)
{
    /// loading in our texture
    ngl::Texture t(_texture);

    /// set Id for our texture
    m_texID=t.setTextureGL();

    /// instance for VAO primitives
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

    /// primitives to create a sphere to encase our game to project the background
    prim->createSphere("space",500,50);

    /// variable for our camera
    m_camera=_cam;
}

void Space::draw()
{
    /// float to set a rotation of background
    static float rot=0.0;

    /// instance of the shader library
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    /// instance of VAO primitives
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

    /// enable blend
    glEnable (GL_BLEND);

    /// set use of texture shader
    (*shader)["Texture"]->use();

    /// initialise transformation
    ngl::Transformation t;

    /// set xyz rotation
    t.setRotation(0,rot,0);

    /// set y rotation
    rot+=0.02;

    ngl::Mat4 MVP;
    MVP=t.getMatrix()*m_camera->getVPMatrix();
    /// set shader paramter
    shader->setShaderParamFromMat4("MVP",MVP);

    /// bind texture
    glBindTexture(GL_TEXTURE_2D,m_texID);

    /// draw primitive
    prim->draw("space");

    ///disable blend
    glDisable(GL_BLEND);

}
