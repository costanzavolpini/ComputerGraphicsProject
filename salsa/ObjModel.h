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
    std::vector<GLfloat> fnormals;

    GLuint vertexBuffer;
    GLuint uvBuffer;
    GLuint normalsBuffer;
};

#endif // SPHERE_H
