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

#include "ObjModel.h"
#include "PlyModel.h"

using namespace std;

/************************************************************************/
/* Canvas to draw                                                       */
/************************************************************************/
class CCanvas : public QGLWidget {
    Q_OBJECT

  public:
    explicit CCanvas(QWidget *parent = 0) : QGLWidget(parent),
                                            textureTrain("./models/train/train.jpg"),
                                            eagleModel("./models/bird/eagle_meshlab.obj") {
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
        timer->start(10);
        tau = 0.0f;
    }

  protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

  private:
    void lookAt(const GLdouble eyex,
                const GLdouble eyey,
                const GLdouble eyez,
                const GLdouble centerx,
                const GLdouble centery,
                const GLdouble centerz,
                const GLdouble upx,
                const GLdouble upy,
                const GLdouble upz);

    void glPerspective(const GLdouble fovy,
                       const GLdouble aspect,
                       const GLdouble zNear,
                       const GLdouble zFar);

    enum View {
        Perspective = 0, // View the scene from a perspective (from above, from a side, or whatever)
        Cockpit          // View the scene from the train cockpit (if you want, or whatever other view)
    };

    void setView(View _view);

    // Models and textures
    Texture textureTrain;
    // Model loaded from .obj format
    ObjModel eagleModel;
    GLfloat tau;

    // NO
    // Model loaded from .ply format
    //    PlyModel eagleModel;
};

#endif
