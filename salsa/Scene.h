#ifndef SCENE_H
#define SCENE_H

#include "ObjModel.h"
#include "texture.hpp"

class Scene {
public:
    /*
     * Load object files and store vertices, normals, uvs
     */
    Scene() : landscape("./models/mountains/mountains.obj"),
              texture("./models/mountains/texture/Color.tif") {
        std::cout << "Loaded Scene parts" << std::endl;
    }

    void init();
    void draw();

    /* Getters
     */
    Texture& getTexture() { return this->texture; }

private:
    // all parts of the scene (ObjModels, drawable independently)
    ObjModel landscape;

    // texture
    Texture texture;
};

#endif // SCENE_H
