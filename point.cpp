#include "point.h"

#include <cmath>

libMesh::Point::Point() : x(0.0f), y(0.0f), z(0.0f)
{
}

libMesh::Point::Point(float x, float y, float z) : x(x), y(y), z(z)
{
}

libMesh::Point::Point(Point &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

void libMesh::Point::add(Vector &vector)
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
}

void libMesh::Point::subtract(Vector &vector)
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
}

float libMesh::Point::distanceTo(Point &other)
{
    return std::sqrt(squaredDistanceTo(other));
}

float libMesh::Point::squaredDistanceTo(Point &other)
{
    Vector diff = Vector(other.y - x, other.y - y, other.z - z);
    return diff.dot(diff);
}