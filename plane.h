#pragma once

#include "vector3.h"
#include "point3.h"
#include "size2.h"

namespace libMesh {
    struct Plane {
        public:
            Plane(Point3 origin, Vector3 xAxis, Vector3 yAxis);
            Point3 origin;
            Vector3 xAxis;
            Vector3 yAxis;

            Point3 get(Size2 index);
            Vector3 getNormal();
    };
}