#include "light.h"
#include <Vector4f.h>
#include <cstdio>
#include <GL/glut.h>
LightInteraction PointLight::castOnPoint(const Vector3f& point) const {
    auto p = position - point;
    return LightInteraction(this, p, p.length(), color * intensity);
}

void PointLight::glTurnOn(int idx) const {
    auto id = GL_LIGHT0 + idx;
    glEnable(id);
    glLightfv(id, GL_DIFFUSE, Vector4f(color.toVec(), 1));
    glLightfv(id, GL_SPECULAR, Vector4f(color.toVec(), 1));
    glLightfv(id, GL_POSITION, Vector4f(position, 1));
}

LightInteraction DirectionalLight::castOnPoint(const Vector3f& point) const {
    return LightInteraction(this, -direction, 1e30f, color * intensity);
}

void DirectionalLight::glTurnOn(int idx) const {
    auto id = GL_LIGHT0 + idx;
    glEnable(id);
    glLightfv(id, GL_DIFFUSE, Vector4f(color.toVec(), 1));
    glLightfv(id, GL_SPECULAR, Vector4f(color.toVec(), 1));
    glLightfv(id, GL_POSITION, Vector4f(-direction, 0));
}
