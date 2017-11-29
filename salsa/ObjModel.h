#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "Point2.h"
#include "Point3.h"
#include <QtOpenGL>

class ObjModel {
  public:
    ObjModel(const std::string &_path);
    void init();
    void draw();

  private:
    std::vector<GLfloat> fvertices;
    std::vector<GLfloat> fuvs;
    std::vector<GLfloat> fnormals; // Won't be used at the moment

    GLuint vertexBuffer;
    GLuint uvBuffer;
};

#endif // SPHERE_H
