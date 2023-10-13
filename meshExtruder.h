#pragma once

#include "mesh.h"
#include "point3.h"
#include "vector3.h"
#include "size2.h"

namespace libMesh {
    class MeshExtruder {
        public:
            static Mesh *box(Point3 center, Size2 size, Vector3 xAxis, Vector3 yAxis, float length);
            static Mesh *cylinder(Point3 center, float diameter, Vector3 xAxis, Vector3 yAxis, float length, int numFaces = 40);
    };
}