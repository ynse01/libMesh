#pragma once

#include "vector.h"
#include "point.h"
#include "size2.h"

namespace libMesh {
    struct Plane {
        public:
            Plane(Point origin, Vector xAxis, Vector yAxis);
            Point origin;
            Vector xAxis;
            Vector yAxis;

            Point get(Size2 index);
    };
}