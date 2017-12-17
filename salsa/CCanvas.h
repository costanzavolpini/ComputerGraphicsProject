/************************************************************************/
/* Guards                                                               */
/************************************************************************/
#ifndef CCANVAS_H
#define CCANVAS_H

#include <QGLWidget>
#include <QTimer>
#include <QtOpenGL>
#include <iostream>

#include "Base.h"
#include "texture.hpp"

using namespace std;
//#include "ObjModel.h"
#include "Bird.h"
#include "Camera.h"
#include "Scene.h"
#include "Sky.h"


/************************************************************************/
/* Canvas to draw                                                       */
/************************************************************************/
class CCanvas : public QGLWidget {
    Q_OBJECT

  public:
    explicit CCanvas(QWidget *parent = 0) : QGLWidget(parent),
                                            bird(),
                                            scene(),
                                            sky() {

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
        timer->start(10);
        tau = 0.0f;
        sunPosition = Point3d(2.8, 5.5, -10);
    }

    static constexpr GLfloat sunSpeed = 50.0f;
    static constexpr GLfloat tauIncrement = 0.01f;

  protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    virtual void keyPressEvent(QKeyEvent *);

  private:
    void glPerspective(const GLdouble fovy,
                       const GLdouble aspect,
                       const GLdouble zNear,
                       const GLdouble zFar);
    Camera camera;

    void generateShadowMap();
    void viewAndProjectMatrixShadow();

    /*
     * Scene objects. The Bird object contains all parts of the bird, and supports animation of
     * the individual pieces; the Scene object contains the environment specification (the
     * mountains, with trees and such, plus maybe a sun and clouds. Maybe.)
     */

    // Bird object
    Bird bird;

    Camera::View view = Camera::View::Eyes;

    // Scene object
    Scene scene;

    //Sky object
    Sky sky;

    Point3d sunPosition;

    /*
     * For rotation (testing)
     */
    GLfloat tau;
};

#endif
