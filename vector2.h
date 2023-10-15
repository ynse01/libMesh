#pragma once

#include <iostream>

namespace libMesh {
    struct Vector2 {
        public:
            Vector2();
            Vector2(float x, float y);
            Vector2(const Vector2& other);
            float x;
            float y;
            
            static Vector2 XAxis() { return Vector2(1.0f, 0.0f); }
            static Vector2 YAxis() { return Vector2(0.0f, 1.0f); }

            float dot(Vector2 other);

            void scale(float value);
            Vector2 scaled(float value);

            friend std::ostream& operator <<(std::ostream& os, const Vector2& v) {
                return os << "Vector2(" << v.x << ", " << v.y << ")";
            }
    };
}