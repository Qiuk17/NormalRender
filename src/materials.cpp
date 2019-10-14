#include "materials.h"
#include <algorithm>
#ifdef _WIN32
#include <corecrt_math_defines.h>
#endif

Color LambertMaterial::getColor(const Collision& collision, const Light& light) const {
    auto interaction = light.castOnPoint(collision.point);
    return ((color * interaction.color) * Vector3f::dot(collision.normal, interaction.direction) * (1 - absorption)).restriction();
}

Color PhongMaterial::getColor(const Collision& collision, const Light& light) const {
    auto interaction = light.castOnPoint(collision.point);
    return ((color * interaction.color * Vector3f::dot(interaction.direction, collision.normal)).restriction() /*+ interaction.color * powf(Vector3f::dot(collision.normal, (-collision.pRay->direction + interaction.direction) / 2), shininess)).restriction();*/
            + (interaction.color * std::min(1.0f, shininess / 10) * powf(std::max(Vector3f::dot((2 * Vector3f::dot(collision.normal, interaction.direction) * collision.normal - interaction.direction), -collision.pRay->direction), 0.0f), shininess)).restriction());
}
