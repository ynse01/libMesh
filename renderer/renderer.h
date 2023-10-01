#pragma once

#include <GLFW/glfw3.h>

namespace libRenderer {
    class Renderer {
        public:
            bool start();
            void close();
            void run();
        private:
            GLFWwindow *window;
    };
}