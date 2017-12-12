#include "Sky.h"
#include "CCanvas.h"

void Sky::init() {
    atmosphere.init();
    texture.setTexture();
}

void Sky::draw(GLfloat tau) {
    glPushAttrib(GL_LIGHTING_BIT);

    GLfloat amb[]  = {0.7f, 0.7f, 0.7f};
    GLfloat diff[] = {0.7f, 0.7f, 0.7f};
    GLfloat spec[] = {0.1f, 0.1f, 0.1f};
    GLfloat shin = 0.0001;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shin);

    glPushMatrix();
    GLfloat scale3 = 200.0f;
    glScalef(scale3, scale3, scale3);
    glTranslatef(0,-0.25f,0);
    glRotatef(-10, 0, 1, 0);

    glRotatef(CCanvas::sunSpeed*tau, 0, 1, 0);
    atmosphere.draw();
    glPopAttrib();
    glPopMatrix();
}
