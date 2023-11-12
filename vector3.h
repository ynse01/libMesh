#pragma once

#include <iostream>

#include "transformation.h"

namespace libMesh {
    struct Vector3 {
        public:
            Vector3();
            Vector3(float x, float y, float z);
            Vector3(const Vector3& other);
            float x;
            float y;
            float z;

            static Vector3 XAxis;
            static Vector3 YAxis;
            static Vector3 ZAxis;

            float dot(Vector3 other);
            Vector3 cross(Vector3 other);

            void transform(Transformation t);
            Vector3 transformed(Transformation t);
            void scale(float value);
            Vector3 scaled(float value);

            bool operator ==(const Vector3& other) const;
            bool operator !=(const Vector3& other) const;

            friend std::ostream& operator <<(std::ostream& os, const Vector3& v) {
                return os << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
            }
    };
}