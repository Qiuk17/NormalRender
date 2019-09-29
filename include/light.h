#ifndef _LIGHT_H
#define _LIGHT_H

#include <Vector3f.h>
#include "color.h"

class Light;
class LightInteraction {
public:
    LightInteraction(const Light* pLight_, const Vector3f& direction_, const Color& color_) : 
        pLight(pLight_), direction(direction_.normalized()), color(color_) {}
    const Light* pLight;
    Vector3f direction;
    Color color;
};

class Light {
public:
    Light(const Color& color_, float intensity_ = 1.0f) : color(color_), intensity(intensity_) {}
    virtual LightInteraction castOnPoint(const Vector3f& point) const = 0;
protected:
    Color color;
    float intensity;
};

class PointLight : public Light {
public:
    PointLight(const Vector3f& position_, const Color& color_, float intensity_ = 1.0f) :
        position(position_), Light(color_, intensity_) {}
    LightInteraction castOnPoint(const Vector3f& point) const override;
private:
    Vector3f position;
};

class DirectionalLight : public Light {
public:
    DirectionalLight(const Vector3f& direction_, const Color& color_, float intensity_ = 1.0f) :
        direction(direction_.normalized()), Light(color_, intensity_) {}
    LightInteraction castOnPoint(const Vector3f& point) const override;
private:
    Vector3f direction;
};

#endif
