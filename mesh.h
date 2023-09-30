#pragma once

#include <vector>

#include "vector.h"
#include "point.h"

namespace libMesh {
    class Mesh {
        public:
            Mesh(std::vector<Point> vertices, std::vector<unsigned int> indices, std::vector<Vector> normals = std::vector<Vector>());
        
            void transform(Transformation t);
        private:
            std::vector<Point> vertices;
            std::vector<Vector> normals;
            std::vector<unsigned int> indices;
    };
}