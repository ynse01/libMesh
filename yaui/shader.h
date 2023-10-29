#pragma once

#include <string>

#include "renderable.h"
#include "../color.h"
#include "../point2.h"
#include "../point3.h"

namespace YetAnotherUI {
    class Shader : Renderable {
        public:
            Shader(const std::string &vertexCode, const std::string &fragmentCode);
            ~Shader();

            void Initialize();
            void Render();
            void Destroy();

            void setUniform(unsigned int index, libMesh::Color color);
            void setUniform(unsigned int index, libMesh::Point3 point);
            void setUniform(unsigned int index, libMesh::Point2 point);
        private:
            unsigned int mShaderId;
            const std::string mVertexShaderCode;
            const std::string mFragmentShaderCode;
    };
}