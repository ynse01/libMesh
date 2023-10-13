#pragma once

#include <cstddef>
#include <vector>

#include "vertex.h"
#include "index3.h"

namespace libMesh {
    class Mesh {
        public:
            Mesh(std::vector<Vertex> vertices, std::vector<Index3> indices);
        
            unsigned int getId() { return id; }

            size_t verticesCount() { return vertices.size(); }
            size_t trianglesCount() { return indices.size(); }
            Vertex getVertex(unsigned int index) { return vertices[index];}
            Index3 getTriangle(unsigned int index) { return indices[index];}

            const void * getVertexPtr() { return vertices.data();}
            const void * getIndexPtr() { return indices.data();}

            void transform(Transformation t);
        private:
            static unsigned int nextId;
            unsigned int id;
            std::vector<Vertex> vertices;
            std::vector<Index3> indices;
    };
}