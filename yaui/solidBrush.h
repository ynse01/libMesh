#pragma once

#include "../color.h"
#include "shader.h"

namespace YetAnotherUI {
    class SolidBrush {
        public:
            SolidBrush() : SolidBrush(libMesh::Color()) {}
            SolidBrush(libMesh::Color color) : mShader(Shader(sVertexShaderCode, sFragmentShaderCode)), color(color) {}
            SolidBrush(const SolidBrush& other) : SolidBrush(other.color) {}
            
            libMesh::Color color;

            void Initialize() { 
                mShader.Initialize();
                mUniformIndex = mShader.getIndexOfUniform("matColor");
            }
            void Render() { 
                mShader.Render();
                mShader.setUniform(mUniformIndex, color);
            }
            void Destroy() { mShader.Destroy(); }
        private:
            Shader mShader;
            int mUniformIndex;
            static const char *sVertexShaderCode;
            static const char *sFragmentShaderCode;
    };
}