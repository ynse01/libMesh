#pragma once

#include "point2.h"
#include "point3.h"
#include "vector3.h"

namespace libMesh {
    struct Plane {
        public:
            Plane(Point3 origin, Vector3 xAxis, Vector3 yAxis);
            Point3 origin;
            Vector3 xAxis;
            Vector3 yAxis;

            Point3 get(Point2 index);
            Vector3 getNormal();
    };
}