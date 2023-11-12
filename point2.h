#pragma once

#include <iostream>

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

            void add(Vector2 vector);
            Point2 added(Vector2 vector);
            void subtract(Vector2 vector);
            Point2 subtracted(Vector2 vector);

            float distanceTo(Point2 other);
            float squaredDistanceTo(Point2 other);

            bool operator ==(const Point2& other) const;
            bool operator !=(const Point2& other) const;

            friend std::ostream& operator <<(std::ostream& os, const Point2& pt) {
                return os << "Point2(" << pt.x << ", " << pt.y << ")";
            }
   };
}