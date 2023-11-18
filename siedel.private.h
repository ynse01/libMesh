#include "siedel.h"
#include "math.h"

#include <vector>
#include <cstring>

struct Segment {
    libMesh::Point2 begin;
    libMesh::Point2 end;
    bool isInserted;
    unsigned int root0;
    unsigned int root1;
    unsigned int next;
    unsigned int prev;
};

class SegmentStructure {
    public:
        SegmentStructure(unsigned int numSegment) {
            seg = new std::vector<Segment>();
            seg->reserve(numSegment);
            permute = nullptr;
            chooseIndex = 0;
        }
        ~SegmentStructure () {
            delete seg;
            if (permute != nullptr) {
                delete permute;
            }
        }

        unsigned int nextIndex() {
            Segment s{};
            seg->push_back(s);
            return seg->size() - 1;
        }
        Segment &operator[](unsigned int index) { return seg->at(index); }

        void generateRandomOrdering();
        unsigned int chooseSegment() { return permute->at(chooseIndex++); }
    private:
        std::vector<Segment> *seg;
        std::vector<unsigned int> *permute;
        unsigned int chooseIndex;
};

enum Side {
    Left,
    Right
};

struct Trapezoid {
    libMesh::Point2 high;
    libMesh::Point2 low;
    unsigned int down0;
    unsigned int down1;
    unsigned int up0;
    unsigned int up1;
    unsigned int up2;
    Side up2Side;
    unsigned int leftSegment;
    unsigned int rightSegment;
    unsigned int sink;
    bool state;
};

class TrapezoidStructure {
    public:
        TrapezoidStructure(unsigned int numTrap) {
            trap = new std::vector<Trapezoid>();
            trap->reserve(numTrap);
        }
        ~TrapezoidStructure () { delete trap; }

        unsigned int nextIndex() { 
            Trapezoid t{};
            trap->push_back(t);
            return trap->size() - 1;
        }
        Trapezoid &operator[](unsigned int index) { return trap->at(index); }
    private:
        std::vector<Trapezoid> *trap;
};

enum NodeType {
    X,
    Y,
    Sink
};

struct QueryNode {
    NodeType type;
    libMesh::Point2 yValue;
    unsigned int segmentIndex;
    unsigned int trapezoidIndex;
    unsigned int left;
    unsigned int right;
    unsigned int parent;
};

class QueryStructure {
    public:
        QueryStructure(unsigned int numNodes) {
            qs = new std::vector<QueryNode>();
            qs->reserve(numNodes);
        }
        ~QueryStructure () { delete qs; }

        unsigned int nextIndex() {
            QueryNode node{};
            qs->push_back(node);
            return qs->size() - 1;
        }
        QueryNode &operator[](unsigned int index) { return qs->at(index); }
    private:
        std::vector<QueryNode> *qs;
};

namespace {

    unsigned int mathLogStarN(unsigned int a);
    unsigned int mathN(unsigned int a, unsigned int b);

    double dot(const libMesh::Point2 &v0, const libMesh::Point2 &v1);
    double cross(const libMesh::Point2 &v0, const libMesh::Point2 &v1, const libMesh::Point2 &v2);

    bool greaterThan(const libMesh::Point2 &a, const libMesh::Point2 &b);
    bool greaterThanEqualTo(const libMesh::Point2 &a, const libMesh::Point2 &b);
    bool equalTo(const libMesh::Point2 &a, const libMesh::Point2 &b);
    bool lessThan(const libMesh::Point2 &a, const libMesh::Point2 &b);
    bool isLeftOf(const Segment &segment, const libMesh::Point2 &b);

    void max(libMesh::Point2 &yValue, const libMesh::Point2 &v0, const libMesh::Point2 &v1);
    void min(libMesh::Point2 &yValue, const libMesh::Point2 &v0, const libMesh::Point2 &v1);

    bool inserted(SegmentStructure &seg, unsigned int segmentIndex, bool mode);

    unsigned int locateEndpoint(const libMesh::Point2 &v, const libMesh::Point2 &vo, unsigned int root, std::vector<Segment> &seg, QueryStructure &qs);

    unsigned int initializeSegments(SegmentStructure &seg, libMesh::PolyLine &polygon, unsigned int *contours, unsigned int nContours);
    
    /* Initialize the query structure (Q) and the trapezoid table (T) 
     * when the first segment is added to start the trapezoidation. The
     * query-tree starts out with 4 trapezoids, one S-node and 2 Y-nodes
     *    
     *                4
     *   -----------------------------------
     *  		  \
     *  	1	   \        2
     *  		    \
     *   -----------------------------------
     *                3
     */
    unsigned int initQueryStructure(SegmentStructure &seg, unsigned int segmentIndex, QueryStructure &qs, TrapezoidStructure &trap);

    void mergeTrapezoids(unsigned int segmentIndex, unsigned int tfirst, unsigned int tlast, Side side, QueryStructure &qs, TrapezoidStructure &tr);

    /* Add in the new segment into the trapezoidation and update Q and T
    * structures. First locate the two endpoints of the segment in the
    * Q-structure. Then start from the topmost trapezoid and go down to
    * the  lower trapezoid dividing all the trapezoids in between .
    */
    unsigned int addSegment(SegmentStructure &seg, unsigned int segmentIndex, QueryStructure &qs, TrapezoidStructure &tr);

    /* Update the roots stored for each of the endpoints of the segment.
    * This is done to speed up the location-query for the endpoint when
    * the segment is inserted into the trapezoidation subsequently
    */
    void findNewRoots(SegmentStructure &seg, unsigned int segmentNumber, QueryStructure &qs, TrapezoidStructure &tr);

    void constructTrapezoids(SegmentStructure &seg, unsigned int segmentIndex, QueryStructure &qs, TrapezoidStructure &trap);

    unsigned int monotonateTrapezoids(TrapezoidStructure &tr, SegmentStructure &seg, unsigned int numSegments);

    void siedelTriangulation(libMesh::PolyLine &polygon);
}