#pragma once

#include "transformation.h"

namespace libMesh {
    struct Vector {
        public:
            Vector();
            Vector(float x, float y, float z);
            float x;
            float y;
            float z;

            static Vector XAxis() { return Vector(1.0f, 0.0f, 0.0f); }
            static Vector YAxis() { return Vector(0.0f, 1.0f, 0.0f); }
            static Vector ZAxis() { return Vector(0.0f, 0.0f, 1.0f); }

            float dot(Vector other);
            Vector cross(Vector other);

            Vector transform(Transformation t);
            Vector scale(float value);
    };
}