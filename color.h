#pragma once

#include <iostream>

namespace libMesh {
    struct Color {
        public:
            Color() : Color(1.0f, 1.0f, 1.0f, 1.0f) {}
            Color(float red, float green, float blue, float alpha = 1.0f): red(red), green(green), blue(blue), alpha(alpha) {}
            Color(const Color& other) : Color(other.red, other.green, other.blue, other.alpha) {}
            float red;
            float green;
            float blue;
            float alpha;

            Color& operator=(const Color& other) { 
                red = other.red;
                green = other.green;
                blue = other.blue;
                alpha = other.alpha;
                return *this;
            }

            friend std::ostream& operator <<(std::ostream& os, const Color& col) {
                return os << "RGBA(" << col.red << ", " << col.green << ", " << col.blue << ", " << col.alpha << ")";
            }
    };
}