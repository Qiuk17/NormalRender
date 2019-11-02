#include "detector.h"
#include "entities.h"

bool Detector::isRayCollided(const Ray& ray, float largerThan, float smallerThan) const {
    return getAnyCollision(ray, largerThan, smallerThan).isValid;
}

void BruteForceDetector::prepareDetector(const std::vector<Entity*>& entities) {
    pEntities = new Entity*[entities.size()];
    for (auto p : entities) {
        pEntities[countEntity++] = p;
    }
}

Collision BruteForceDetector::getClosestCollision(const Ray& ray) const {
    auto res = Collision(&ray, false);
    for (int i = 0; i < countEntity; i++) {
        auto n = pEntities[i]->interact(ray);
        if (!n.isValid) continue;
        if (!res.isValid) {res = n; continue;}
        res = res.distance > n.distance ? n : res;
    }
    return res;
}

Collision BruteForceDetector::getAnyCollision(const Ray& ray, float largerThan, float smallerThan) const {
    for (int i = 0; i < countEntity; i++) {
        auto n = pEntities[i]->interact(ray);
        if (n.isValid && n.distance > largerThan && n.distance < smallerThan) return n;
    }
    return Collision(&ray, false);
}
