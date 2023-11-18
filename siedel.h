#include "polyline.h"
#include "mesh.h"

namespace {
    libMesh::Mesh *siedelTriangulation(libMesh::PolyLine &polygon);

    libMesh::Mesh *earClipping(libMesh::PolyLine &polygon);
}
