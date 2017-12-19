#include "Bird.h"
#include "CCanvas.h"

/*
 * Initialise buffers to draw
 */
void Bird::init() {
    body.init();
    head.init();
    tail.init();
    left_wing_far.init();
    left_wing_close.init();
    right_wing_far.init();
    right_wing_close.init();
    texture.setTexture();

    psi = 0.0f;
    direction = startDirection;

    /*
     * Setup path
     */
    path.reserve(100);

    // Start point (origin)
    path.push_back(Point3d(0, 0, 0));

    //  Points on path
    // ATTENTION: TWO SAME VERTICES IN A ROW CRASH THE APP //
    path.push_back(Point3d( -10,   0,  10 ));
    path.push_back(Point3d( -35,   0,  10 ));
    path.push_back(Point3d( -50,   0,  20 ));
    path.push_back(Point3d( -70,   0,  40 ));
    path.push_back(Point3d( -90,   0,  60 ));
    path.push_back(Point3d(-100,   0,  40 ));
    path.push_back(Point3d( -90,   0,  20 ));
    path.push_back(Point3d( -70,   0,  10 ));
    path.push_back(Point3d( -50,   0,   0 ));
    path.push_back(Point3d( -40,   5, -20 ));
    path.push_back(Point3d( -30,   5, -40 ));
    path.push_back(Point3d( -20,   5, -60 ));
    path.push_back(Point3d(   0,   5, -70 ));
    path.push_back(Point3d(  20,   5, -65 ));
    path.push_back(Point3d(  35,   5, -65 ));
    path.push_back(Point3d(  50,   10, -80 ));
    path.push_back(Point3d(  70,   10, -60 ));
    path.push_back(Point3d(  90,   15, -40 ));
    path.push_back(Point3d(  80,   12, -30 ));
    path.push_back(Point3d(  20,    7, -10 ));

    // Variables and constants
    indexPath = 0;
    pathT = 0;
    pathLength = path.size();

    /*
     * Get first position
     */
    position = flyPath(0.0f);
}

/*
 * Draw all parts
 * Each part is saved as an object at the origin (in general,
 * one side at the origin for all pieces except the body).
 *
 * Before drawing, translate each object back into the correct
 * position so that the bird is complete. These transformations are
 * hardcoded based on how the files were created, the transformations
 * are detailed in the file part_translations.txt
 * Do the transformations in the same order as they are described in
 * the file, but reverse the sign (except for the depth axis that is
 * inverted in Blender) and remember that Z and Y are inverted in Blender.
 *
 * Finally apply local transformations to animate the bird.
 */
void Bird::draw() {

    /*
     * transformations to see bird from different perspectives
     */
    //    GLfloat scale = 0.2f;
    //    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);

    // oscillate
    //    glRotatef(100*tau, 0.0f, 1.0f, 0.0f);
    //    glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
    //    glRotatef(50*tau, 1.0f, 0.0f, 0.0f);
    //    glTranslatef(4.0f, 2.0f, 0.0f);
    //    glScalef(scale, scale, scale);

    /*
     * Slight correction to whole bird (it should not look too much down)
     */
    glRotatef(-6.0f, 1, 0, 0);

    /* Body:
     * - static
     * - center at origin
     * - no movement applied
     */
    glPushMatrix(); // push body
    if (this->animate) {
        glTranslatef(0.0f, -0.1 * sin(psi), 0.0f);
        glRotatef(sin(psi), 1.0f, 0.0f, 0.0f);
    }
    body.draw();

    /* Head:
     * - can move (e.g. look a bit around)
     * - back of head (neck connection) at origin
     * - translated forward and down
     */
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 1.2f);
    head.draw();
    glPopMatrix();

    /* Tail:
     * - can move (e.g. up and down)
     * - connection with body at origin
     * - translated backwards
     */
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -1.75f);
    tail.draw();
    glPopMatrix();

    /* Left wing:
     * - can move (e.g. up and down)
     * - connection with body at origin
     * - two pieces, many transformations (also nested)
     */
    glPushMatrix(); // push left_wing_close
    glTranslatef(0.7f, 0.2f, 0.0f);
    glRotatef(16.0f, 0.0f, 0.0f, 1.0f);  // Z: + 16.0
    glRotatef(-6.75f, 0.0f, 1.0f, 0.0f); // Y: -  6.75

    // animation
    if (this->animate) {
        glRotatef(32 * sin(psi), 0.0f, 0.0f, 1.0f);
    }
    left_wing_close.draw();

    /* translate and draw far part after close part of wing
     */
    glPushMatrix(); // push left_wing_far
    glTranslatef(1.6f, 0.0f, 1.0f);
    glRotatef(-12.0f, 0.0f, 0.0f, 1.0f); // Z: - 12.0

    // animation
    if (this->animate) {
        glRotatef(16 * sin(psi), 0.0f, 0.0f, 1.0f);
    }
    left_wing_far.draw();
    glPopMatrix(); // pop left_wing_far

    glPopMatrix(); // pop left_wing_close

    /* Right wing:
     * - can move (e.g. up and down)
     * - connection with body at origin
     * - two pieces, many transformations (also nested)
     */
    glPushMatrix(); // push right_wing_close
    glTranslatef(-0.7f, 0.2f, 0.0f);
    glRotatef(-16.0f, 0.0f, 0.0f, 1.0f); // Z: - 16.0
    glRotatef(6.75f, 0.0f, 1.0f, 0.0f);  // Y: +  6.75

    // animation
    if (this->animate) {
        glRotatef(32 * sin(psi), 0.0f, 0.0f, -1.0f);
    }
    right_wing_close.draw();

    /* translate and draw far part after close part of wing
     */
    glPushMatrix(); // push right_wing_far
    glTranslatef(-1.25f, 0.0f, 0.5f);
    glRotatef(12.0f, 0.0f, 0.0f, 1.0f); // Z: + 12.0

    // animation
    if (this->animate) {
        glRotatef(16 * sin(psi), 0.0f, 0.0f, -1.0f);
    }
    right_wing_far.draw();
    glPopMatrix(); // pop right_wing_far

    glPopMatrix(); // pop right_wing_close

    glPopMatrix(); // pop body
}

