#ifndef PLYMODEL_H
#define PLYMODEL_H

#include "Point2.h"
#include "Point3.h"
#include <QtOpenGL>

class PlyModel {
  public:
    PlyModel(const std::string &_path);
    void init();
    void draw();

  private:
    std::vector<GLfloat> fvertices;
    std::vector<GLfloat> fuvs;
    std::vector<GLfloat> fnormals;
    std::vector<GLfloat> fcolors;

    GLuint vertexBuffer;
    GLuint uvBuffer;
    GLuint normalBuffer;
    GLuint colorBuffer;
};

#endif // SPHERE_H
