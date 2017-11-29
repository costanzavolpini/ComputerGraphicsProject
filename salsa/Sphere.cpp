#include "Sphere.h"
#include "Base.h"
#include <math.h>

Sphere::Sphere(const int &lats, const int &longs) : lats(lats), longs(longs)
{
    build();
}

void Sphere::build()
{
    float step = 2 * PI / lats;
    for(int i = 0; i < lats; ++i) buildSegment(i, step);
}

void Sphere::buildSegment(const int &index, const float &phiStep)
{
    float phis[] = { index * phiStep, (index + 1) * phiStep };

    strips.push_back(PointArray());
    textures.push_back(TextureArray());

    PointArray &segment = strips.back();
    TextureArray &txt = textures.back();

    set(Point3d(0, 1, 0), Point2d(0.0, 1.0f * index / longs), segment, txt);

    float step = PI / longs;
    for(int i = 1; i < longs; ++i) {
        const float theta = i * step;

        for(int j = 0; j < 2; ++j) {
            const float phi = phis[j];
            const Point3d p(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));
            set(p, Point2d(1.0f * (lats - index - j) / lats, 1.0f * (longs - i) / longs), segment, txt);
        }
    }

    set(Point3d(0, -1, 0), Point2d(1.0f * index / lats, 1.0), segment, txt);
}

void Sphere::set(const Point3d &p, const Point2d &t, PointArray &segment, TextureArray &txt) const
{
    segment.push_back(p);
    assert(t.x() >= 0.0 && t.x() <= 1.0);

    // assert(t.y() >= 0.0 && t.y() <= 1.0);

    txt.push_back(t);
}

void Sphere::draw()
{
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    for(unsigned int i = 0; i < strips.size(); ++i) {
        const PointArray &segment = strips[i];
        const TextureArray &txt = textures[i];

        glBegin(GL_TRIANGLE_STRIP);
        for(unsigned int j = 0; j < segment.size(); ++j) {
            const Point3d &p = segment[j];
            const Point2d &t = txt[j];

            glNormal3d(p.x(), p.y(), p.z());
            glTexCoord2d(t.x(), t.y());
            glVertex3d(p.x(), p.y(), p.z());
        }
        glEnd();
    }
}
