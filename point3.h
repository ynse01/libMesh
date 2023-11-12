#pragma once

#include <iostream>

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

            void add(Vector3 vector);
            Point3 added(Vector3 vector);
            void subtract(Vector3 vector);
            Point3 subtracted(Vector3 vector);

            float distanceTo(Point3 other);
            float squaredDistanceTo(Point3 other);

            void transform(Transformation t);
            Point3 transformed(Transformation t);

            bool operator ==(const Point3& other) const;
            bool operator !=(const Point3& other) const;

            friend std::ostream& operator <<(std::ostream& os, const Point3& pt) {
                return os << "Point3(" << pt.x << ", " << pt.y << ", " << pt.z << ")";
            }
    };
}