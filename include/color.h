#ifndef _COLOR_H
#define _COLOR_H

#include <algorithm>
#include <iostream>
#include <cmath>
#include <Vector3f.h>

class Color {
	friend std::istream& operator>> (std::istream& is, Color& color);
public:
	float r, g, b;
	explicit Color (float gray = 0.0f) : r(gray), g(gray), b(gray) {restricted();}
	Color(float r, float g, float b) : r(r), g(g), b(b) {restricted();}
	Color (const Vector3f& c) : r(c.x()), g(c.y()), b(c.z()) {restricted();}
	Color& restricted() {r = std::max(std::min(r, 1.0f), 0.0f); g = std::max(std::min(g, 1.0f), 0.0f); b = std::max(std::min(b, 1.0f), 0.0f); return *this;}
	Color restriction() const { return Color(std::max(std::min(r, 1.0f), 0.0f), std::max(std::min(g, 1.0f), 0.0f), std::max(std::min(b, 1.0f), 0.0f)); }
	Color operator+ (const Color& c) const { return Color(r + c.r, g + c.g, b + c.b); }
	Color operator- (const Color& c) const { return Color(r - c.r, g - c.g, b - c.b); }
	Color operator* (const Color& c) const { return Color(r * c.r, g * c.g, b * c.b); }
	Color operator* (const float  f) const { return Color(r * f, g * f, b * f); }
	Color operator/ (const float  f) const { return this->operator*(1 / f); }

	Color& operator+= (const Color& c) { r += c.r; g += c.g; b += c.b; return *this; }
	Color& operator-= (const Color& c) { r -= c.r; g -= c.g; b -= c.b; return *this; }
	Color& operator*= (const Color& c) { r *= c.r; g *= c.g; b *= c.b; return *this; }
	Color& operator*= (const float  f) { r *= f; g *= f; b *= f; return *this;}
	Color& operator/= (const float  f) { return this->operator*=(1 / f); }

	Color exp() const { return Color(::exp(r), ::exp(g), ::exp(b)); }
	float strength() const { return (r + g + b) / 3; }

	Vector3f toVec() const { return Vector3f(r, g, b); }
};

#endif
