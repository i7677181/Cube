#include "SphereBump.h"
#include "NGLScene.h"
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/NGLStream.h>


SphereBump::SphereBump(ngl::Vec3 _pos)

{
m_pos=_pos;
}

void SphereBump::draw()
{
 ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    if(m_hit)
        shader->setUniform("Colour",1.0f,0.0f,0.0f);

    else
        shader->setUniform("Colour",0.0f,0.0f,1.0f);
        ngl::VAOPrimitives::instance()->draw("sphereBump");

}
