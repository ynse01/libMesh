#pragma once

#include "transformation.h"

namespace libMesh {
    struct Vector3 {
        public:
            Vector3();
            Vector3(float x, float y, float z);
            float x;
            float y;
            float z;

            static Vector3 XAxis() { return Vector3(1.0f, 0.0f, 0.0f); }
            static Vector3 YAxis() { return Vector3(0.0f, 1.0f, 0.0f); }
            static Vector3 ZAxis() { return Vector3(0.0f, 0.0f, 1.0f); }

            float dot(Vector3 other);
            Vector3 cross(Vector3 other);

            Vector3 transform(Transformation t);
            Vector3 scale(float value);
    };
}