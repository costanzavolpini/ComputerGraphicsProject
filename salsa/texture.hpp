//* (c) Copyright 2014 by
//*     Teseo Schneider / Dmitry Anisimov
//*     The texture class.

#ifndef TEXTURE_H
#define TEXTURE_H

// Qt includes.
#include <QtOpenGL>

// The texture class.
class Texture
{
public:
    // Constructor.
    Texture(const std::string &path) : loaded(false), path(path) { }

    // Bind the program.
    inline void bind()
    {
        assert(loaded);

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, name);
    }

    // Unbind the program.
    inline void unbind()
    {
        glDisable(GL_TEXTURE_2D);
    }

    // Set 2D texture.
    void setTexture()
    {
        QImageReader reader(path.c_str());
        QImage img;

        const bool read = reader.read(&img);

        if(!read) {
            std::cout << "Failed to read: " << path.c_str() << " with message:" << reader.errorString().toStdString().c_str() << "; " << std::endl;
            assert(read);
            return;
        }

        assert(img.width() > 0.0);

        img = QGLWidget::convertToGLFormat(img);

        glGenTextures(1, &name);
        glBindTexture(GL_TEXTURE_2D, name);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

        loaded = true;
    }

private:
    // Global variables.
    bool loaded;
    const std::string path;
    GLuint name;
};

#endif // TEXTURE_H
