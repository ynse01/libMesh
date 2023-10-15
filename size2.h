#pragma once

#include <iostream>

namespace libMesh {
    struct Size2 {
        public:
            Size2(float width, float height) : width(width), height(height) {}
            Size2(const Size2& other) : Size2(other.width, other.height) {}
            float width;
            float height;

            Size2 scale(float scale) {
                return Size2(width * scale, height * scale);
            }

            friend std::ostream& operator <<(std::ostream& os, const Size2& size) {
                return os << "Size2(" << size.width << ", " << size.height << ")";
            }
    };
}