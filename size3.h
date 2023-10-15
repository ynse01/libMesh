#pragma once

#include <iostream>

namespace libMesh {
    struct Size3 {
        public:
            Size3(float width, float height, float depth)
             : width(width), height(height), depth(depth) {}
            Size3(const Size3& other) : Size3(other.width, other.height, other.depth) {}
            float width;
            float height;
            float depth;

            Size3 scale(float scale) {
                return Size3(width * scale, height * scale, depth * scale);
            }

            friend std::ostream& operator <<(std::ostream& os, const Size3& size) {
                return os << "Size3(" << size.width << ", " << size.height << ", " << size.depth << ")";
            }
    };
}