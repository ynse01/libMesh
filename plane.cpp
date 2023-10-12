#include "plane.h"

libMesh::Plane::Plane(Point origin, Vector xAxis, Vector yAxis)
: origin(origin), xAxis(xAxis), yAxis(yAxis)
{
}

libMesh::Point libMesh::Plane::get(Size2 index)
{
    return 
        origin
            .add(xAxis.scale(index.width))
            .add(yAxis.scale(index.height));
}
