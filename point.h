#pragma once

#include "vector.h"

namespace libMesh {
    struct Point {
        public:
            Point();
            Point(float x, float y, float z);
            Point(const Point& other);
            float x;
            float y;
            float z;

            Point add(Vector& vector);
            Point subtract(Vector& vector);

            float distanceTo(Point& other);
            float squaredDistanceTo(Point& other);

            Point transform(Transformation t);
    };
}