#ifndef _COLLISION_H
#define _COLLISION_H

#include <cassert>
#include <vecmath.h>
#include "ray.h"

class Entity;
class Collision {
public:
    const Entity* pEntity;
    const Ray* pRay;
    Vector3f normal;
    Vector3f point;
    float distance;
    bool isValid;
    Collision(const Ray* pRay_, bool isValid_ = false) : pEntity(nullptr), pRay(pRay_), normal(), point(), distance(INFINITY), isValid(false) {assert(!isValid);}
    Collision(const Entity* pEntity_, const Ray* pRay_, const Vector3f& normal_, const Vector3f& point_, float distance_) :
        pEntity(pEntity_), pRay(pRay_), normal(normal_.normalized()), point(point_), distance(distance_), isValid(true) {}
};

#endif
