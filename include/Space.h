#ifndef STARMAP_H__
#define STARMAP_H__

#include <ngl/Camera.h>
#include <string>

class Space
{
  public :
    Space(std::string _texture, ngl::Camera *_cam);
    ~Space();
    void draw();
  private :
    GLuint m_texID;
    ngl::Camera *m_camera;
};

#endif
