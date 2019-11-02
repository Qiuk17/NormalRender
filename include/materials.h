#ifndef _MATERIALS_H
#define _MATERIALS_H

#include <vecmath.h>
#include "collision.h"
#include "color.h"
#include "light.h"

class Material {
public:
    virtual Color getColor(const Collision& collision, const Light& light) const = 0;
    virtual void glApply() const = 0;
};

class DiffMaterial : public Material {
public:
    DiffMaterial(const Color& color_, float absorption_ = 0.0f) : color(color_), absorption(absorption_) {}
    Color getColor(const Collision& collision, const Light& light) const override;
    void glApply() const override;
private:
    Color color;
    float absorption;
};

class PhongMaterial : public Material {
public:
    PhongMaterial(const Color& color_, float shininess_) : color(color_), shininess(shininess_) {}
    PhongMaterial(const Color& color_, const Color& specColor_, float shininess_) : color(color_), specColor(specColor_), shininess(shininess_) {}
    Color getColor(const Collision& collision, const Light& light) const override;
    void glApply() const override;
private:
    Color color;
    Color specColor;
    float shininess;
};

#endif
