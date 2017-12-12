#ifndef SKY_H
#define SKY_H

#include "ObjModel.h"
#include "texture.hpp"

class Sky {
public:
    /*
     * Load object files and store vertices, normals, uvs
     */
    Sky() : atmosphere("./models/sky/sky.obj"),
              texture("./models/sky/texture/skydome.png") {
        std::cout << "Loaded Sky parts" << std::endl;
    }

    void init();
    void draw(GLfloat tau);

    /* Getters
     */
    Texture& getTexture() { return this->texture; }

private:
    // all parts of the scene (ObjModels, drawable independently)
    ObjModel atmosphere;

    // texture
    Texture texture;
};

#endif // SKY_H
