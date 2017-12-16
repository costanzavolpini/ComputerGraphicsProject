#include "Scene.h"

void Scene::init() {
    landscape.init();
    texture.setTexture();
}

void Scene::draw() {
    glPushMatrix();
    // TODO: translate down
    glTranslatef(0,-10,0);
    landscape.draw();
    glPopMatrix();
}
