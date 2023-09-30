#pragma once

#include <vector>

#include "vector.h"
#include "point.h"

namespace libMesh {
    class Mesh {
        public:
        private:
            std::vector<Point> vertices;
            std::vector<Vector> normals;
            std::vector<unsigned int> indices;
    };
}