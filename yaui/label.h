#pragma once

#include <string>

#include "font.h"

namespace YetAnotherUI {
    class Label {
        public:
            Label(Font& font, std::string& text);

            std::string &getText() { return mText; }
            void setText(const std::string &text);
        private:
        std::string mText;
    };
}