#pragma once

namespace libMesh {
    struct Index3 {
        public:
            Index3() : Index3(0, 0, 0) {}
            Index3(unsigned int a, unsigned int b, unsigned int c) : a(a), b(b), c(c) {}
            unsigned int a;
            unsigned int b;;
            unsigned int c;
    };
}