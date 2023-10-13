#pragma once

#include "../mesh.h"

#include <GLFW/glfw3.h>

namespace libRenderer {
    struct VertexBufferHandle {
        unsigned int vertexArrayObject;
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
            GLFWwindow *window;
            std::vector<VertexBufferHandle> vertexArrays = std::vector<VertexBufferHandle>();
    };
}