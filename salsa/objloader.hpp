#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Point3.h"
#include "Point2.h"
#include <vector>

#include "gl.h"

bool loadOBJ(
	const char * path, 
    std::vector<Point3d> & out_vertices,
    std::vector<Point2d> & out_uvs,
    std::vector<Point3d> & out_normals
);

void vecPoint2dToFloat(std::vector<Point2d> &_vec, std::vector<GLfloat> &_out);
void vecPoint3dToFloat(std::vector<Point3d> &_vec, std::vector<GLfloat> &_out);

#endif
