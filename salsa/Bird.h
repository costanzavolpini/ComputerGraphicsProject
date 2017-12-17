#ifndef BIRD_H
#define BIRD_H

#include "ObjModel.h"
#include "texture.hpp"
#include <iostream>

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

        startDirection = Point3d(0.0f, 0.0f, 1.0f);

        /* PATH VARIABLES */
        indexPath = 0;
        maxPath = 5;
        path[0] = Point3d(0, 0, -80);
        path[1] = Point3d(2, 0, -30);
        path[2] = Point3d(0, 0, 0);
        path[3] = Point3d(-5, 0, 8);
        path[4] = Point3d(20, 0, 9);

        pathT = 0;

    }

    Point3d startDirection;

    static constexpr GLfloat psiIncrement = 0.1f;

    void init();
    void draw();

    void inc();
    void fly(GLfloat tau);
    Point3d flyPath(GLfloat tau);

    /* Getters
     */
    Texture &getTexture() { return this->texture; }
    Point3d &getDirection() { return this->direction; }
    Point3d &getPosition() { return this->position; }
    int orientationTest(Point3d, Point3d, Point3d);

    /* Setters
     */
    void setAnimate(bool animate) { this->animate = animate; }
    void setMove(bool move) { this->move = move; }

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

    // current position and direction the bird is facing
    Point3d direction;
    Point3d position;

    // path
    Point3d path[20];
    int indexPath;
    int maxPath;
    int speed;
    int pathT;
};

#endif // BIRD_H
