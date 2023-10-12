#pragma once

#include "mesh.h"
#include "size2.h"

namespace libMesh {
    class MeshExtruder {
        public:
            static Mesh *box(Point center, Size2 size, Vector xAxis, Vector yAxis, float length);
            static Mesh *cylinder(Point center, float diameter, Vector xAxis, Vector yAxis, float length);
    };
}