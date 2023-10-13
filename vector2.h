#pragma once

namespace libMesh {
    struct Vector2 {
        public:
            Vector2();
            Vector2(float x, float y);
            float x;
            float y;
            
            static Vector2 XAxis() { return Vector2(1.0f, 0.0f); }
            static Vector2 YAxis() { return Vector2(0.0f, 1.0f); }

            float dot(Vector2 other);

            Vector2 scale(float value);
    };
}