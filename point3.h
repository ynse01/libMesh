#pragma once

#include "vector3.h"

namespace libMesh {
    struct Point3 {
        public:
            Point3();
            Point3(float x, float y, float z);
            Point3(const Point3& other);
            float x;
            float y;
            float z;

            static Vector3 betweenPoints(Point3 p0, Point3 p1);

            Point3 add(Vector3 vector);
            Point3 subtract(Vector3 vector);

            float distanceTo(Point3 other);
            float squaredDistanceTo(Point3 other);

            Point3 transform(Transformation t);
    };
}