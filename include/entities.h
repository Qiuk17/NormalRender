#ifndef _ENTITIES_H
#define _ENTITIES_H

#include "collision.h"
#include "materials.h"
#include "ray.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
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
        d = - Vector3f::dot(normal, position);
    }
    Collision interact(const Ray& ray) const override;
private:
    Vector3f normal;
    float d;
    float offset;
};

class Triangle : public Entity {
public:
    Triangle(const Vector3f& p1_, const Vector3f& p2_, const Vector3f& p3_, const Material* pMaterial_ = nullptr) :
        Entity(p1_, pMaterial_), normal(Vector3f::cross(p2_ - p1_, p3_ - p1_).normalized()) {
            d = -Vector3f::dot(normal, position);
            p[0] = p1_; p[1] = p2_; p[2] = p3_;
            flattenMode = ILLIGAL;
            if (std::abs(normal.z()) > 1e-4f) flattenMode = XY;
            else if (std::abs(normal.y()) > 1e-4f) flattenMode = XZ;
            else if (std::abs(normal.x()) > 1e-4f) flattenMode = YZ;
            std::cout << flattenMode;
            for (int i = 0; i < 3; i++) {
                switch (flattenMode) {
                    case ILLIGAL: return;
                    case XY: f[i] = p[i].xy(); break;
                    case XZ: f[i] = p[i].xz(); break;
                    case YZ: f[i] = p[i].yz(); break;
                }
            }
        }
    Collision interact(const Ray& ray) const override;
private:
    enum FlattenMode {ILLIGAL, XY, XZ, YZ};
    enum Quadrant {I, II, III, IV};
    Vector3f p[3];
    Vector3f normal;
    Vector2f f[3];
    FlattenMode flattenMode;
    float d;
    Vector2f flatten(const Vector3f& point) const {
        switch (flattenMode) {
            case ILLIGAL: return Vector2f();
            case XY: return point.xy();
            case XZ: return point.xz();
            case YZ: return point.yz();
        }
    }
    static Quadrant getQuadrant(const Vector2f& point) {
        if (point.x() >= 0) {
            if (point.y() >= 0) return I;
            else return IV;
        }
        else {
            if (point.y() >= 0) return II;
            else return III;
        }
    }
    static int getDeltaDegree(const Vector2f& pointFrom, const Vector2f& pointTo) {
        auto qFrom = getQuadrant(pointFrom), qTo = getQuadrant(pointTo);
        auto delta = qTo - qFrom;
        if (delta == 0) return 0;
        if (delta == 1 || delta == -3) return 1;
        if (delta == 2 || delta == -2)  {
            auto d = pointTo.y() * pointFrom.x() - pointTo.x() * pointFrom.y();
            if (d == 0) throw 0;
            return d > 0 ? 2 : -2;
        }
        if (delta == 3 || delta == -1) return -1;
    }
};

class Mesh : public Entity {
public:
    Mesh(const char* objPath, const Vector3f& position_ = Vector3f(), const Material* pMaterial_ = nullptr) : Entity(position_, pMaterial_) {
        std::ifstream objStream(objPath);
        std::cout << objStream.flags();
        if (!objStream.good())
            std::__throw_ios_failure("[Warning] Failed to open mesh file. Ignoring.");
        char flag;
        std::vector<Vector3f*> vertexs;
        std::vector<Triangle*> triangles;
        Vector3f* vp; Triangle* tp;
        int i1, i2, i3;
        while (objStream >> flag) {
            if (objStream.eof()) break;
            switch (flag) {
                case 'v': 
                    vp = new Vector3f();
                    objStream >> *vp;
                    vertexs.emplace_back(vp);
                    break;
                case 'f': 
                    objStream >> i1 >> i2 >> i3;
                    tp = new Triangle(*vertexs[i1 - 1], *vertexs[i2 - 1], *vertexs[i3 - 1], pMaterial_);
                    triangles.emplace_back(tp);
                    break;
                default: std::__throw_invalid_argument("Obj file is not valid.");
            }
        }
        arrayVertexPtr = new Vector3f*[vertexs.size()];
        arrayTrianglePtr = new Triangle*[triangles.size()];
        for (auto p: vertexs) arrayVertexPtr[countVertex++] = p;
        for (auto p: triangles) arrayTrianglePtr[countTriangle++] = p;
        std::cout << countTriangle << " faces\n";
    }
    ~Mesh() override {
        for (int i = 0; i < countVertex; i++) delete arrayVertexPtr[i];
        for (int i = 0; i < countTriangle; i++) delete arrayTrianglePtr[i];
        delete arrayVertexPtr; delete arrayTrianglePtr;
    }
    Collision interact(const Ray& ray) const override;
private:
    Vector3f**  arrayVertexPtr;
    int countVertex = 0;
    Triangle** arrayTrianglePtr;
    int countTriangle = 0;
};

// class Transform : public Entity {

// };

#endif
