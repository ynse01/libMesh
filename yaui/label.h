#pragma once

#include <string>

#include "font.h"
#include "solidTextBrush.h"
#include "../point2.h"

namespace YetAnotherUI {
    class Label : public Renderable {
        public:
            Label(Font& font, std::string& text, SolidTextBrush& brush, float scale);

            std::string &getText() { return mText; }
            void setText(const std::string &text);

            void Initialize();
            void Render();
            void Destroy();
        private:
            Font &mFont;
            std::string mText;
            SolidTextBrush mBrush;
            libMesh::Point2 mPosition;
            float mScale;
            unsigned int mVAO;
            unsigned int mVBO;
    };
}