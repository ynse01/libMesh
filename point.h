#pragma once

#include "vector.h"

namespace libMesh {
    struct Point {
        public:
            Point();
            Point(float x, float y, float z);
            Point(Point& other);
            float x;
            float y;
            float z;

            void add(Vector& vector);
            void subtract(Vector& vector);

            float distanceTo(Point& other);
            float squaredDistanceTo(Point& other);
    };
}