/*
 * Increment number for animations
 */
void Bird::inc() {
    psi += psiIncrement;
}

void Bird::fly(GLfloat tau) {
    if (this->move) {
        Point3d currPos = this->position;
        Point3d nextPos = flyPath(tau);

        glTranslatef(currPos.x(), currPos.y(), currPos.z());

        GLfloat scale = 0.2f;
        glScalef(scale, scale, scale);

        direction = (nextPos - currPos).normalized();

        Point3d directionXZ = Point3d(direction.x(), 0.0f, direction.z());
        Point3d startDirectionXZ = Point3d(startDirection.x(), 0.0f, startDirection.z());
        GLfloat yAngle = startDirectionXZ.getAngle(directionXZ) * 180 / PI;

        Point3d directionYZ = Point3d(0.0f, direction.y(), direction.z());
        Point3d startDirectionYZ = Point3d(0.0f, startDirection.y(), startDirection.z());
        GLfloat xAngle = startDirectionYZ.getAngle(directionYZ) * 180 / PI;
//        cout << "angle is: " << xAngle;

        Point3d directionXY = Point3d(direction.x(), direction.y(), 0.0f);
        Point3d startDirectionXY = Point3d(direction.x(), startDirection.y(), 0.0f);
        GLfloat zAngle = startDirectionXY.getAngle(directionXY) * 180 / PI;


        GLfloat sign = (startDirectionXZ ^ directionXZ).y();
        yAngle = copysign(yAngle, sign);

        sign = (startDirectionYZ ^ directionYZ).x();
        xAngle = copysign(xAngle, sign);

        sign = (startDirectionXY ^ directionXY).z();
        zAngle = copysign(zAngle, sign);

//        glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
        glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
//        glRotatef(zAngle, 0.0f, 0.0f, 1.0f);

        // save position
        this->position = nextPos;
    }
}

int Bird::orientationTest(Point3d a, Point3d mid, Point3d b) {
    // orientation test on x and z axys
    // 0: collinear
    // positive: left
    // negative: right
    double cross = (mid.x() - a.x()) * (b.z() - a.z()) - (mid.z() - a.z()) * (b.x() - a.x());
    if(cross < -0.0001) return -1; // right
    if(cross > 0.0001) return 1;
    return 0;                      // collinear
}

//void Bird::test() {
//    float xcr, ycr;   //Points on the Catmull-Rom spline
//    float dx, dy;     //tangent components
//    glClear(GL_COLOR_BUFFER_BIT);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();

//    glPointSize(6.0);
//    glColor3f(1.0, 0.0, 1.0);
//    glBegin(GL_POINTS);
//    for(int i = 0; i < numPts; i++)
//           glVertex2f(x[i], y[i]);
//    glEnd();

