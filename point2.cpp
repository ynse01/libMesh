#include "point2.h"

#include <cmath>

libMesh::Point2::Point2()
{
}

libMesh::Point2::Point2(float x, float y)
{
}

libMesh::Point2::Point2(const Point2 &other)
{
}

libMesh::Vector2 libMesh::Point2::betweenPoints(Point2 p0, Point2 p1)
{
    return Vector2(p1.x - p0.x, p1.y - p0.y);
}

libMesh::Point2 libMesh::Point2::add(Vector2 vector)
{
    return Point2(x + vector.x, y + vector.y);
}

libMesh::Point2 libMesh::Point2::subtract(Vector2 vector)
{
    return Point2(x - vector.x, y - vector.y);
}

float libMesh::Point2::distanceTo(Point2 other)
{
    return std::sqrt(squaredDistanceTo(other));
}

float libMesh::Point2::squaredDistanceTo(Point2 other)
{
    Vector2 diff = betweenPoints(*this, other);
    return diff.dot(diff);
}
