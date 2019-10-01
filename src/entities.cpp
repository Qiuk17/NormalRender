#include <cmath>
#include <iostream>
#include "entities.h"

Collision Sphere::interact(const Ray& ray) const {
    auto delta = position - ray.from;
    bool inside = delta.squaredLength() < radius_2;
    float D = Vector3f::dot(delta, ray.direction);
    if (!inside && D <= 0) return Collision(&ray, false);
    float h_2 = delta.squaredLength() - D * D;
    if (h_2 > radius_2) return Collision(&ray, false);
    float d = inside ? D + sqrt(radius_2 - h_2) : D - sqrt(radius_2 - h_2);
    auto cpoint = ray.pointAt(d);
    return Collision(this, &ray, inside ? position - cpoint : cpoint - position, cpoint, d);
}
