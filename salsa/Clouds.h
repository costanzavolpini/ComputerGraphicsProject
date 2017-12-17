#ifndef CLOUDS_H
#define CLOUDS_H

#include <QtOpenGL>
#include "Base.h"

class Clouds {
public:

    Clouds();
    void init();
    void inc(GLfloat tau);
    void draw();

private:

    /*
     * Single particle buffer (12 vertices) defined in Clouds::init
     */

    // Vector with positions of particles
    std::vector<GLfloat> particlePositions;

    // Vector with color of particles
    std::vector<GLfloat> particleColors;


    // The maximum number of particles that we render and the actual number
    GLuint64 maxParticles = 1e6;
    GLuint64 particlesCount = 4;

    // Buffers (for single particle quad, particle positions, particle colors
    GLuint billboard_vertex_buffer;
    GLuint particles_position_buffer;
    GLuint particles_color_buffer;

    // converter
    void vecPoint3dToFloat(std::vector<Point3d> &_vec, std::vector<GLfloat> &_out);
};

#endif // CLOUDS_H
