#include "Scene.h"

void Scene::init() {
    landscape.init();
    texture.setTexture();
}

void Scene::draw() {
    glPushMatrix();
    landscape.draw();
    glPopMatrix();
}
