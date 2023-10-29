#pragma once

#include "../color.h"
#include "shader.h"

namespace YetAnotherUI {
    class SolidTextBrush {
        public:
            SolidTextBrush() : SolidTextBrush(libMesh::Color()) {}
            SolidTextBrush(libMesh::Color color) : mShader(Shader(sVertexShaderCode, sFragmentShaderCode)), color(color) {}
            SolidTextBrush(const SolidTextBrush& other) : SolidTextBrush(other.color) {}
            
            libMesh::Color color;

            void Initialize() { mShader.Initialize(); }
            void Render() { 
                mShader.Render();
                mShader.setUniform(0, color);
            }
            void Destroy() { mShader.Destroy(); }
        private:
            Shader mShader;
            static const std::string sVertexShaderCode;
            static const std::string sFragmentShaderCode;
    };
}