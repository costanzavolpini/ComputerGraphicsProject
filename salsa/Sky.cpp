#include "Sky.h"

void Sky::init() {
    atmosphere.init();
    texture.setTexture();
}

void Sky::draw() {
    glPushAttrib(GL_LIGHTING_BIT);

        // glColor3f(1.0f, 1.0f, 1.0f);
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
    glTranslatef(0,-0.1f,0);


    atmosphere.draw();
    glPopAttrib();
    glPopMatrix();
}
