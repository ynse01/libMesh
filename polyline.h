#pragma once

#include <vector>

#include "point2.h"
#include "lineSegment2.h"

namespace libMesh {
    class PolyLine {
        public:
            PolyLine() : mPoints(std::vector<Point2>()) {  }
            PolyLine(std::vector<Point2> points) : mPoints(points) {  }
            PolyLine(const PolyLine &other) : PolyLine() {
                mPoints = other.mPoints;
            }
            
            void add(Point2 point) { mPoints.push_back(point); }
            void close() { mPoints.push_back(mPoints[0]); }
            unsigned int count() { return mPoints.size(); }
            Point2 getPoint(unsigned int index) { return mPoints[index]; }
            LineSegment2 getSegment(unsigned int index) { return LineSegment2(mPoints[index], mPoints[index + 1]); }
            void triangulate();

            friend std::ostream& operator <<(std::ostream& os, const PolyLine& poly) {
                return os;
            }
        private:
            std::vector<Point2> mPoints;
    };
}