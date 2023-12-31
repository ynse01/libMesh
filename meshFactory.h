#pragma once

#include "mesh.h"
#include "point3.h"
#include "vector3.h"
#include "size2.h"
#include "size3.h"
#include "plane.h"

namespace libMesh {
    class MeshFactory {
        public:
            static Mesh *box(Point3 center, Size2 size, Vector3 xAxis, Vector3 yAxis, float length);
            static Mesh *cylinder(Point3 center, float diameter, Vector3 xAxis, Vector3 yAxis, float length, unsigned int numFaces = 40);
        private:
            static void makeFaceFromRectangle(std::vector<Vertex> &vertices, std::vector<Index3> &indices, Plane &plane, Size2 size);
            static void makeFaceFromParallelCurves(std::vector<Vertex> &vertices, std::vector<Index3> &indices, Point3 *left, Point3 *right, size_t size);
            static void makeFaceAroundCenter(std::vector<Vertex> &vertices, std::vector<Index3> &indices, Point3 center, Point3 *edge, size_t size);
    };
}