#include <cmath>
#include <iostream>
#include <GL/glut.h>
#include "entities.h"

Collision Group::interact(const Ray& ray) const {
    return pDetector->getClosestCollision(ray);
}

void Group::glDraw() const {
    for (const auto p : vecEntityPtr) p->glDraw();
}

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

void Sphere::glDraw() const {
    Entity::glDraw();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(position.x(), position.y(), position.z());
    glutSolidSphere(radius, 80, 80);
    glPopMatrix();
}

Collision Plane::interact(const Ray& ray) const {
    float n_d = Vector3f::dot(normal, ray.direction);
    float t = - (d + Vector3f::dot(normal, ray.from)) / n_d;
    if (isnanf(t) || t <= 0) return Collision(&ray, false);
    return Collision(this, &ray, n_d < 0 ? normal : -normal, ray.pointAt(t), t);
}

void Plane::glDraw() const {
    Entity::glDraw();
    auto y = Vector3f::cross(normal, Vector3f::RIGHT);
    auto x = Vector3f::cross(y, normal);
    auto o = d * normal;
    const float size = 10.0f;
    x *= size, y *= size;
    glBegin(GL_TRIANGLES);
    glNormal3fv(normal);
    glVertex3fv(o + x + y);
    glVertex3fv(o - x - y);
    glVertex3fv(o + x - y);
    glNormal3fv(normal);
    glVertex3fv(o + x + y);
    glVertex3fv(o - x + y);
    glVertex3fv(o - x - y);
    glEnd();
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

void Triangle::glDraw() const {
    Entity::glDraw();
    glBegin(GL_TRIANGLES);
    glNormal3fv(normal);
    glVertex3fv(p[0]); glVertex3fv(p[1]); glVertex3fv(p[2]);
    glEnd();
}

Collision Mesh::interact(const Ray& ray) const {
    Collision ans(&ray, false);
    for (int i = 0; i < countTriangle; i++) {
        auto c = arrayTrianglePtr[i]->interact(ray);
        if (!c.isValid) continue;
        if (!ans.isValid) {ans = c; continue;}
        if (c.distance < ans.distance) {ans = c; continue;}
    }
    return ans;
}

void Mesh::glDraw() const {
    for (int i = 0; i < countTriangle; i++)
        arrayTrianglePtr[i]->glDraw();
}

Collision Transform::interact(const Ray& ray) const {
    auto col = pEntity->interact(Ray(transformPoint(transform, ray.from), transformDirection(transform, ray.direction)));
    if (!col.isValid) return Collision(&ray, false);
    return Collision(col.pEntity, &ray, transformDirection(transform.transposed(), col.normal).normalized(), ray.pointAt(col.distance), col.distance);
}

void Transform::glDraw() const {
    Entity::glDraw();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(transform.inverse());
    pEntity->glDraw();
    glPopMatrix();
}
