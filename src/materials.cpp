#include "materials.h"

#ifdef _WIN32
#include <corecrt_math_defines.h>
#endif

Color LambertMaterial::getColor(const Collision& collision, const Light& light) const {
    auto interaction = light.castOnPoint(collision.point);
    return ((color * interaction.color) * Vector3f::dot(collision.normal, interaction.direction) * M_1_PI * (1 - absorption)).restriction();
}

Color PhongMaterial::getColor(const Collision& collision, const Light& light) const {
    auto interaction = light.castOnPoint(collision.point);
    return (color * interaction.color * Vector3f::dot(interaction.direction, collision.normal)).restriction() 
            + (interaction.color * powf(Vector3f::dot((2 * Vector3f::dot(collision.normal, interaction.direction) * collision.normal - interaction.direction), -collision.pRay->direction), shininess)).restriction();
}
