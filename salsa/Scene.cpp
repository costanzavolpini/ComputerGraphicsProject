#include "Scene.h"

void Scene::init() {
    landscape.init();
    texture.setTexture();
}

void Scene::draw() {
    glPushMatrix();
    glTranslatef(-5.0f, -19.0f, -30.0f);
    landscape.draw();
    glPopMatrix();
}
