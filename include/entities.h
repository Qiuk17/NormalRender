#ifndef _ENTITIES_H
#define _ENTITIES_H

#include "collision.h"
#include "detector.h"
#include "materials.h"
#include "ray.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <fstream>
#include <functional>
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
    virtual void glDraw() const { if (pMaterial) pMaterial->glApply(); };
    const Vector3f& getPosition() const {return position;}
    //virtual BoundingBox getBoundingBox() const = 0;
    virtual const Material* getMaterial() const { return pMaterial; }
protected:
    Vector3f position;
    const Material* pMaterial;
};

class Group : public Entity {
public:
    Group(const std::vector<Entity*>& entities, Detector* detector = new BruteForceDetector()) : Entity(0, nullptr), pDetector(detector) {
        for (auto i : entities) vecEntityPtr.emplace_back(i);
        pDetector->prepareDetector(vecEntityPtr);
    }
    ~Group() {
        for (auto i : vecEntityPtr) delete i;
        delete pDetector;
    }
    Entity* getEntity(int index) { return vecEntityPtr[index]; }
    Collision interact(const Ray& ray) const override;
    void glDraw() const override;
    const Detector* getDetector() const { return pDetector; }
private:
    std::vector<Entity*> vecEntityPtr;
    Detector* pDetector;
};

class Sphere : public Entity {
public:
    Sphere(const Vector3f& position_
         , float radius_
         , const Material* pMaterial_ = nullptr) 
         : Entity(position_, pMaterial_)
         , radius(radius_)
         , radius_2(radius_ * radius_) {}
    Collision interact(const Ray& ray) const override;
    void glDraw() const override;
private:
    float radius, radius_2;
};

class Plane : public Entity {
public:
    Plane(const Vector3f& normal_
        , float offset_
        , const Material* pMaterial_ = nullptr) 
        : normal(normal_)
        , offset(offset_)
        , Entity(Vector3f() + offset_ * normal_, pMaterial_) {
        d = - Vector3f::dot(normal, position);
    }
    Collision interact(const Ray& ray) const override;
    void glDraw() const override;
private:
    Vector3f normal;
    float d;
    float offset;
};

class Triangle : public Entity {
public:
    Triangle(const Vector3f& p1_
           , const Vector3f& p2_
           , const Vector3f& p3_
           , const Material* pMaterial_ = nullptr) 
           : Entity(p1_, pMaterial_)
           , normal(Vector3f::cross(p2_ - p1_, p3_ - p1_).normalized()) {
            d = -Vector3f::dot(normal, position);
            p[0] = p1_; p[1] = p2_; p[2] = p3_;
            flattenMode = ILLIGAL;
            if (std::abs(normal.z()) > 1e-4f) flattenMode = XY;
            else if (std::abs(normal.y()) > 1e-4f) flattenMode = XZ;
            else if (std::abs(normal.x()) > 1e-4f) flattenMode = YZ;
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
    void glDraw() const override;
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
    Mesh(const char* objPath
       , const Vector3f& position_ = Vector3f()
       , const Material* pMaterial_ = nullptr) 
       : Entity(position_, pMaterial_) {
        std::ifstream objStream(objPath);
        if (!objStream.good()) {
            char buf[400];
            sprintf(buf, "[Warning] Failed to open \"%s\". Ignoring.", objPath);
            std::__throw_ios_failure(buf);
        }
        char flag;
        std::vector<Vector3f*> vertexs;
        std::vector<Triangle*> triangles;
        std::vector<std::tuple<int, int, int>*> ids;
        Vector3f* vp; Triangle* tp; std::tuple<int, int, int>* ip;
        int i1, i2, i3;
        while (objStream >> flag) {
            if (objStream.eof()) break;
            switch (flag) {
                case 'v': 
                    vp = new Vector3f();
                    objStream >> *vp;
                    *vp += position;
                    vertexs.emplace_back(vp);
                    break;
                case 'f': 
                    objStream >> i1 >> i2 >> i3;
                    tp = new Triangle(*vertexs[i1 - 1], *vertexs[i2 - 1], *vertexs[i3 - 1], pMaterial_);
                    ip = new std::tuple<int, int, int>(i1 - 1, i2 - 1, i3 - 1);
                    triangles.emplace_back(tp);
                    ids.emplace_back(ip);
                    break;
                default: std::__throw_invalid_argument("Obj file is not valid.");
            }
        }
        arrayVertexPtr = new Vector3f*[vertexs.size()];
        arrayTrianglePtr = new Triangle*[triangles.size()];
        arrayTriangleIdPtr = new std::tuple<int, int, int>*[ids.size()];
        for (auto p: vertexs) arrayVertexPtr[countVertex++] = p;
        for (auto p: triangles) arrayTrianglePtr[countTriangle++] = p;
        countTriangle = 0;
        for (auto p: ids) arrayTriangleIdPtr[countTriangle++] = p;
    }
    ~Mesh() override {
        for (int i = 0; i < countVertex; i++) delete arrayVertexPtr[i];
        for (int i = 0; i < countTriangle; i++) delete arrayTrianglePtr[i], arrayTriangleIdPtr[i];
        delete arrayVertexPtr; delete arrayTrianglePtr; delete arrayTriangleIdPtr;
    }
    Collision interact(const Ray& ray) const override;
    void glDraw() const override;
protected:
    Vector3f**  arrayVertexPtr;
    int countVertex = 0;
    Triangle** arrayTrianglePtr;
    std::tuple<int, int, int>** arrayTriangleIdPtr;
    int countTriangle = 0;
};

class Transform : public Entity {
public:
    Transform(Entity* pEntity_, const Matrix4f& transform_)
        : pEntity(pEntity_),
        transform(transform_.inverse()),
        Entity(pEntity_->getPosition(), nullptr) {}
    Collision interact(const Ray& ray) const override;
    void glDraw() const override;
private:
    Entity* pEntity;
    Matrix4f transform;

