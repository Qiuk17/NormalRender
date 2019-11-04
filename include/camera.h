#ifndef _CAMERA_H
#define _CAMERA_H

#include <vecmath.h>
#include <cmath>
#include "ray.h"

class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up).normalized();
        this->up = Vector3f::cross(this->horizontal, this->direction).normalized();
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(int x, int y) const = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    virtual void glSetup() const;
protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
            const Vector3f &up, int imgW, int imgH, float angle) : Camera(center, direction, up, imgW, imgH) {
        rotateMatrix = Matrix3f(horizontal, -up, direction);
        fovyd = angle / M_PI * 180.0;
        cmosHalfHeight = tan(angle / 2); cmosHalfWidth = cmosHalfHeight * width / (float)height;
        pixelWidth = 2 * cmosHalfWidth / (float)imgW; pixelHeight = 2 * cmosHalfHeight / (float)imgH;
    }
    Ray generateRay(int x, int y) const override;
    void glSetup() const override;
private:
    float cmosHalfWidth, cmosHalfHeight, pixelWidth, pixelHeight, fovyd;
    Matrix3f rotateMatrix;
};

#endif
