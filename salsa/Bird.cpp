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
}

/*
 * Draw all parts
 * For now, just draw them where they are.
 */
void Bird::draw() {

    glRotatef(180.0, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    body.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 2.0f);
    head.draw();
    glPopMatrix();

    glPushMatrix();
//    glRotatef(3*sin(tau), 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -1.4f);
    tail.draw();
    glPopMatrix();

//    glPushMatrix();
//    left_wing_far.draw();
//    glPopMatrix();

//    glPushMatrix();
//    left_wing_close.draw();
//    glPopMatrix();

//    glPushMatrix();
//    right_wing_far.draw();
//    glPopMatrix();

//    glPushMatrix();
//    right_wing_close.draw();
//    glPopMatrix();
}
