#pragma once

#include "vector3.h"
#include "point2.h"
#include "point3.h"

namespace libMesh {
    struct Vertex {
        public:
            Point3 position;
            Vector3 normal;
            Point2 texCoord;
    };
}