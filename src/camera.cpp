#include "camera.h"

Ray PerspectiveCamera::generateRay(int x, int y) const {
    auto drc = Vector3f(-cmosHalfWidth + x * pixelWidth, -cmosHalfHeight + y * pixelHeight, 1).normalized();
    return Ray(center, rotateMatrix * drc);
}
