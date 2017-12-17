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
    glRotatef(16.0f, 0.0f, 0.0f, 1.0f);     // Z: + 16.0
    glRotatef(-6.75f, 0.0f, 1.0f, 0.0f);    // Y: -  6.75

    // animation
    if (this->animate) {
      glRotatef(32*sin(psi), 0.0f, 0.0f, 1.0f);
    }
    left_wing_close.draw();

    /* translate and draw far part after close part of wing
     */
    glPushMatrix(); // push left_wing_far
    glTranslatef(1.6f, 0.0f, 1.0f);
    glRotatef(-12.0f, 0.0f, 0.0f, 1.0f);    // Z: - 12.0

    // animation
    if (this->animate) {
      glRotatef(16*sin(psi), 0.0f, 0.0f, 1.0f);
    }
    left_wing_far.draw();
    glPopMatrix();  // pop left_wing_far

    glPopMatrix();  // pop left_wing_close



    /* Right wing:
     * - can move (e.g. up and down)
     * - connection with body at origin
     * - two pieces, many transformations (also nested)
     */
    glPushMatrix(); // push right_wing_close
    glTranslatef(-0.7f, 0.2f, 0.0f);
    glRotatef(-16.0f, 0.0f, 0.0f, 1.0f);    // Z: - 16.0
    glRotatef(6.75f, 0.0f, 1.0f, 0.0f);     // Y: +  6.75

    // animation
    if (this->animate) {
      glRotatef(32*sin(psi), 0.0f, 0.0f, -1.0f);
    }
    right_wing_close.draw();

    /* translate and draw far part after close part of wing
     */
    glPushMatrix(); // push right_wing_far
    glTranslatef(-1.25f, 0.0f, 0.5f);
    glRotatef(12.0f, 0.0f, 0.0f, 1.0f);     // Z: + 12.0

    // animation
    if (this->animate) {
      glRotatef(16*sin(psi), 0.0f, 0.0f, -1.0f);
    }
    right_wing_far.draw();
    glPopMatrix();  // pop right_wing_far

    glPopMatrix();  // pop right_wing_close

    glPopMatrix();  // pop body
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
        GLfloat yAngle = startDirectionXZ.getAngle(directionXZ) * 180/PI;



        GLfloat sign = (startDirectionXZ ^ directionXZ).y();
        yAngle = copysign(yAngle, sign);
        glRotatef(yAngle, 0.0f, 1.0f, 0.0f);

        // save position
        this->position = nextPos;
    }
}

/*
 * Return position relative to the given tau
 */
Point3d Bird::flyPath(GLfloat tau) {
    GLfloat speed = 0.1f;
    GLfloat amplitude = 50.0f;
    GLfloat deltaY = 0.0;
    GLfloat scale = 2 / (3 - cos(2*speed*tau));
    Point3d translate(0,5,-30);

    Point3d nextPosition(scale * amplitude * cos(speed*tau), deltaY * sin(speed*tau/13) + deltaY, scale * amplitude * sin(2*speed*tau)/2);
    nextPosition += translate;

    return nextPosition;
//    return Point3d(4*cos(tau), 0, 4*sin(tau));
}
