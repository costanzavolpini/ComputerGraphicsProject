#ifndef CAMERA_H
#define CAMERA_H

#include "Base.h"
#include "Bird.h"
#include <QtOpenGL>

class Camera {

  public:
    explicit Camera() {
        defaultPosition = Point3d(0, 0, 0);
        defaultDirection = Point3d(0, 0, -1);

        manualPosition = Point3d(defaultPosition.x(), defaultPosition.y(), defaultPosition.z());
        manualDirection = Point3d(defaultDirection.x(), defaultDirection.y(), defaultDirection.z());
        manualAngleHorizontal = atan2(defaultDirection.z(), defaultDirection.x());
        manualAngleVertical = defaultDirection.y();
        manualSpeed = 0.02;
    }

    enum View {
        Side = 0, // View the scene from a side perspective
        Eyes,     // View the scene from the Bird's eyes
        Above,    // View the scene from an above perspective
        Manual
    };

    void setView(View _view, Bird bird);
    void keyPressEvent(QKeyEvent *event, View _view);
    Camera::View getNextView(View _view);

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

    void getNextView();

    Point3d defaultPosition;
    Point3d defaultDirection;
    Point3d manualPosition;
    Point3d manualDirection;
    double manualAngleHorizontal;
    double manualAngleVertical;
    double manualSpeed;
};

#endif // CAMERA_H
