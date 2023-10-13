#include "point.h"
#include "transformation.h"

#include <cmath>

libMesh::Point::Point() : Point(0.0f, 0.0f, 0.0f)
{
}

libMesh::Point::Point(float x, float y, float z) : x(x), y(y), z(z)
{
}

libMesh::Point::Point(const Point &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

libMesh::Vector libMesh::Point::betweenPoints(Point p0, Point p1)
{
    return Vector(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
}

libMesh::Point libMesh::Point::add(Vector vector)
{
    return Point(x + vector.x, y + vector.y, z + vector.z);
}

libMesh::Point libMesh::Point::subtract(Vector vector)
{
    return Point(x - vector.x, y - vector.y, z - vector.z);
}

float libMesh::Point::distanceTo(Point other)
{
    return std::sqrt(squaredDistanceTo(other));
}

float libMesh::Point::squaredDistanceTo(Point other)
{
    Vector diff = Vector(other.y - x, other.y - y, other.z - z);
    return diff.dot(diff);
}

libMesh::Point libMesh::Point::transform(Transformation t)
{
    float newX = (x * t.m00 + y * t.m10 + z * t.m20 + t.m03) / t.m33;
    float newY = (x * t.m01 + y * t.m11 + z * t.m21 + t.m13) / t.m33;
    float newZ = (x * t.m02 + y * t.m12 + z * t.m22 + t.m23) / t.m33;
    return Point(newX, newY, newZ);
}
