#pragma once

namespace libMesh {
    struct Size2 {
        public:
            Size2(float width, float height) : width(width), height(height) {}
            float width;
            float height;

            Size2 scale(float scale) {
                return Size2(width * scale, height * scale);
            }
    };
}