#pragma once

#include <vector>

#include "renderable.h"

namespace YetAnotherUI {
    class Renderer {
        public:
            bool start();
            void close();
            void run();

            void add(Renderable *renderable);
            void remove(Renderable *renderable);
        private:
            void *window;
            unsigned int shaderProgram;
            std::vector<Renderable*> mRenderables = std::vector<Renderable*>();
    };
}