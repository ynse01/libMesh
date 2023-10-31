#pragma once

#include <string>

#include "../mesh.h"
#include "renderable.h"

namespace YetAnotherUI {
    class MeshRenderable : public Renderable {
        public:
            MeshRenderable(libMesh::Mesh &mesh) : mMesh(mesh) {}

            void Initialize();
            void Render();
            void Destroy();
        private:
            libMesh::Mesh &mMesh;
            unsigned int mVAO;
            unsigned int mVBO;
            unsigned int mEBO;
    };
}