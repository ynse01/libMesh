#pragma once

#include <string>

#include "renderable.h"
#include "../color.h"
#include "../point2.h"
#include "../point3.h"

namespace YetAnotherUI {
    class Shader : public Renderable {
        public:
            Shader(const char *vertexCode, const char *fragmentCode);
            ~Shader();

            void Initialize();
            void Render();
            void Destroy();

            int getIndexOfUniform(const char *name);
            void setUniform(unsigned int index, libMesh::Color color);
            void setUniform(unsigned int index, libMesh::Point3 point);
            void setUniform(unsigned int index, libMesh::Point2 point);
        private:
            unsigned int mShaderId;
            const char *mVertexShaderCode;
            const char *mFragmentShaderCode;
    };
}