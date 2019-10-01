#ifndef _DETECTOR_H
#define _DETECTOR_H

#include <vector>

#include "ray.h"
#include "entities.h"
#include "collision.h"

class Detector {
public:
    virtual ~Detector() {}
    virtual void prepareDetector(const std::vector<Entity*>& entities) = 0;
    virtual Collision getClosestCollision(const Ray& ray) const = 0;
    virtual Collision getAnyCollision(const Ray& ray, float largerThan = -1.0f, float smallerThan = 1e34) const = 0;
    virtual bool isRayCollided(const Ray& ray, float largerThan = -1.0f, float smallerThan = 1e34) const;
};

class BruteForceDetector : public Detector {
public:
    ~BruteForceDetector() override { delete pEntities; }
    void prepareDetector(const std::vector<Entity*>& entities) override;
    Collision getClosestCollision(const Ray& ray) const override;
    Collision getAnyCollision(const Ray& ray, float largerThan = -1.0f, float smallerThan = 1e34) const override;
private:
    Entity** pEntities = nullptr;
    int countEntity = 0;
};

#endif
