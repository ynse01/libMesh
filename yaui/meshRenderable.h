#pragma once

#include <string>

#include "../mesh.h"
#include "renderable.h"
#include "solidBrush.h"

namespace YetAnotherUI {
    class MeshRenderable : public Renderable {
        public:
            MeshRenderable(libMesh::Mesh &mesh, SolidBrush brush) : mMesh(mesh), mBrush(brush) {}

            void Initialize();
            void Render();
            void Destroy();
        private:
            libMesh::Mesh &mMesh;
            SolidBrush &mBrush;
            unsigned int mVAO;
            unsigned int mVBO;
            unsigned int mEBO;
    };
}