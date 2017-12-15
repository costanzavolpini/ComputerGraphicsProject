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
#include "Scene.h"
#include "Sky.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


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

    static constexpr GLfloat sunSpeed = 5.0f;
    static constexpr GLfloat tauIncrement = 0.01f;

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
        Side = 0,        // View the scene from a side perspective
        Eyes,            // View the scene from the Bird's eyes
        Above            // View the scene from an above perspective
    };

    void setView(View _view);


    /*
     * Scene objects. The Bird object contains all parts of the bird, and supports animation of
     * the individual pieces; the Scene object contains the environment specification (the
     * mountains, with trees and such, plus maybe a sun and clouds. Maybe.)
     */

    // Bird object
    Bird bird;

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
