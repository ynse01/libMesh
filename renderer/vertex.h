#pragma once

#include "../vector3.h"
#include "../point2.h"
#include "../point3.h"

namespace libRenderer {
    struct Vertex {
        public:
            libMesh::Point3 position;
            libMesh::Vector3 normal;
            libMesh::Point2 texCoord;
    };
}