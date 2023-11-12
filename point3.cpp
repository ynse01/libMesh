#include "point3.h"
#include "transformation.h"
#include "math.h"

#include <cmath>

libMesh::Point3::Point3() : Point3(0.0f, 0.0f, 0.0f)
{
}

libMesh::Point3::Point3(float x, float y, float z) : x(x), y(y), z(z)
{
}

libMesh::Point3::Point3(const Point3 &other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

libMesh::Vector3 libMesh::Point3::betweenPoints(Point3 p0, Point3 p1)
{
    return Vector3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
}

void libMesh::Point3::add(Vector3 vector)
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
}

libMesh::Point3 libMesh::Point3::added(Vector3 vector)
{
    return Point3(x + vector.x, y + vector.y, z + vector.z);
}

void libMesh::Point3::subtract(Vector3 vector)
{
    x -= vector.x;
    y -= vector.y;
    z -= vector.z;
}

libMesh::Point3 libMesh::Point3::subtracted(Vector3 vector)
{
    return Point3(x - vector.x, y - vector.y, z - vector.z);
}

float libMesh::Point3::distanceTo(Point3 other)
{
    return std::sqrt(squaredDistanceTo(other));
}

float libMesh::Point3::squaredDistanceTo(Point3 other)
{
    Vector3 diff = betweenPoints(*this, other);
    return diff.dot(diff);
}

void libMesh::Point3::transform(Transformation t)
{
    float newX = (x * t.m00 + y * t.m10 + z * t.m20 + t.m03) / t.m33;
    float newY = (x * t.m01 + y * t.m11 + z * t.m21 + t.m13) / t.m33;
    float newZ = (x * t.m02 + y * t.m12 + z * t.m22 + t.m23) / t.m33;
    x = newX;
    y = newY;
    z = newZ;
}

libMesh::Point3 libMesh::Point3::transformed(Transformation t)
{
    float newX = (x * t.m00 + y * t.m10 + z * t.m20 + t.m03) / t.m33;
    float newY = (x * t.m01 + y * t.m11 + z * t.m21 + t.m13) / t.m33;
    float newZ = (x * t.m02 + y * t.m12 + z * t.m22 + t.m23) / t.m33;
    return Point3(newX, newY, newZ);
}

bool libMesh::Point3::operator==(const Point3 &other) const
{
    return Math::Equals(x, other.x) && Math::Equals(y, other.y) && Math::Equals(z, other.z);
}

bool libMesh::Point3::operator!=(const Point3 &other) const
{
    return !(*this == other);
}
