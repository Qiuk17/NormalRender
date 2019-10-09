#ifndef _ENTITIES_H
#define _ENTITIES_H

#include "collision.h"
#include "materials.h"
#include "ray.h"

class BoundingBox {
public:
    Vector3f min;
    Vector3f max;
};

class Entity {
public:
    Entity(const Vector3f& position_, const Material* pMaterial_ = nullptr) : position(position_), pMaterial(pMaterial_) {}
    virtual ~Entity() = default;
    virtual Collision interact(const Ray& ray) const = 0;
    //virtual BoundingBox getBoundingBox() const = 0;
    const Material* getMaterial() const { return pMaterial; }
protected:
    Vector3f position;
    const Material* pMaterial;
};

class Sphere : public Entity {
public:
    Sphere(const Vector3f& position_, float radius_, const Material* pMaterial_ = nullptr) : Entity(position_, pMaterial_), radius(radius_), radius_2(radius_ * radius_) {}
    Collision interact(const Ray& ray) const override;
private:
    float radius, radius_2;
};

class Plain : public Entity {
public:
    Plain(const Vector3f& normal_, float offset_, const Material* pMaterial_ = nullptr) : normal(normal_), offset(offset_), Entity(Vector3f() + offset_ * normal_, pMaterial_) {
        origin = - Vector3f::dot(normal, position);
    }
    Collision interact(const Ray& ray) const override;
private:
    Vector3f normal;
    Vector3f origin;
    float offset;
};

// class Triangle : public Entity {
// public:
//     Triangle(const Vector3f& p1_, const Vector3f& p2_, const Vector3f& p3_, const Material* pMaterial_ = nullptr) :
//         p1(p1_), p2(p2_), p3(p3_), Entity(p1_, pMaterial_), normal(Vector3f::cross(p2_ - p1_, p3_ - p1_).normalized()) {}
//     Collision interact(const Ray& ray) const override;
// private:
//     Vector3f p1, p2, p3;
//     Vector3f normal;
// };

// class Mesh : public Entity {
// public:
//     Mesh(const char* objPath, const Vector3f& position_ = Vector3f(), const Material* pMaterial_ = nullptr) : Entity(position_, pMaterial_) {

//     }
//     ~Mesh() override {

//     }
// private:
//     std::vector<Vector3f> vertexArray;
//     std::vector<Triangle*> trianglePointerArray;
// };

// class Transform : public Entity {

// };

#endif