    static Vector3f transformPoint(const Matrix4f &mat, const Vector3f &point) {
        return (mat * Vector4f(point, 1)).xyz();
    }
    static Vector3f transformDirection(const Matrix4f &mat, const Vector3f &dir) {
        return (mat * Vector4f(dir, 0)).xyz();
    }
};

class RevCurveSurface;
class Curve : public Entity {
    friend class RevCurveSurface;
public:
    Curve(std::vector<Vector3f>* pControls_
        , int resolution_
        , int k_
        , bool isBSpline_) 
        : Entity(0, nullptr)
        , pControls(pControls_)
        , resolution(resolution_)
        , isBSpline(isBSpline_)
        , k(k_) {
        n = pControls->size() - 1;
        if (!isBSpline && k != n) std::__throw_logic_error("[Error] Invalid brezier curve.");
        for (int i = 0; i <= n + k + 1; i++) knot[i] = i / (float)(n + k + 1);
        pSamplePoints = new Vector3f[resolution + 1];
        pSamplePointsNormal = new Vector3f[resolution + 1];
        for (int sampleId = 0; sampleId <= resolution; sampleId++) {
            float t = isBSpline ? knot[k] + (sampleId / (float)resolution) * knot[n - k + 1] : (sampleId / (float)resolution);
            pSamplePoints[sampleId] = Vector3f(0);
            Vector3f& point = pSamplePoints[sampleId];
            Vector3f& normal = pSamplePointsNormal[sampleId];
            Vector3f before(0), after(0);
            float eps = 1e-4;
            for (int i = 0; i <= n; i++) {
                auto& p = pControls->at(i);
                point += p * b(i, k, t);
                before += p * b(i, k, t - eps);
                after += p * b(i, k, t + eps);
            }
            normal = (after - before);
            //for (int i = 0; i < n ; i++) normal += (pControls->at(i + 1) - pControls->at(i)) * b(i, k - 1, t);
            //normal = Vector3f(-normal.y(), normal.x(), 0);
            normal.normalize();
        }
    }
    ~Curve() override {
        delete pControls;
        delete pSamplePoints, pSamplePointsNormal;
    }
    Collision interact(const Ray& ray) const override { std::__throw_runtime_error("[Error] Curve cannot interact with ray."); }
    void glDraw() const override;
protected:
    std::vector<Vector3f>* pControls = nullptr;
    bool isBSpline;
    int n, k;
    float knot[200];
    int resolution;
    Vector3f* pSamplePoints = nullptr;
    Vector3f* pSamplePointsNormal = nullptr;

