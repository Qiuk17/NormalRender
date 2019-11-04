#include "camera.h"
#include <GL/glut.h>

Ray PerspectiveCamera::generateRay(int x, int y) const {
    auto drc = Vector3f(-cmosHalfWidth + x * pixelWidth, -cmosHalfHeight + y * pixelHeight, 1).normalized();
    return Ray(center, rotateMatrix * drc);
}

void Camera::glSetup() const {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(center.x(), center.y(), center.z(),
              center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
              up.x(), up.y(), up.z());
}

void PerspectiveCamera::glSetup() const {
    Camera::glSetup();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovyd, pixelWidth / (double)pixelHeight, 0.01, 100.0);
}
