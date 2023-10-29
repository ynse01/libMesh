#pragma once

#include "../mesh.h"

namespace libRenderer {
    struct VertexBufferHandle {
        unsigned int vertexArrayObject;
        unsigned int vertexBufferObject;
        unsigned int elementBufferObject;
        libMesh::Mesh* mesh;
    };

    class Renderer {
        public:
            bool start();
            void close();
            void run();

            void addMesh(libMesh::Mesh *mesh);
            void removeMesh(libMesh::Mesh *mesh);
        private:
            void *window;
            unsigned int shaderProgram;
            std::vector<VertexBufferHandle> vertexArrays = std::vector<VertexBufferHandle>();
    };
}