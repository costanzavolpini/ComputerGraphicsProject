#include "CCanvas.h"
#include "Base.h"
#include "Sphere.h"

using namespace std;

//-----------------------------------------------------------------------------

void CCanvas::initializeGL() {
    glClearColor(0.8117647059f, 0.8470588235f, 0.862745098f, 0.5f); // black background
    glClearDepth(1.0f);                                             // depth buffer setup
    glEnable(GL_DEPTH_TEST);                                        // enables depth testing
    glDepthFunc(GL_LEQUAL);                                         // the type of depth testing to do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);              // really nice perspective calculations
    glShadeModel(GL_SMOOTH);

    // One light source
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    /*
     * The position is transformed by the modelview matrix when glLightfv is called (just as if it were
     * a point), and it is stored in eye coordinates. If the w component of the position is 0.0,
     * the light is treated as a directional source. Diffuse and specular lighting calculations take
     * the light's direction, but not its actual position, into account, and attenuation is disabled.
     * Otherwise, diffuse and specular lighting calculations are based on the actual location of the
     * light in eye coordinates, and attenuation is enabled. The default position is (0,0,1,0); thus,
     * the default light source is directional, parallel to, and in the direction of the -z axis.
     */
    GLfloat lightpos[] = {(float)sunPosition.x(), (float)sunPosition.y(), (float)sunPosition.z(), 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    //    GLfloat lightAmb[] = {0.3, 0.3, 0.3};
    GLfloat lightAmb[] = {1.0, 1.0, 1.0};  //TODO remove
                                           //    GLfloat lightDiff[] = {0.4, 0.4, 0.4};
    GLfloat lightDiff[] = {1.0, 1.0, 1.0}; //TODO remove
    GLfloat lightSpec[] = {1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);

    /*
     * Before you can use the texture you need to initialize it by calling the setTexture() method.
     * Before you can use OBJ/PLY model, you need to initialize it by calling init() method.
     */
    bird.init();
    bird.setAnimate(true);
    bird.setMove(true);
    scene.init();
    sky.init();

    // Example for debugging
    //    example.init();
}

//-----------------------------------------------------------------------------

void CCanvas::glPerspective(const GLdouble fovy, const GLdouble aspect, const GLdouble zNear, const GLdouble zFar) {
    const GLdouble d = 1.0 / tan(fovy / 360.0 * PI);
    const GLdouble delta = zNear - zFar;

    GLdouble *mat = new GLdouble[16];

    mat[0] = d / aspect;
    mat[1] = 0.0;
    mat[2] = 0.0;
    mat[3] = 0.0;

    mat[4] = 0.0;
    mat[5] = d;
    mat[6] = 0.0;
    mat[7] = 0.0;

    mat[8] = 0.0;
    mat[9] = 0.0;
    mat[10] = (zNear + zFar) / delta;
    mat[11] = -1.0;

    mat[12] = 0.0;
    mat[13] = 0.0;
    mat[14] = 2.0 * zNear * zFar / delta;
    mat[15] = 0.0;

    glMultMatrixd(mat);

    delete[] mat;
}

void CCanvas::lookAt(const GLdouble eyeX,
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

void CCanvas::resizeGL(int width, int height) {
    // set up the window-to-viewport transformation
    glViewport(0, 0, width, height);

    // vertical camera opening angle
    double beta = 60.0;

    // aspect ratio
    double gamma;
    if (height > 0)
        gamma = width / (double)height;
    else
        gamma = width;

    // front and back clipping plane at
    double n = -0.01;
    double f = -1000.0;

    // frustum corners
    // double t = -tan(beta * 3.14159 / 360.0) * n;
    // double b = -t;
    // double r = gamma * t;
    // double l = -r;

    // set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glFrustum(l,r, b,t, -n,-f);

    // alternatively, directly from alpha and gamma
    glPerspective(beta, gamma, -n, -f);
}

//-----------------------------------------------------------------------------

void CCanvas::setView(View _view) {
    switch (_view) {
        case Side: {
            lookAt(-0.5, -0.25f, -10,
                   0, 0, 0,
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
        case Above: {
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

void CCanvas::paintGL() {
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Setup the current view
    setView(View::Manual);

    // You can always change the light position here if you want
    //    Point3d newSunPos(sunPosition.x() * sin(10*tau), sunPosition.y(), sunPosition.z() * cos(10*tau));
    //    GLfloat lightpos[] = {(float)newSunPos.x(), (float)newSunPos.y(), (float)newSunPos.z(), 0.0f};
    glPushMatrix();
    GLfloat lightpos[] = {(float)sunPosition.x(), (float)sunPosition.y(), (float)sunPosition.z(), 0.0f};
    glRotatef(sunSpeed * tau, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glPopMatrix();

    /*** Draw Axes in the global coordinate system ***/

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(-3.0f, 0.0f, 0.0f);
    glVertex3f(60.0f, 0.0f, 0.0f);
    glEnd();
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, -3.0f, 0.0f);
    glVertex3f(0.0f, 60.0f, 0.0f);
    glEnd();
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -3.0f);
    glVertex3f(0.0f, 0.0f, 60.0f);
    glEnd();
    // vector to sun
    //    glColor3f(1.0f, 1.0f, 1.0f);
    //    glBegin(GL_LINES);
    //        glVertex3f(newSunPos.x()*100000, newSunPos.y()*100000, newSunPos.z()*100000);
    //        glVertex3f(0.0f, 0.0f, 0.0f);
    //    glEnd();
    glEnable(GL_LIGHTING);

    /**** Setup and draw your objects ****/

    // You can freely enable/disable some of the lights in the scene as you wish
    glEnable(GL_LIGHT0);
    //    glDisable(GL_LIGHT1);
    // Before drawing an object, you can set its material properties
    /*
     * Bind texture and push new matrix before drawing
     */
    bird.getTexture().bind();
    glPushMatrix();

    /*
     * Obtaining the values of the current modelview matrix
     *  GLfloat matrix[16];
     *  glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
    */

    /* Increment tau (for project main animation)
     */
    tau += tauIncrement;

    /*
     * Update flight path of the bird
     */
    bird.fly(tau);

    /*
     * Draw Bird
     */
    bird.inc();
    bird.draw();

    /*
     * Unbind texture and pop matrix
     * Remove the last transformation matrix from the stack - you have drawn your last
     * object with a new transformation and now you go back to the previous one
     */
    glPopMatrix();
    bird.getTexture().unbind();

    /*
     * Bind texture and push new matrix before drawing
     */
    scene.getTexture().bind();
    glPushMatrix();

    /*
     * Obtaining the values of the current modelview matrix
     *  GLfloat matrix[16];
     *  glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
    */

    GLfloat scale2 = 1.0f;
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f);
    glTranslatef(-5.0f, -9.0f, -30.0f);
    glScalef(scale2, scale2, scale2);

    /*
     * Draw Scene
     */
    scene.draw();

    /*
     * Unbind texture and pop matrix
     * Remove the last transformation matrix from the stack - you have drawn your last
     * object with a new transformation and now you go back to the previous one
     */
    glPopMatrix();
    scene.getTexture().unbind();

    sky.getTexture().bind();

    sky.draw(tau);

    sky.getTexture().unbind();
}

void CCanvas::keyPressEvent(QKeyEvent *event) {
    Point3d forward(manualDirection.x(), 0, manualDirection.z());
    Point3d rightward(manualDirection.z(), 0, -manualDirection.x());
    Point3d upperward(0, 1, 0);
    cout << manualDirection.x() << ' ' << manualDirection.y() << ' ' << manualDirection.z() << endl;

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
            manualAngleHorizontal -= 0.02;
            manualDirection.x() = cos(manualAngleHorizontal);
            manualDirection.z() = sin(manualAngleHorizontal);
            break;
        case Qt::Key_L:
            manualAngleHorizontal += 0.02;
            manualDirection.x() = cos(manualAngleHorizontal);
            manualDirection.z() = sin(manualAngleHorizontal);
            break;
        case Qt::Key_I:
            if (manualAngleVertical <= 1) {
                manualAngleVertical += 0.02;
                manualDirection.y() = manualAngleVertical;
            }
            break;
        case Qt::Key_K:
            if (manualAngleVertical >= -1) {
                manualAngleVertical -= 0.02;
                manualDirection.y() = manualAngleVertical;
            }
            break;
    }
}
