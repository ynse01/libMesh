#pragma once

#include <cstddef>
#include <vector>

#include "vector3.h"
#include "point3.h"

namespace libMesh {
    class Mesh {
        public:
            Mesh(std::vector<Point3> vertices, std::vector<unsigned int> indices);
            Mesh(std::vector<Point3> vertices, std::vector<unsigned int> indices, std::vector<Vector3> normals);
        
            unsigned int getId() { return id; }

            size_t verticesCount() { return vertices.size(); }
            size_t trianglesCount() { return indices.size() / 3; }
            Point3 getVertex(unsigned int index) { return vertices[index];}
            unsigned int getIndex(unsigned int index) { return indices[index];}
            Vector3 getNormal(unsigned int index) { return normals[index];}
            const void * getVertexPtr() { return vertices.data();}
            const void * getIndexPtr() { return indices.data();}

            void transform(Transformation t);
        private:
            static unsigned int nextId;
            unsigned int id;
            std::vector<Point3> vertices;
            std::vector<Vector3> normals;
            std::vector<unsigned int> indices;
    };
}