//    if(numPts > 3)
//    {
//       glColor3f(1.,0.,0.);
//       glBegin(GL_LINES); //draw tangents
//       for(int i = 1; i < numPts-1; i++){
//           dx = 0.2*(x[i+1]-x[i-1]);
//           dy = 0.2*(y[i+1]-y[i-1]);
//           glVertex2f(x[i]-dx, y[i]-dy);
//           glVertex2f(x[i]+dx,y[i]+dy);
//        }
//        glEnd();

//        glColor3f(0., 0., 1.);
//        glBegin(GL_LINE_STRIP);
//        for(int i = 1; i < numPts-2; i++)
//        {
//            for(int k = 0;  k < 50; k++){    //50 points
//               float t = k*0.02;  //Interpolation parameter
//               //--Eq. (7.34)--
//               xcr = x[i] + 0.5*t*(-x[i-1]+x[i+1])
//                   + t*t*(x[i-1] - 2.5*x[i] + 2*x[i+1] - 0.5*x[i+2])
//                   + t*t*t*(-0.5*x[i-1] + 1.5*x[i] - 1.5*x[i+1] + 0.5*x[i+2]);
//               ycr = y[i] + 0.5*t*(-y[i-1]+y[i+1])
//                   + t*t*(y[i-1] - 2.5*y[i] + 2*y[i+1] - 0.5*y[i+2])
//                   + t*t*t*(-0.5*y[i-1] + 1.5*y[i] - 1.5*y[i+1] + 0.5*y[i+2]);
//               glVertex2f(xcr, ycr);
//           }
//        }
//        glEnd();
//    }
//    glFlush();
//}

Point3d Bird::flyPath(GLfloat tau) {

    /* Draw path (for debugging) */
    glLineWidth(6.0);
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < pathLength; ++i) {
        glVertex3f(path[i].x(), path[i].y(), path[i].z());
    }
    glEnd();

    float xcr, ycr, zcr;   // Points on the Catmull-Rom spline

    if (pathT == 0) indexPath = (indexPath + 1) % pathLength;

    /*
     * Four points for interpolation
     */
    int prev = (indexPath == 0) ? pathLength - 1 : indexPath - 1;
    Point3d p1 = path[prev];
    Point3d p2 = path[indexPath];
    Point3d p3 = path[(indexPath + 1) % pathLength];
    Point3d p4 = path[(indexPath + 2) % pathLength];

    /*
     * If we just changed pair of points, compute new speed between them
     */
    if (pathT == 0) {
       Point3d p2_25 = catmull_point(0.25, p1, p2, p3, p4);
       Point3d p2_5 = catmull_point(0.5, p1, p2, p3, p4);
       Point3d p2_75 = catmull_point(0.75, p1, p2, p3, p4);
       float distance = (p3 - p2_75).norm() + (p2_75 - p2_5).norm() + (p2_5 - p2_25).norm() + (p2_25 - p2).norm();
       std::cout << "Current: " << p2;
       speed = ceil(distance * 15);
       // std::cout << speed << ' ' << distance << std::endl;
    }

    /*
     * Do interpolation
     */
    float t = (float)pathT / (float)speed;  //Interpolation parameter
    xcr = catmull_interp(t, p1.x(), p2.x(), p3.x(), p4.x());
    ycr = catmull_interp(t, p1.y(), p2.y(), p3.y(), p4.y());
    zcr = catmull_interp(t, p1.z(), p2.z(), p3.z(), p4.z());

    // Increment step between current pair of points, return current point between pair
    pathT = (pathT + 1) % speed;
    return Point3d(xcr, ycr, zcr);
}

float Bird::catmull_interp(float t, float c1, float c2, float c3, float c4) {
    return c2 + 0.5*t*(-c1 + c3) + t * t * (c1 - 2.5 * c2 + 2 * c3 - 0.5 * c4)
            + t * t * t * (-0.5 * c1 + 1.5 * c2 - 1.5 * c3 + 0.5 * c4);
}

Point3d Bird::catmull_point(float t, Point3d p1,  Point3d p2,  Point3d p3,  Point3d p4) {
    return Point3d(catmull_interp(t, p1.x(), p2.x(), p3.x(), p4.x()), catmull_interp(t, p1.y(), p2.y(), p3.y(), p4.y()),
                   catmull_interp(t, p1.z(), p2.z(), p3.z(), p4.z()));
}
