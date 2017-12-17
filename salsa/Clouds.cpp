#include "Clouds.h"

/*
 * Costructor: creates array of positions of particles
 */
Clouds::Clouds() {
    std::vector<Point3d> positions;
    positions.push_back(Point3d(0.0f, 1.0f, -5.0f));
    positions.push_back(Point3d(0.0f, 2.0f,  5.0f));
    positions.push_back(Point3d(0.0f, 3.0f, -5.0f));
    positions.push_back(Point3d(0.0f, 4.0f,  5.0f));
    std::vector<Point3d> colors;
    colors.push_back(Point3d(0.0f, 0.0f, 0.0f));
    colors.push_back(Point3d(0.0f, 0.0f, 0.0f));
    colors.push_back(Point3d(0.0f, 0.0f, 0.0f));
    colors.push_back(Point3d(0.0f, 0.0f, 0.0f));
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
     -50.0,  -50.0f,  0.0f,
      50.0f, -50.0f,  0.0f,
     -50.0f,  50.0f,  0.0f,
      50.0f,  50.0f,  0.0f,
    };
//    static const GLfloat g_vertex_buffer_data[] = {
//        -0.5f, -0.5f,  0.0f,
//         0.5f, -0.5f,  0.0f,
//        -0.5f,  0.5f,  0.0f,
//         0.5f,  0.5f,  0.0f,
//    };
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}

void Clouds::inc(GLfloat tau) {

}

void Clouds::draw() {
//    glDisable(GL_LIGHTING);
    // Update the buffers that OpenGL uses for rendering.
    // There are much more sophisticated means to stream data from the CPU to the GPU,
    // but this is outside the scope of this tutorial.
    // http://www.opengl.org/wiki/Buffer_Object_Streaming

    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(GLfloat) * 3, &particlePositions[0]);

    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(GLubyte) * 3, &particleColors[0]);



    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(
     0, // attribute. No particular reason for 0, but must match the layout in the shader.
     3, // size
     GL_FLOAT, // type
     GL_FALSE, // normalized?
     0, // stride
     (void*)0 // array buffer offset
    );

    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glVertexAttribPointer(
     1, // attribute. No particular reason for 1, but must match the layout in the shader.
     3, // size : x + y + z => 3
     GL_FLOAT, // type
     GL_FALSE, // normalized?
     0, // stride
     (void*)0 // array buffer offset
    );

    // 3rd attribute buffer : particles' colors
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glVertexAttribPointer(
     2, // attribute. No particular reason for 1, but must match the layout in the shader.
     3, // size : r + g + b => 3
     GL_UNSIGNED_BYTE, // type
     GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
     0, // stride
     (void*)0 // array buffer offset
    );



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

//    glColor3f(1.0f, 0.0f, 1.0f);
//    glBegin(GL_LINES);
//    glVertex3f(-1.0f, -1.0f, -1.0f);
//    glVertex3f(1.0f, 1.0f, 1.0f);
//    glEnd();
//    glEnable(GL_LIGHTING);
}

void Clouds::vecPoint3dToFloat(std::vector<Point3d> &_vec, std::vector<GLfloat> &_out) {
    _out.clear();
    for (auto i = _vec.begin(); i != _vec.end(); ++i) {
        _out.push_back(i->x());
        _out.push_back(i->y());
        _out.push_back(i->z());
    }
}
