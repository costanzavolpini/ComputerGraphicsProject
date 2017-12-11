#include "Bird.h"

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

//    glRotatef(180.0, 0.0f, 1.0f, 0.0f);

    /* Body:
     * - static
     * - center at origin
     * - no movement applied
     */
    glPushMatrix();
    body.draw();
    glPopMatrix();

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
      glRotatef(32*sin(psi/30), 0.0f, 0.0f, 1.0f);
    }
    left_wing_close.draw();

    /* translate and draw far part after close part of wing
     */
    glPushMatrix(); // push left_wing_far
    glTranslatef(1.6f, 0.0f, 1.0f);
    glRotatef(-12.0f, 0.0f, 0.0f, 1.0f);    // Z: - 12.0

    // animation
    if (this->animate) {
      glRotatef(16*sin(psi/30), 0.0f, 0.0f, 1.0f);
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
      glRotatef(32*sin(psi/30), 0.0f, 0.0f, -1.0f);
    }
    right_wing_close.draw();

    /* translate and draw far part after close part of wing
     */
    glPushMatrix(); // push right_wing_far
    glTranslatef(-1.25f, 0.0f, 0.5f);
    glRotatef(12.0f, 0.0f, 0.0f, 1.0f);     // Z: + 12.0

    // animation
    if (this->animate) {
      glRotatef(16*sin(psi/30), 0.0f, 0.0f, -1.0f);
    }
    right_wing_far.draw();
    glPopMatrix();  // pop right_wing_far

    glPopMatrix();  // pop right_wing_close
}

/*
 * Increment number for animations
 */
void Bird::inc() {
    psi += 1.0;
}

void Bird::fly(GLfloat tau) {
    if (this->move) {
        GLfloat scale = 0.2f;
    //    glRotatef(tau, 0.5f*sin(tau/500), -1.0f, 0.0f);
        glTranslatef(cos(tau) * 4.0f, sin(tau), sin(2*tau) * 4.0f);
        glScalef(scale, scale, scale);
        glRotatef(2.5*sin(tau) * 45.0f, 0.0f, -1.0f, 0.0f);
    }
}
