#include "CCanvas.h"
#include "Base.h"
#include "Sphere.h"

//shadow
#define SHADOW_MAP_RATIO 2
#define RENDER_WIDTH 640.0
#define RENDER_HEIGHT 480.0
// Hold id of the framebuffer for light POV rendering
GLuint fboId;

// Z values will be rendered to this texture when using fboId framebuffer
GLuint depthTextureId;

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

//-----------------------------------------------------------------------------

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

/**
* Rendering 1. Create shadowMap (it contains the depth of each element that is visible from the light source (sun))
* Rendering 2. View from camera's point of view. shadowFramebuffer contains standard projection of vertices into screen space.
* Vertex-shader contains each vertex that should be projected into space.
* Fragment-shader can access to shadowMap, it contains position of vertex in light source's space.
* Using screen space position of fragment, check: 
* If distance from shadowMap < distance current fragment to light source, then current fragment is in shadow.
* 
* reference: http://www.sunandblackcat.com/tipFullView.php?l=eng&topicid=34 
*/

void CCanvas::generateShadowMap(){
    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
     GLuint FramebufferName = 0;
     glGenFramebuffers(1, &FramebufferName);
     glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

     // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
     GLuint depthTexture;
     glGenTextures(1, &depthTexture);
     glBindTexture(GL_TEXTURE_2D, depthTexture);
     glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

     glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

     glDrawBuffer(GL_NONE); // No color buffer is drawn to.

     // Always check that our framebuffer is ok
//     if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//     return false;
}

//-----------------------------------------------------------------------------
/*
 * Shadow Mapping for point light (sun)
 * To calculate all shadows from point light source it's required to save information in shadow cube map for all space around the light source.
 * To use cube map as shadow map it's required to render scene for six times into each face of cube texture.
 * 
 * reference: http://www.sunandblackcat.com/tipFullView.php?l=eng&topicid=36
 */ 
void CCanvas::viewAndProjectMatrixShadow(){
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, shadowMapSize, shadowMapSize, 
    //         0, GL_RED, GL_FLOAT, nullptr);

    //TODO understand how it works
}

//-----------------------------------------------------------------------------

void CCanvas::paintGL() {
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



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



    /*
     * Shadow Management:
     * generate shadow map (preprocessing step)
     */
    generateShadowMap();
//    viewAndProjectMatrixShadow(); //TODO : View and projection matrices for light source's point of view

    //TODO : Precision of the shadow map
    //TODO : Rendering into the shadow map
    //etc........



    /*
     * Setup the current view
     */
    camera.setView(view, bird);



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
    switch (event->key()) {
        case Qt::Key_W:
        case Qt::Key_A:
        case Qt::Key_S:
        case Qt::Key_D:
        case Qt::Key_Space:
        case Qt::Key_Z:
        case Qt::Key_J:
        case Qt::Key_L:
        case Qt::Key_I:
        case Qt::Key_K:
            camera.keyPressEvent(event, view);
            break;

        case Qt::Key_Control:
            view = camera.getNextView(view);
            break;
    }
}
