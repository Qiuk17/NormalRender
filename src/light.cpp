#include "light.h"

LightInteraction PointLight::castOnPoint(const Vector3f& point) const {
    return LightInteraction(this, position - point, color * intensity);
}

LightInteraction DirectionalLight::castOnPoint(const Vector3f& point) const {
    return LightInteraction(this, -direction, color * intensity);
}
