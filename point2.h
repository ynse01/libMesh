#pragma once

#include "vector2.h"

namespace libMesh {
    struct Point2 {
        public:
            Point2();
            Point2(float x, float y);
            Point2(const Point2& other);
            float x;
            float y;
            
            static Vector2 betweenPoints(Point2 p0, Point2 p1);

            Point2 add(Vector2 vector);
            Point2 subtract(Vector2 vector);

            float distanceTo(Point2 other);
            float squaredDistanceTo(Point2 other);
   };
}