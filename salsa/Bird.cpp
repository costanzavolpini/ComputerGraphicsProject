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

    glPushMatrix();
    body.draw();
    glPopMatrix();

    glPushMatrix();
    head.draw();
    glPopMatrix();

    glPushMatrix();
    tail.draw();
    glPopMatrix();

    glPushMatrix();
    left_wing_far.draw();
    glPopMatrix();

    glPushMatrix();
    left_wing_close.draw();
    glPopMatrix();

    glPushMatrix();
    right_wing_far.draw();
    glPopMatrix();

    glPushMatrix();
    right_wing_close.draw();
    glPopMatrix();
}
