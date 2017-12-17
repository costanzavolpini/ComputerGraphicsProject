#include "Clouds.h"

/*
 * Costructor: creates array of positions of particles
 */
Clouds::Clouds() {
    std::vector<Point3d> positions;
    positions.push_back(Point3d(1, 1, 1));
    positions.push_back(Point3d(1, 2, 1));
    positions.push_back(Point3d(1, 3, 1));
    positions.push_back(Point3d(1, 4, 1));
    std::vector<Point3d> colors;
    colors.push_back(Point3d(0, 0, 0));
    colors.push_back(Point3d(0, 0, 0));
    colors.push_back(Point3d(0, 0, 0));
    colors.push_back(Point3d(0, 0, 0));
    vecPoint3dToFloat(positions, particlePositions);
    vecPoint3dToFloat(colors, particleColors);
    std::cout << "Created clouds" << std::endl;
}

void Clouds::init() {
//    static const GLfloat g_vertex_buffer_data[] = {
//     -0.005f, -0.005f, 0.000f,
//      0.005f, -0.005f, 0.000f,
//     -0.005f,  0.005f, 0.000f,
//      0.005f,  0.005f, 0.000f,
//    };
    static const GLfloat g_vertex_buffer_data[] = {
     -500.0,  -500.0f,  0.0f,
      500.0f, -500.0f,  0.0f,
     -500.0f,  500.0f,  0.0f,
      500.0f,  500.0f,  0.0f,
    };
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}

void Clouds::inc(GLfloat tau) {

}

void Clouds::draw() {
    glDisable(GL_LIGHTING);
    // Update the buffers that OpenGL uses for rendering.
    // There are much more sophisticated means to stream data from the CPU to the GPU,
    // but this is outside the scope of this tutorial.
    // http://www.opengl.org/wiki/Buffer_Object_Streaming

    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(GLfloat) * 4, &particlePositions[0]);
//    (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);

    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(GLubyte) * 4, &particleColors[0]);


    // These functions are specific to glDrawArrays*Instanced*.
    // The first parameter is the attribute buffer we're talking about.
    // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
    // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(2, 1); // color : one per quad -> 1

    // Draw the particules !
    // This draws many times a small triangle_strip (which looks like a quad).
    // This is equivalent to :
    // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
    // but faster.
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount);
    glEnable(GL_LIGHTING);
}

void Clouds::vecPoint3dToFloat(std::vector<Point3d> &_vec, std::vector<GLfloat> &_out) {
    _out.clear();
    for (auto i = _vec.begin(); i != _vec.end(); ++i) {
        _out.push_back(i->x());
        _out.push_back(i->y());
        _out.push_back(i->z());
    }
}
