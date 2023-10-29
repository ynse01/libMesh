#pragma once

#include <string>

namespace libMesh {
    class Font;
    class TextPolygon {
        public:
            TextPolygon(Font& font, std::string text) {}
            
            void render() {
                // 1. Get list of glyphs to render, from the string characters.
                // 2. Find the distances between the charcters
                // 3. Split font height in 10 steps, possible aligning with poly positions.
                // 4. Calculate intersection at each row (x coord + poly index).
                // 5. Fill in the sections between the rows with triangles.
            }

            friend std::ostream& operator <<(std::ostream& os, const TextPolygon& poly) {
                return os;
            }
    };
}