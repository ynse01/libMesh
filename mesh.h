#pragma once

#include <cstddef>
#include <vector>

#include "vector.h"
#include "point.h"

namespace libMesh {
    class Mesh {
        public:
            Mesh(std::vector<Point> vertices, std::vector<unsigned int> indices);
            Mesh(std::vector<Point> vertices, std::vector<unsigned int> indices, std::vector<Vector> normals);
        
            size_t verticesCount() { return vertices.size(); }
            size_t trianglesCount() { return indices.size() / 3; }
            Point getVertex(unsigned int index) { return vertices[index];}
            unsigned int getIndex(unsigned int index) { return indices[index];}
            Vector getNormal(unsigned int index) { return normals[index];}

            void transform(Transformation t);
        private:
            std::vector<Point> vertices;
            std::vector<Vector> normals;
            std::vector<unsigned int> indices;
    };
}