    float b(int i, int p, float t) {
        if (!isBSpline) {
            float cip = 1;
            for (int m = i + 1; m <= p; m++) cip *= m;
            for (int m = 2; m <= p - i; m++) cip /= m;
            for (int m = 0; m < p - i; m++) cip *= 1 - t;
            for (int m = 0; m < i; m++) cip *= t;
            return cip;
        }
        if (p == 0) return t >= knot[i] && t < knot[i+1] ? 1.0f : 0.0f;
        return (t - knot[i]) / knot[p] * b(i, p-1, t) +
               (knot[i + p + 1] - t) / knot[p] * b(i+1, p-1, t);
    }
};

class RevCurveSurface : public Entity {
public:
    RevCurveSurface(Curve* pCurve_
                  , Material* pMaterial_)
                  : Entity(0, pMaterial_)
                  , pCurve(pCurve_) {
        for (auto& p : *(pCurve->pControls))
            if (p.z() != 0.0f) std::__throw_logic_error("[Error] For RevSurface, z must be 0.");
        const int resolution = 40;
        for(int ci = 0; ci <= pCurve->resolution; ci++) {
            const auto& v = pCurve->pSamplePoints[ci];
            const auto& n = pCurve->pSamplePointsNormal[ci];
            for (int i = 0; i < resolution; i++) {
                float t = i / (float)resolution;
                Quat4f rot;
                rot.setAxisAngle(t * 2 * M_PI, Vector3f::UP);
                Vector3f pnew = Matrix3f::rotation(rot) * v;
                Vector3f pNormal = Vector3f::cross(n, -Vector3f::FORWARD);
                Vector3f nnew = Matrix3f::rotation(rot) * pNormal;
                VV.emplace_back(pnew);
                VN.emplace_back(nnew);
                int i1 = (i + 1 == resolution) ? 0 : i + 1;
                if (ci != pCurve->resolution) {
                    VF.emplace_back((ci + 1) * resolution + i, ci * resolution + i1, ci * resolution + i);
                    VF.emplace_back((ci + 1) * resolution + i, (ci + 1) * resolution + i1, ci * resolution + i1);
                }
            }
        }
    }
    ~RevCurveSurface() { delete pCurve; }
    Collision interact(const Ray& ray) const override { std::__throw_logic_error("[Error] RevSurface cannot interact with ray."); }
    void glDraw() const override;
private:
    Curve* pCurve;
    std::vector<Vector3f> VV;
    std::vector<Vector3f> VN;
    std::vector<std::tuple<unsigned, unsigned, unsigned>> VF;
};

class FfdMesh : public Mesh {
public:
    FfdMesh(const char* objPath
          , unsigned int resX, unsigned int resY, unsigned resZ
          , const Vector3f& position_ = Vector3f()
          , const Material* pMaterial_ = nullptr)
          : Mesh (objPath, position_, pMaterial_)
          , resX(resX), resY(resY), resZ(resZ)
          , countControls((resX + 1) * (resY + 1) * (resZ + 1)) {
              auto boundingBoxMin = -Vector3f::INF;
              auto boundingBoxMax =  Vector3f::INF;
              for (int i = 0; i < countVertex; i++) {
                  boundingBoxMax.extendMax(*arrayVertexPtr[i]);
                  boundingBoxMin.extendMin(*arrayVertexPtr[i]);
              }
              auto dim = boundingBoxMax - boundingBoxMin;
              arrayControls = new Vector3f[(resX + 1) * (resY + 1) * (resZ + 1)];
              arrayParas = new Vector3f[countVertex];
              for (int x = 0; x <= resX; x++)
                for (int y = 0; y <= resY; y++)
                    for (int z = 0; z <= resZ; z++) {
                        arrayControls[x * resY * resZ + y * resZ + z] = 
                            Vector3f(boundingBoxMin.x() + dim.x() * x / resX
                                   , boundingBoxMin.y() + dim.y() * y / resY
                                   , boundingBoxMin.z() + dim.z() * z / resZ);
                    }
              for (int i = 0; i < countVertex; i++) {
                  const auto& v = *arrayVertexPtr[i];
                  arrayParas[i] = Vector3f((v.x() - boundingBoxMin.x()) / dim.x()
                                         , (v.y() - boundingBoxMin.y()) / dim.y()
                                         , (v.z() - boundingBoxMin.z()) / dim.z());
              }
          }

    ~FfdMesh() {
        delete arrayControls, arrayParas;
    }

    void edit(const std::vector<Vector3f>& controls) {
        for (int i = 0; i < countControls; i++) arrayControls[i] = controls[i];
        for (int i = 0; i < countVertex; i++) {
            auto& v = *arrayVertexPtr[i], &parameter = arrayParas[i];
            v = Vector3f(0, 0, 0);
            for (int x = 0; x <= resX; x++)
                for (int y = 0; y <= resY; y++)
                    for (int z = 0; z <= resZ; z++) {
                        v += arrayControls[x * resY * resZ + y * resZ + z] * base(x, resX, parameter.x()) * base(y, resY, parameter.y()) * base(z, resZ, parameter.z());
                    }
        }
        for (int i = 0; i < countTriangle; i++) {
            auto& triangle = *arrayTrianglePtr[i];
            auto& id = *arrayTriangleIdPtr[i];
            triangle = Triangle(*arrayVertexPtr[std::get<0>(id)], *arrayVertexPtr[std::get<1>(id)], *arrayVertexPtr[std::get<2>(id)]);
        }
    }
    void glDraw() const override;

private:
    unsigned int resX, resY, resZ, countControls;
    Vector3f* arrayControls;
    Vector3f* arrayParas;

    static float base(int i, int p, float t) {
        float cip = 0;
        for (int m = i + 1; m <= p; m++) cip *= m;
        for (int m = 2; m <= p - i; m++) cip /= m;
        for (int m = 0; m < p - i; m++) cip *= 1 - t;
        for (int m = 0; m < i; m++) cip *= t;
        return cip;
    }

};

#endif
