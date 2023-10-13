#include "plane.h"

libMesh::Plane::Plane(Point3 origin, Vector3 xAxis, Vector3 yAxis)
: origin(origin), xAxis(xAxis), yAxis(yAxis)
{
}

libMesh::Point3 libMesh::Plane::get(Size2 index)
{
    return 
        origin
            .add(xAxis.scale(index.width))
            .add(yAxis.scale(index.height));
}

libMesh::Vector3 libMesh::Plane::getNormal()
{
    return xAxis.cross(yAxis);
}
