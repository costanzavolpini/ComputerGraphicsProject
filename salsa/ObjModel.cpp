#include "ObjModel.h"
#include "Base.h"
#include <math.h>

#include "objloader.hpp"

ObjModel::ObjModel(const std::string &_path) {
    std::vector<Point3d> vertices;
    std::vector<Point2d> uvs;
    std::vector<Point3d> normals;
    bool res = loadOBJ(_path.c_str(), vertices, uvs, normals);
    vecPoint3dToFloat(vertices, fvertices);
    vecPoint2dToFloat(uvs, fuvs);
    vecPoint3dToFloat(normals, fnormals);
}

void ObjModel::init() {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, fvertices.size() * sizeof(GLfloat), &fvertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, fuvs.size() * sizeof(GLfloat), &fuvs[0], GL_STATIC_DRAW);
}

void ObjModel::draw() {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexPointer(
                3,                  // size
                GL_FLOAT,           // type
                0,                  // stride
                (void*)0            // array buffer offset
            );
    glEnableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glTexCoordPointer(
        2,                                // size
        GL_FLOAT,                         // type
        0,                                // stride
        (void*)0                          // array buffer offset
    );
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glDrawArrays(GL_TRIANGLES, 0, fvertices.size() / 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
