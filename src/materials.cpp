#include "materials.h"
#include <algorithm>
#include <GL/glut.h>

#ifdef _WIN32
#include <corecrt_math_defines.h>
#endif

Color DiffMaterial::getColor(const Collision& collision, const Light& light) const {
    auto interaction = light.castOnPoint(collision.point);
    return ((color * interaction.color) * Vector3f::dot(collision.normal, interaction.direction) * (1 - absorption)).restriction();
}

Color PhongMaterial::getColor(const Collision& collision, const Light& light) const {
    auto interaction = light.castOnPoint(collision.point);
    return ((color * interaction.color * Vector3f::dot(interaction.direction, collision.normal)).restriction()
            + (interaction.color * std::min(1.0f, shininess / 10) * powf(std::max(Vector3f::dot((2 * Vector3f::dot(collision.normal, interaction.direction) * collision.normal - interaction.direction), -collision.pRay->direction), 0.0f), shininess)).restriction());
}

void DiffMaterial::glApply() const {
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Vector4f(color.toVec(), 1));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Vector4f(0, 0, 0, 1));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Vector2f(0, 1));
}

void PhongMaterial::glApply() const {
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Vector4f(color.toVec(), 1));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Vector4f(specColor.toVec(), 1));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Vector2f(shininess * 4.0, 1));
}
