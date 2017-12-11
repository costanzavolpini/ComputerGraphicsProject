#ifndef BIRD_H
#define BIRD_H

#include "ObjModel.h"
#include "texture.hpp"

static std::string parts_path = "./models/bird/eagle/parts_2/";
static std::string textures_path = "./models/bird/eagle/texture/";

class Bird {

public:
    /*
     * Load object files and store vertices, normals, uvs
     */
    Bird() : body(parts_path + "body.obj"),
             head(parts_path + "head.obj"),
             tail(parts_path + "tail.obj"),
             left_wing_far(parts_path + "left_wing_far.obj"),
             left_wing_close(parts_path + "left_wing_close.obj"),
             right_wing_far(parts_path + "right_wing_far.obj"),
             right_wing_close(parts_path + "right_wing_close.obj"),
             texture(textures_path + "full.png") {
        std::cout << "Loaded Bird parts" << std::endl;
    }


    void init();
    void draw();

    void inc();

    /* Getters
     */
    Texture& getTexture() { return this->texture; }

    /* Setters
     */
    void setAnimate(bool animate) { this->animate = animate; }

private:
    // whether to animate the bird (moving wings, tail, ...) or not
    bool animate;

    // whether to move the bird (on some path) or not
    bool move;

    // all parts of the bird (ObjModels, drawable independently)
    ObjModel body;
    ObjModel head;
    ObjModel tail;
    ObjModel left_wing_far;
    ObjModel left_wing_close;
    ObjModel right_wing_far;
    ObjModel right_wing_close;

    // texture
    Texture texture;

    // incremented number for animations
    GLfloat psi;
};

#endif // BIRD_H
