#include "plane.h"

libMesh::Plane::Plane(Point3 origin, Vector3 xAxis, Vector3 yAxis)
: origin(origin), xAxis(xAxis), yAxis(yAxis)
{
}

libMesh::Point3 libMesh::Plane::get(Point2 pos)
{
    return 
        origin
            .added(xAxis.scaled(pos.x))
            .added(yAxis.scaled(pos.y));
}

libMesh::Vector3 libMesh::Plane::getNormal()
{
    return xAxis.cross(yAxis);
}

bool libMesh::Plane::operator==(const Plane &other) const
{
    return origin == other.origin && xAxis == other.xAxis && yAxis == other.yAxis;
}

bool libMesh::Plane::operator!=(const Plane &other) const
{
    return !(*this == other);
}
