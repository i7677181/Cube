#ifndef WINDOW_H_
#define WINDOW_H_
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/VertexArrayObject.h>
#include <ngl/Text.h>
#include <QOpenGLWindow>
#include <memory>
#include <QSet>
#include "Sphere.h"
#include "SphereBump.h"
#include <enemy.h>

class Window : public QOpenGLWindow
{
  public:

    Window();
    ~Window();
    void initializeGL();
    void paintGL();
    // Qt 5.5.1 must have this implemented and uses it
    void resizeGL(QResizeEvent *_event);
    // Qt 5.x uses this instead! http://doc.qt.io/qt-5/qopenglwindow.html#resizeGL
    void resizeGL(int _w, int _h);

private:

    int m_spinXFace;
    int m_spinYFace;
    bool m_rotate;
    bool m_translate;
    int m_origX;
    int m_origY;
    int m_origXPos;
    int m_origYPos;
    int m_width;
    int m_height;
    ngl::Mat4 m_mouseGlobalTX;
    ngl::Camera m_cam;
    ngl::Transformation m_transform;
    ngl::Vec3 m_modelPos;
    ngl::Real m_lightAngle;


    void loadMatricesToShader();
    void keyPressEvent(QKeyEvent *_event);
    void mouseMoveEvent (QMouseEvent * _event );
    void mousePressEvent ( QMouseEvent *_event);
    void mouseReleaseEvent ( QMouseEvent *_event );
    void wheelEvent( QWheelEvent *_event);
    void keyReleaseEvent(QKeyEvent *_event);
    QSet<Qt::Key> m_keysPressed;
    void drawScene(const std::string &_shader);
    void timerEvent (QTimerEvent *event);
    int m_updateSphereTimer;
    int m_redrawTimer;
    void moveSphere();
    void stopSphere();
    std::unique_ptr<Sphere> m_sphere;
    std::unique_ptr<ngl::Light> m_light;

    void moveEnemy();
    std::unique_ptr<SphereBump> m_sphereBump;
    void Cube();
    void updateLight();
    std::unique_ptr<ngl::VertexArrayObject> m_vao;
    std::unique_ptr<Enemy> m_enemy;
    std::unique_ptr<ngl::BBox> m_bbox;
    void checkCollisions();
    void triggerEnemy();
    bool sphereSphereCollision( ngl::Vec3 _pos1, GLfloat _radius1, ngl::Vec3 _pos2, GLfloat _radius2 );
};

#endif
