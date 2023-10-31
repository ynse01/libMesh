#pragma once

namespace YetAnotherUI {
    class Renderable {
        public:
            virtual void Initialize() = 0;
            virtual void Render() = 0;
            virtual void Destroy() = 0;
    };
}