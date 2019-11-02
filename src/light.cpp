#include "light.h"
#include <cstdio>
LightInteraction PointLight::castOnPoint(const Vector3f& point) const {
    auto p = position - point;
    return LightInteraction(this, p, p.length(), color * intensity);
}

LightInteraction DirectionalLight::castOnPoint(const Vector3f& point) const {
    return LightInteraction(this, -direction, 1e30f, color * intensity);
}
