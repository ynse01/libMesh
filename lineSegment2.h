#pragma once

#include <vector>

#include "point2.h"

namespace libMesh {
    class LineSegment2 {
        public:
            LineSegment2(Point2 begin, Point2 end) : begin(begin), end(end) {  }
            
            Point2 begin;
            Point2 end;

            friend std::ostream& operator <<(std::ostream& os, const LineSegment2& segment) {
                return os << "LineSegment from " << segment.begin << " to " << segment.end;
            }
    };
}