#include <cmath>
#include "entities.h"

Collision Sphere::interact(const Ray& ray) const {
    auto a = ray.direction.squaredLength();
    auto delta = position - ray.from;
    auto b = -2 * Vector3f::dot(ray.direction, delta);
    auto c = delta.squaredLength() - radius * radius;
	auto d = b * b - 4 * a * c;
    if (d < 0) return Collision(&ray);
    auto sd = sqrt(d);
    auto greater = -b + sd;
    if (greater <= 0) return Collision(&ray);
    auto less = -b - sd;
    auto dis = (less <= 0 ? greater : less) / (2 * a);
    auto collisionPoint = ray.pointAt(dis);
    return Collision(this, &ray, less > 0 ? collisionPoint - position : position - collisionPoint, collisionPoint, dis);
}
