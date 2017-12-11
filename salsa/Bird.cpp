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
 * For now, just draw them where they are.
 */
void Bird::draw() {

//    glRotatef(180.0, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    body.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 2.0f);
//    glRotatef(10*sin(psi/10), 0.0f, 0.0f, 1.0f);
    head.draw();
    glPopMatrix();

    glPushMatrix();
//    glRotatef(3*sin(tau), 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -1.4f);
    tail.draw();
    glPopMatrix();


    glPushMatrix();
    // hardcoded based on which movements were applied to the part on Blender
    glTranslatef(-1.0f, 0.0f, 0.45f);
    glRotatef(-17.36, 1.0f, 0.0f, 0.0f);     // X: -17.36 deg
//    glRotatef(10.00, 0.0f, 1.0f, 0.0f);     // Y: 10.00 deg
//    glRotatef(-19.7, 0.0f, 0.0f, 1.0f);     // Z: -19.7 deg

    // animation
    if (this->animate) {
      glRotatef(4*sin(psi/10), 0.0f, 0.0f, 1.0f);
    }
    left_wing_close.draw();
    glPopMatrix();

//    glPushMatrix();
//    left_wing_far.draw();
//    glPopMatrix();

//    glPushMatrix();
//    right_wing_far.draw();
//    glPopMatrix();

//    glPushMatrix();
//    right_wing_close.draw();
//    glPopMatrix();
}

/*
 * Increment number for animations
 */
void Bird::inc() {
    psi += 1.0;
}
