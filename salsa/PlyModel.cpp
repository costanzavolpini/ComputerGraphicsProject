#include "PlyModel.h"
#include "Base.h"
#include <math.h>
#include <fstream>

#include "tinyply.h"

PlyModel::PlyModel(const std::string &_path) {
    // Read the file and create a std::istringstream suitable
    // for the lib -- tinyply does not perform any file i/o.
    std::ifstream ss(_path, std::ios::binary);

    // Parse the ASCII header fields
    tinyply::PlyFile file(ss);

    /*for (auto e : file.get_elements())
    {
        std::cout << "element - " << e.name << " (" << e.size << ")" << std::endl;
        for (auto p : e.properties)
        {
            std::cout << "\tproperty - " << p.name << " (" << PropertyTable[p.propertyType].str << ")" << std::endl;
        }
    }
    std::cout << std::endl;

    for (auto c : file.comments)
    {
        std::cout << "Comment: " << c << std::endl;
    }*/

    // Define containers to hold the extracted data. The type must match
    // the property type given in the header. Tinyply will interally allocate the
    // the appropriate amount of memory.
    std::vector<float> verts;
    std::vector<float> norms;
    std::vector<uint8_t> colors;
    std::vector<uint32_t> faces;
    std::vector<float> uvCoords;

    // Read file line by line

    uint32_t vertexCount, normalCount, colorCount, faceCount, faceTexcoordCount, faceColorCount;
    vertexCount = normalCount = colorCount = faceCount = faceTexcoordCount = faceColorCount = 0;

    // The count returns the number of instances of the property group. The vectors
    // above will be resized into a multiple of the property group size as
    // they are "flattened"... i.e. verts = {x, y, z, x, y, z, ...}
    vertexCount = file.request_properties_from_element("vertex", { "x", "y", "z" }, verts);
    normalCount = file.request_properties_from_element("vertex", { "nx", "ny", "nz" }, norms);
    colorCount = file.request_properties_from_element("vertex", { "red", "green", "blue", "alpha" }, colors);

    // For properties that are list types, it is possibly to specify the expected count (ideal if a
    // consumer of this library knows the layout of their format a-priori). Otherwise, tinyply
    // defers allocation of memory until the first instance of the property has been found
    // as implemented in file.read(ss)
    faceCount = file.request_properties_from_element("face", { "vertex_indices" }, faces, 3);
    faceTexcoordCount = file.request_properties_from_element("face", { "texcoord" }, uvCoords, 6);

    // Now populate the vectors...
    file.read(ss);

    //std::cout << "Vertices: " << vertexCount << ", " << verts.size() << std::endl;
    //std::cout << "Normals: " << normalCount << ", " << norms.size() << std::endl;
    //std::cout << "Colors: " << colorCount << ", " << colors.size() << std::endl;
    //std::cout << "Faces: " << faceCount << ", " << faces.size() << std::endl;
    //std::cout << "TexCoords: " << faceTexcoordCount << ", " << uvCoords.size() << std::endl;

    // Copy red values into opengl arrays
    for(auto i = faces.begin(); i != faces.end(); i++) {
        fvertices.push_back(verts.at(*i * 3));
        fvertices.push_back(verts.at(*i * 3 + 1));
        fvertices.push_back(verts.at(*i * 3 + 2));
        fnormals.push_back(norms.at(*i * 3));
        fnormals.push_back(norms.at(*i * 3 + 1));
        fnormals.push_back(norms.at(*i * 3 + 2));
        fcolors.push_back((GLfloat)(colors.at(*i * 4)) / 255.0f);
        fcolors.push_back((GLfloat)(colors.at(*i * 4 + 1)) / 255.0f);
        fcolors.push_back((GLfloat)(colors.at(*i * 4 + 2)) / 255.0f);
        fcolors.push_back((GLfloat)(colors.at(*i * 4 + 3)) / 255.0f);
    }
    for(auto i = uvCoords.begin(); i != uvCoords.end(); i++) {
        fuvs.push_back(*i);
    }
}

void PlyModel::init() {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, fvertices.size() * sizeof(GLfloat), &fvertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, fuvs.size() * sizeof(GLfloat), &fuvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, fnormals.size() * sizeof(GLfloat), &fnormals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, fcolors.size() * sizeof(GLfloat), &fcolors[0], GL_STATIC_DRAW);
}

void PlyModel::draw() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexPointer(
                3,                  // size
                GL_FLOAT,           // type
                0,                  // stride
                (void*)0            // array buffer offset
            );

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glTexCoordPointer(
        2,                                // size
        GL_FLOAT,                         // type
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glNormalPointer(
        GL_FLOAT,                         // type
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    /*
     * For some reason the color array is not being mapped while drawing with glDrawArrays this way.
     * I could not resolve this issue - maybe some of you will have more luck.
     */
    /*glEnableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glColorPointer(
        4,                                // size
        GL_FLOAT,                 // type
        0,                                // stride
        (void*)0                          // array buffer offset
    );*/

    glDrawArrays(GL_TRIANGLES, 0, fvertices.size() / 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
}
