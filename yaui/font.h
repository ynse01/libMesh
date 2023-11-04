#pragma once

#include <string>
#include <map>

#include "renderable.h"

namespace YetAnotherUI {
    struct Character {
        unsigned int TextureId;
        unsigned int Size[2];
        int Bearing[2];
        long int Advance;
    };

    class Font : public Renderable {
        public:
            Font(std::string &path);
            ~Font();

            Character getCharacter(char c) { return mCharacters[c]; }
            long int getKerning(char current, char previous);

            void Initialize();
            void Render();
            void Destroy();
        private:
            void *mFace;
            bool mHasKerning;
            std::map<char, Character> mCharacters;
    };
}