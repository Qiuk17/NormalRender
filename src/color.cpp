#include "color.h"

std::istream& operator>> (std::istream& is, Color& color) {
    float a, b, c;
    is >> a >> b >> c;
    color = Color(a, b, c);
    return is;
}
