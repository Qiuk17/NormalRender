#ifndef _RAY_H
#define _RAY_H

#include <vecmath.h>

class Ray {
public:
	Vector3f from, direction;
    Ray() = delete;
    Ray(const Vector3f& from_, const Vector3f& direction_) : from(from_), direction(direction_.normalized()) {}
	Ray(const Vector3f& from_, const Vector3f& to_, bool from_point_to_point) : from(from_), direction((to_ - from_).normalized()) {}
    Vector3f pointAt(float t) const {return from + t * direction;}
};

#endif
