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

Collision Plain::interact(const Ray& ray) const {
    float n_d = Vector3f::dot(normal, ray.direction);
    float t = - (d + Vector3f::dot(normal, ray.from)) / n_d;
    if (isnanf(t) || t <= 0) return Collision(&ray, false);
    return Collision(this, &ray, n_d < 0 ? normal : -normal, ray.pointAt(t), t);
}

Collision Triangle::interact(const Ray& ray) const {
    float n_d = Vector3f::dot(normal, ray.direction);
    float t = - (d + Vector3f::dot(normal, ray.from)) / n_d;
    if (isnanf(t) || t <= 0) return Collision(&ray, false);
    auto cpoint = ray.pointAt(t);
    auto fcpoint = flatten(cpoint);
    auto delta = 0;
    for (int i = 0; i < 3; i++) {
        try {
            delta += getDeltaDegree(f[i] - fcpoint, f[(i + 1) % 3] - fcpoint);
        } catch (int& e) {
            delta = 2; break;
        }
    }
    if (delta == 0) return Collision(&ray, false);
    return Collision(this, &ray, n_d < 0 ? normal : -normal, cpoint, t);
}
