#include "Camera.h"
#include <iostream>

void Camera::setView(View _view, Bird bird) {
    switch (_view) {
        case Side: {
            lookAt(defaultPosition.x(), defaultPosition.y(), defaultPosition.z(),
                   defaultDirection.x(), defaultDirection.y(), defaultDirection.z(),
                   0, 1, 0);
            break;
        }
        case Eyes: {
            // view from bird's eyes
            Point3d bird_position = bird.getPosition();
            Point3d head_position = bird_position + 0.25f * bird.getDirection();
            Point3d bird_line_of_sight = bird_position + 1.0f * bird.getDirection();
            lookAt(head_position.x(), head_position.y() + 0.15f, head_position.z(),
                   bird_line_of_sight.x(), bird_line_of_sight.y() - 0.1f, bird_line_of_sight.z(),
                   0.0f, 1.0f, 0.0f);
            break;
        }
        case Follow: {
            Point3d bird_position = bird.getPosition();
            Point3d camera_above(-25, 10, -25);
            lookAt(camera_above.x(), camera_above.y(), camera_above.z(),
                   bird_position.x(), bird_position.y(), bird_position.z(),
                   0, 1, 0);
            break;
        }
        case Manual: {
            Point3d line_of_sight = manualPosition + manualDirection;
            lookAt(manualPosition.x(), manualPosition.y(), manualPosition.z(),
                   line_of_sight.x(), line_of_sight.y(), line_of_sight.z(),
                   0, 1, 0);
            break;
        }
    }
}

void Camera::keyPressEvent(QKeyEvent *event, View _view) {
    if (_view != Camera::Manual) {
        return;
    }
    Point3d forward(manualDirection.x(), 0, manualDirection.z());
    Point3d rightward(manualDirection.z(), 0, -manualDirection.x());
    Point3d upperward(0, 1, 0);

    switch (event->key()) {
        case Qt::Key_W:
            manualPosition = manualPosition + forward;
            break;
        case Qt::Key_A:
            manualPosition = manualPosition + rightward;
            break;
        case Qt::Key_S:
            manualPosition = manualPosition - forward;
            break;
        case Qt::Key_D:
            manualPosition = manualPosition - rightward;
            break;
        case Qt::Key_Space:
            manualPosition = manualPosition + upperward;
            break;
        case Qt::Key_Z:
            manualPosition = manualPosition - upperward;
            break;
        case Qt::Key_J:
            manualAngleHorizontal -= manualSpeed;
            manualDirection.x() = cos(manualAngleHorizontal);
            manualDirection.z() = sin(manualAngleHorizontal);
            break;
        case Qt::Key_L:
            manualAngleHorizontal += manualSpeed;
            manualDirection.x() = cos(manualAngleHorizontal);
            manualDirection.z() = sin(manualAngleHorizontal);
            break;
        case Qt::Key_I:
            if (manualAngleVertical <= 1) {
                manualAngleVertical += manualSpeed;
                manualDirection.y() = manualAngleVertical;
            }
            break;
        case Qt::Key_K:
            if (manualAngleVertical >= -1) {
                manualAngleVertical -= manualSpeed;
                manualDirection.y() = manualAngleVertical;
            }
            break;
    }
}

Camera::View Camera::getNextView(View _view) {
    if (_view == Side) {
        std::cout << "View: Eyes" << std::endl;
        return Eyes;
    }
    if (_view == Eyes) {
        std::cout << "View: Flollow" << std::endl;
        return Follow;
    }
    if (_view == Follow) {
        std::cout << "View: Manual" << std::endl;
        return Manual;
    }
    if (_view == Manual) {
        std::cout << "View: Side" << std::endl;
        return Side;
    }
}

void Camera::lookAt(const GLdouble eyeX,
                    const GLdouble eyeY,
                    const GLdouble eyeZ,
                    const GLdouble centerX,
                    const GLdouble centerY,
                    const GLdouble centerZ,
                    const GLdouble upX,
                    const GLdouble upY,
                    const GLdouble upZ) {
    Point3d VP(eyeX, eyeY, eyeZ);
    Point3d q(centerX, centerY, centerZ);
    Point3d VUP(upX, upY, upZ);
    Point3d VPN = VP - q;

    GLdouble *mat = new GLdouble[16]; // remember: column-major order!

    // set up the LookAt matrix correctly!
    Point3d p_prime = VP;
    Point3d z_prime = VPN.normalized();
    Point3d x_prime = (VUP ^ z_prime).normalized();
    Point3d y_prime = z_prime ^ x_prime;

    // column 1
    mat[0] = x_prime[0];
    mat[1] = y_prime[0];
    mat[2] = z_prime[0];
    mat[3] = 0;

    // column 2
    mat[4] = x_prime[1];
    mat[5] = y_prime[1];
    mat[6] = z_prime[1];
    mat[7] = 0;

    // column 3
    mat[8] = x_prime[2];
    mat[9] = y_prime[2];
    mat[10] = z_prime[2];
    mat[11] = 0;

    // sure? column 4
    mat[12] = -x_prime * p_prime;
    mat[13] = -y_prime * p_prime;
    mat[14] = -z_prime * p_prime;
    mat[15] = 1;

    glMultMatrixd(mat);

    delete[] mat;
}
