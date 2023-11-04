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

            void Initialize() { 
                mShader.Initialize();
                mUniformIndex = mShader.getIndexOfUniform("textColor");
            }
            void Render() { 
                mShader.Render();
                mShader.setUniform(mUniformIndex, color);
            }
            void Destroy() { mShader.Destroy(); }
        private:
            Shader mShader;
            int mUniformIndex;
            static const std::string sVertexShaderCode;
            static const std::string sFragmentShaderCode;
    };
}