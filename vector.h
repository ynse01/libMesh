#pragma once

namespace libMesh {
    struct Vector {
        public:
            Vector();
            Vector(float x, float y, float z);
            float x;
            float y;
            float z;

            float dot(Vector& other);
            Vector cross(Vector& other);
    };
}