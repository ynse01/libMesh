#include "siedel.h"
#include "siedel.private.h"
#include "math.h"

#include <cstring>
#include <cmath>
#include <limits>
#include <sys/time.h>

void SegmentStructure::generateRandomOrdering() {
    struct timeval tval;
    struct timezone tzone;
    unsigned int i;
    unsigned int n = seg->size();
    unsigned int m, st[n], *p;

    chooseIndex = 0;
    permute = new std::vector<unsigned int>();
    permute->reserve(n);
    gettimeofday(&tval, &tzone);
    srand48(tval.tv_sec);

    for(i = 0u; i <= n; i++) {
        st[i] = i;
    }
    p = st;
    for (i = 1; i <= n; i++) {
        m = lrand48() % (n + 1 - i) + 1;
        permute->push_back(p[m]);
        if (m != 1) {
            p[m] = p[1];
        }
    }
}

namespace siedel {

    unsigned int mathLogStarN(unsigned int n) {
        unsigned int i;
        double v = (double)n;
        for(i = 0; v >= 1; i++) {
            v = std::log2(v);
        }
        return i - 1;
    }

    unsigned int mathN(unsigned int n, unsigned int h) {
        unsigned int i;
        double v;

        for (i = 0u, v = n; i < h; i++) {
            v = log2(v);
        }
  
        return (int) ceil(1.0 * n / v);
    }

    double dot(const libMesh::Point2 &v0, const libMesh::Point2 &v1) {
        return (v0.x * v1.x + v0.y * v1.y);
    }

    double cross(const libMesh::Point2 &v0, const libMesh::Point2 &v1, const libMesh::Point2 &v2) {
        return ((v1.x - v0.x) * (v2.y - v0.y) - (v1.y - v0.y) * (v2.x - v0.x));
    }

    bool greaterThan(const libMesh::Point2 &v0, const libMesh::Point2 &v1) {
        if (libMesh::Math::GreaterThan(v0.y, v1.y)) {
            return true;
        } else if (libMesh::Math::LessThan(v0.y, v1.y)) {
            return false;
        } else {
            return v0.x > v1.x;
        }
    }

    bool greaterThanEqualTo(const libMesh::Point2 &v0, const libMesh::Point2 &v1) {
        if (libMesh::Math::GreaterThan(v0.y, v1.y)) {
            return true;
        } else if (libMesh::Math::LessThan(v0.y, v1.y)) {
            return false;
        } else {
            return v0.x >= v1.x;
        }
    }

    bool equalTo(const libMesh::Point2 &v0, const libMesh::Point2 &v1)
    {
        return libMesh::Math::Equals(v0.y, v1.y) && libMesh::Math::Equals(v0.x, v1.x);
    }

    bool lessThan(const libMesh::Point2 &v0, const libMesh::Point2 &v1)
    {
        if (libMesh::Math::LessThan(v0.y, v1.y)) {
            return true;
        } else if (libMesh::Math::GreaterThan(v0.y, v1.y)) {
            return false;
        } else {
            return (v0.x < v1.x);
        }
    }

    bool isLeftOf(const Segment &s, const libMesh::Point2 &v)
    {
        double area;

        if (greaterThan(s.end, s.begin)) {
            if (libMesh::Math::Equals(s.end.y, v.y)) {
                if (v.x < s.end.x) {
                    area = 1.0;
                } else {
                    area = -1.0;
                }
            } else if (libMesh::Math::Equals(s.begin.y, v.y)) {
                if (v.x < s.begin.x) {
                    area = 1.0;
                } else {
                    area = -1.0;
                }
            } else {
                area = cross(s.begin, s.end, v);
            }
        }
        return false;
    }

    void max(libMesh::Point2 &yValue, const libMesh::Point2 &v0, const libMesh::Point2 &v1) {
        if (libMesh::Math::GreaterThan(v0.y, v1.y)) {
            yValue = v0;
        } else if (libMesh::Math::Equals(v0.y, v1.y)) {
            if (libMesh::Math::GreaterThan(v0.x, v1.x)) {
                yValue = v0;
            } else {
                yValue = v1;
            }
        } else {
            yValue = v1;
        }
    }

    void min(libMesh::Point2 &yValue, const libMesh::Point2 &v0, const libMesh::Point2 &v1) {
        if (libMesh::Math::LessThan(v0.y, v1.y)) {
            yValue = v0;
        } else if (libMesh::Math::Equals(v0.y, v1.y)) {
            if (v0.x < v1.x) {
                yValue = v0;
            } else {
                yValue = v1;
            }
        } else {
            yValue = v1;
        }
    }

    bool inserted(SegmentStructure &seg, unsigned int segmentIndex, bool mode)
    {
        if (mode) {
            return seg[seg[segmentIndex].prev].isInserted;
        } else {
            return seg[seg[segmentIndex].next].isInserted;
        }
    }

    unsigned int locateEndpoint(const libMesh::Point2 &v, const libMesh::Point2 &vo, unsigned int root, SegmentStructure &seg, QueryStructure &qs) {
        QueryNode *rptr = &qs[root];
  
        switch (rptr->type) {
            case NodeType::Sink:
                return rptr->trapezoidIndex;
            case NodeType::Y:
                if (greaterThan(v, rptr->yValue)) { /* above */
	                return locateEndpoint(v, vo, rptr->right, seg, qs);
                } else if (equalTo(v, rptr->yValue)) { 
                    /* the point is already inserted. */
	                if (greaterThan(vo, rptr->yValue)) { /* above */
	                    return locateEndpoint(v, vo, rptr->right, seg, qs);
                    } else {
	                    return locateEndpoint(v, vo, rptr->left, seg, qs); /* below */	    
	                }
                } else {
	                return locateEndpoint(v, vo, rptr->left, seg, qs); /* below */
                }

            case NodeType::X:
                if (equalTo(v, seg[rptr->segmentIndex].begin) || 
                    equalTo(v, seg[rptr->segmentIndex].end)) {
                    if (libMesh::Math::Equals(v.y, vo.y)) { /* horizontal segment */
                        if (vo.x < v.x) {
                            return locateEndpoint(v, vo, rptr->left, seg, qs); /* left */
                        } else {
                            return locateEndpoint(v, vo, rptr->right, seg, qs); /* right */
                        }
                    }
                    else if (isLeftOf(seg[rptr->segmentIndex], vo)) {
                        return locateEndpoint(v, vo, rptr->left, seg, qs); /* left */
                    } else {
                        return locateEndpoint(v, vo, rptr->right, seg, qs); /* right */
                    }
                } else if (isLeftOf(seg[rptr->segmentIndex], v)) {
                    return locateEndpoint(v, vo, rptr->left, seg, qs); /* left */
                } else {
                    return locateEndpoint(v, vo, rptr->right, seg, qs); /* right */	
                }

            default:
                fprintf(stderr, "Haggu !!!!!\n");
                break;
        }
        return 0u;
    }

    unsigned int initializeSegments(SegmentStructure &seg, libMesh::PolyLine &polygon, unsigned int *contours, unsigned int nContours) {
        unsigned int contourCount = 0;
        unsigned int i = 1;
        unsigned int nPoints;

        while(contourCount < nContours) {
            unsigned int j;
            unsigned int first, last;

            nPoints = contours[contourCount];
            first = i;
            last = first + nPoints - 1;
            // First segment
            seg[i].begin = polygon.getPoint(i);
            seg[i].next = i + 1;
            seg[i].prev = last;
            seg[last].end = seg[i].begin;
            seg[i].isInserted = false;
            i++;
            for(j = 1u; j < nPoints - 1; j++, i++) {
                seg[i].begin = polygon.getPoint(i);
                seg[i].next = i + 1;
                seg[i].prev = i - 1;
                seg[i - 1].end = seg[i].begin;
                seg[i].isInserted = false;
            }
            // Last segment
            seg[i].begin = polygon.getPoint(i);
            seg[i].next = first;
            seg[i].prev = i - 1;
            seg[i - 1].end = seg[i].begin;
            seg[i].isInserted = false;
            i++;
            contourCount++;
        }
        return i - 1;
    }

    unsigned int initQueryStructure(SegmentStructure &seg, unsigned int segmentIndex, QueryStructure &qs, TrapezoidStructure &tr) {
        Segment &s = seg[segmentIndex];
        unsigned int root, i1, i2, i3, i4, i5, i6, i7;
        unsigned int qIndex, trIndex = 1;

        i1 = qs.nextIndex();
        qs[i1].type = NodeType::Y;
        max(qs[i1].yValue, s.begin, s.end);
        root = i1;

        qs[i1].right = i2 = qs.nextIndex();
        qs[i2].type = NodeType::Sink;
        qs[i2].parent = i1;

        qs[i1].left = i3 = qs.nextIndex();
        qs[i3].type = NodeType::Y;
        min(qs[i3].yValue, s.begin, s.end);
        qs[i3].parent = i1;

        qs[i3].left = i4 = qs.nextIndex();
        qs[i4].type = NodeType::Sink;
        qs[i4].parent = i3;

        qs[i3].right = i5 = qs.nextIndex();
        qs[i5].type = NodeType::X;
        qs[i5].segmentIndex = segmentIndex;
        qs[i5].parent = i3;

        qs[i5].left = i6 = qs.nextIndex();
        qs[i6].type = NodeType::Sink;
        qs[i6].parent = i5;

        qs[i5].right = i7 = qs.nextIndex();
        qs[i7].type = NodeType::Sink;
        qs[i7].parent = i5;

        unsigned int t1, t2, t3, t4;
        t1 = tr.nextIndex();
        t2 = tr.nextIndex();
        t3 = tr.nextIndex();
        t4 = tr.nextIndex();

        tr[t1].high = tr[t2].high = tr[t4].low = qs[i1].yValue;
        tr[t1].low = tr[t2].low = tr[t3].high = qs[i3].yValue;
        tr[t4].high.y = std::numeric_limits<float>().infinity();
        tr[t4].high.x = std::numeric_limits<float>().infinity();
        tr[t3].low.y = -std::numeric_limits<float>().infinity();
        tr[t3].low.x = -std::numeric_limits<float>().infinity();
        tr[t1].rightSegment = tr[t2].leftSegment = segmentIndex;
        tr[t1].up0 = tr[t2].up0 = t4;
        tr[t1].down0 = tr[t2].down0 = t3;
        tr[t4].down0 = tr[t3].up0 = t1;
        tr[t4].down1 = tr[t3].up1 = t2;

        tr[t1].sink = i6;
        tr[t2].sink = i7;
        tr[t3].sink = i4;
        tr[t4].sink = i2;

        tr[t1].state = tr[t2].state = true;
        tr[t3].state = tr[t4].state = true;

        qs[i2].trapezoidIndex = t4;
        qs[i4].trapezoidIndex = t3;
        qs[i6].trapezoidIndex = t1;
        qs[i7].trapezoidIndex = t2;

        s.isInserted = true;
        return 0;
    }

    void mergeTrapezoids(unsigned int segmentIndex, unsigned int tfirst, unsigned int tlast, Side side, QueryStructure &qs, TrapezoidStructure &tr)
    {
        int t, tnext, cond;
        int ptnext;

        /* First merge polys on the LHS */
        t = tfirst;
        while ((t > 0) && greaterThanEqualTo(tr[t].low, tr[tlast].low)) {
            if (side == Side::Left) {
                cond = ((((tnext = tr[t].down0) > 0) && (tr[tnext].rightSegment == segmentIndex)) ||
                    (((tnext = tr[t].down1) > 0) && (tr[tnext].rightSegment == segmentIndex)));
            } else {
                cond = ((((tnext = tr[t].down0) > 0) && (tr[tnext].leftSegment == segmentIndex)) ||
                    (((tnext = tr[t].down1) > 0) && (tr[tnext].leftSegment == segmentIndex)));
            }
            if (cond) {
                if ((tr[t].leftSegment == tr[tnext].leftSegment) &&
                    (tr[t].rightSegment == tr[tnext].rightSegment)) { /* good neighbours */
                    /* merge them */
                    /* Use the upper node as the new node i.e. t */
                
                    ptnext = qs[tr[tnext].sink].parent;
                    
                    if (qs[ptnext].left == tr[tnext].sink) {
                        qs[ptnext].left = tr[t].sink;
                    } else {
                        qs[ptnext].right = tr[t].sink;	/* redirect parent */
                    }
                    
                    /* Change the upper neighbours of the lower trapezoids */
                    
                    if ((tr[t].down0 = tr[tnext].down0) > 0) {
                        if (tr[tr[t].down0].up0 == tnext) {
                            tr[tr[t].down0].up0 = t;
                        } else if (tr[tr[t].down0].up1 == tnext) {
                            tr[tr[t].down0].up1 = t;
                        }
                    }
                    if ((tr[t].down1 = tr[tnext].down1) > 0) {
                        if (tr[tr[t].down1].up0 == tnext) {
                            tr[tr[t].down1].up0 = t;
                        } else if (tr[tr[t].down1].up1 == tnext) {
                            tr[tr[t].down1].up1 = t;
                        }
                    }
                    tr[t].low = tr[tnext].low;
                    tr[tnext].state = false; /* invalidate the lower */
                                        /* trapezium */
                } else { /* not good neighbours */
                    t = tnext;
                }
            } else { /* do not satisfy the outer if */
                t = tnext;
            }
        } /* end-while */
    }

    /* Add in the new segment into the trapezoidation and update Q and T
    * structures. First locate the two endpoints of the segment in the
    * Q-structure. Then start from the topmost trapezoid and go down to
    * the  lower trapezoid dividing all the trapezoids in between .
    */

    unsigned int addSegment(SegmentStructure &seg, unsigned int segmentIndex, QueryStructure &qs, TrapezoidStructure &tr) {
        Segment &s = seg[segmentIndex];
        Segment &so = seg[segmentIndex];
        unsigned int tu, tl, sk, tfirst, tlast, tnext;
        unsigned int tfirstr, tlastr, tfirstl, tlastl;
        unsigned int i1, i2, t, t1, t2, tn;
        libMesh::Point2 tpt;
        int tritop = 0, tribot = 0;
        bool isSwapped = false;
        int tmptriseg;

        if (greaterThan(s.end, s.begin)) {
            /* Get higher vertex in begin */
            int tmp;
            tpt = s.begin;
            s.begin = s.end;
            s.end = tpt;
            tmp = s.root0;
            s.root0 = s.root1;
            s.root1 = tmp;
            isSwapped = true;
        }

        if (!inserted(seg, segmentIndex, isSwapped)) {
            /* insert v0 in the tree */
            int tmp_d;

            tu = locateEndpoint(s.begin, s.end, s.root0, seg, qs);
            tl = tr.nextIndex();		/* tl is the new lower trapezoid */
            tr[tl].state = true;
            tr[tl] = tr[tu];
            tr[tu].low.y = tr[tl].high.y = s.begin.y;
            tr[tu].low.x = tr[tl].high.x = s.begin.x;
            tr[tu].down0 = tl;
            tr[tu].down1 = 0;
            tr[tl].up0 = tu;
            tr[tl].up1 = 0;

            if (((tmp_d = tr[tl].down0) > 0) && (tr[tmp_d].up0 == tu))
            tr[tmp_d].up0 = tl;
            if (((tmp_d = tr[tl].down0) > 0) && (tr[tmp_d].up1 == tu))
            tr[tmp_d].up1 = tl;

            if (((tmp_d = tr[tl].down1) > 0) && (tr[tmp_d].up0 == tu))
            tr[tmp_d].up0 = tl;
            if (((tmp_d = tr[tl].down1) > 0) && (tr[tmp_d].up1 == tu))
            tr[tmp_d].up1 = tl;

            /* Now update the query structure and obtain the sinks for the */
            /* two trapezoids */ 
            
            i1 = qs.nextIndex();		/* Upper trapezoid sink */
            i2 = qs.nextIndex();		/* Lower trapezoid sink */
            sk = tr[tu].sink;
            
            qs[sk].type = NodeType::Y;
            qs[sk].yValue = s.begin;
            qs[sk].segmentIndex = segmentIndex;	/* not really reqd ... maybe later */
            qs[sk].left = i2;
            qs[sk].right = i1;

            qs[i1].type = NodeType::Sink;
            qs[i1].trapezoidIndex = tu;
            qs[i1].parent = sk;

            qs[i2].type = NodeType::Sink;
            qs[i2].trapezoidIndex = tl;
            qs[i2].parent = sk;

            tr[tu].sink = i1;
            tr[tl].sink = i2;
            tfirst = tl;
            }
        else				/* v0 already present */
            {       /* Get the topmost intersecting trapezoid */
            tfirst = locateEndpoint(s.begin, s.end, s.root0, seg, qs);
            tritop = 1;
            }


        if (!inserted(seg, segmentIndex, isSwapped)) {
            /* insert v1 in the tree */
            int tmp_d;

            tu = locateEndpoint(s.end, s.begin, s.root1, seg, qs);

            tl = tr.nextIndex();		/* tl is the new lower trapezoid */
            tr[tl].state = true;
            tr[tl] = tr[tu];
            tr[tu].low.y = tr[tl].high.y = s.end.y;
            tr[tu].low.x = tr[tl].high.x = s.end.x;
            tr[tu].down0 = tl;      
            tr[tu].down1 = 0;
            tr[tl].up0 = tu;
            tr[tl].up1 = 0;

            if (((tmp_d = tr[tl].down0) > 0) && (tr[tmp_d].up0 == tu))
            tr[tmp_d].up0 = tl;
            if (((tmp_d = tr[tl].down0) > 0) && (tr[tmp_d].up1 == tu))
            tr[tmp_d].up1 = tl;

            if (((tmp_d = tr[tl].down1) > 0) && (tr[tmp_d].up0 == tu))
            tr[tmp_d].up0 = tl;
            if (((tmp_d = tr[tl].down1) > 0) && (tr[tmp_d].up1 == tu))
            tr[tmp_d].up1 = tl;
            
            /* Now update the query structure and obtain the sinks for the */
            /* two trapezoids */ 
            
            i1 = qs.nextIndex();		/* Upper trapezoid sink */
            i2 = qs.nextIndex();		/* Lower trapezoid sink */
            sk = tr[tu].sink;
            
            qs[sk].type = NodeType::Y;
            qs[sk].yValue = s.end;
            qs[sk].segmentIndex = segmentIndex;	/* not really reqd ... maybe later */
            qs[sk].left = i2;
            qs[sk].right = i1;

            qs[i1].type = NodeType::Sink;
            qs[i1].trapezoidIndex = tu;
            qs[i1].parent = sk;

            qs[i2].type = NodeType::Sink;
            qs[i2].trapezoidIndex = tl;
            qs[i2].parent = sk;

            tr[tu].sink = i1;
            tr[tl].sink = i2;
            tlast = tu;
            }
        else				/* v1 already present */
            {       /* Get the lowermost intersecting trapezoid */
            tlast = locateEndpoint(s.end, s.begin, s.root1, seg, qs);
            tribot = 1;
            }
        
        /* Thread the segment into the query tree creating a new X-node */
        /* First, split all the trapezoids which are intersected by s into */
        /* two */

        t = tfirst;			/* topmost trapezoid */
        
        while ((t > 0) && 
            greaterThanEqualTo(tr[t].low, tr[tlast].low))
                        /* traverse from top to bot */
            {
            int t_sav, tn_sav;
            sk = tr[t].sink;
            i1 = qs.nextIndex();		/* left trapezoid sink */
            i2 = qs.nextIndex();		/* right trapezoid sink */
            
            qs[sk].type = NodeType::Y;
            qs[sk].segmentIndex = segmentIndex;
            qs[sk].left = i1;
            qs[sk].right = i2;

            qs[i1].type = NodeType::Sink;	/* left trapezoid (use existing one) */
            qs[i1].trapezoidIndex = t;
            qs[i1].parent = sk;

            qs[i2].type = NodeType::Sink;	/* right trapezoid (allocate new) */
            qs[i2].trapezoidIndex = tn = tr.nextIndex();
            tr[tn].state = true;
            qs[i2].parent = sk;

            if (t == tfirst)
            tfirstr = tn;
            if (&tr[t].low == &tr[tlast].low)
            tlastr = tn;

            tr[tn] = tr[t];
            tr[t].sink = i1;
            tr[tn].sink = i2;
            t_sav = t;
            tn_sav = tn;

            /* error */

            if ((tr[t].down0 <= 0) && (tr[t].down1 <= 0)) /* case cannot arise */
            {
                fprintf(stderr, "add_segment: error\n");
                break;
            }
            
            /* only one trapezoid below. partition t into two and make the */
            /* two resulting trapezoids t and tn as the upper neighbours of */
            /* the sole lower trapezoid */
            
            else if ((tr[t].down0 > 0) && (tr[t].down1 <= 0))
            {			/* Only one trapezoid below */
            if ((tr[t].up0 > 0) && (tr[t].up1 > 0))
                {			/* continuation of a chain from abv. */
                if (tr[t].up2 > 0) /* three upper neighbours */
                {
                if (tr[t].up2Side == Side::Left)
                    {
                    tr[tn].up0 = tr[t].up1;
                    tr[t].up1 = -1;
                    tr[tn].up1 = tr[t].up2;
                    
                    tr[tr[t].up0].down0 = t;
                    tr[tr[tn].up0].down0 = tn;
                    tr[tr[tn].up1].down0 = tn;
                    }
                else		/* intersects in the right */
                    {
                    tr[tn].up1 = -1;
                    tr[tn].up0 = tr[t].up1;
                    tr[t].up1 = tr[t].up0;
                    tr[t].up0 = tr[t].up2;

                    tr[tr[t].up0].down0 = t;
                    tr[tr[t].up1].down0 = t;
                    tr[tr[tn].up0].down0 = tn;		      
                    }
                
                tr[t].up2 = tr[tn].up2 = 0;
                }
                else		/* No usave.... simple case */
                {
                tr[tn].up0 = tr[t].up1;
                tr[t].up1 = tr[tn].up1 = -1;
                tr[tr[tn].up0].down0 = tn;
                }
                }
            else 
                {			/* fresh seg. or upward cusp */
                int tmp_u = tr[t].up0;
                int td0, td1;
                if (((td0 = tr[tmp_u].down0) > 0) && 
                ((td1 = tr[tmp_u].down1) > 0))
                {		/* upward cusp */
                if ((tr[td0].rightSegment > 0) &&
                    !isLeftOf(seg[tr[td0].rightSegment], s.end))
                    {
                    tr[t].up0 = tr[t].up1 = tr[tn].up1 = -1;
                    tr[tr[tn].up0].down1 = tn;
                    }
                else		/* cusp going leftwards */
                    { 
                    tr[tn].up0 = tr[tn].up1 = tr[t].up1 = -1;
                    tr[tr[t].up0].down0 = t;
                    }
                }
                else		/* fresh segment */
                {
                tr[tr[t].up0].down0 = t;
                tr[tr[t].up0].down1 = tn;
                }	      
                }
            
            if (tr[t].low == tr[tlast].low && tribot) {
                /* bottom forms a triangle */

                if (isSwapped)	
                tmptriseg = seg[segmentIndex].prev;
                else
                tmptriseg = seg[segmentIndex].next;
                
                if ((tmptriseg > 0) && isLeftOf(seg[tmptriseg], s.begin))
                {
                        /* L-R downward cusp */
                tr[tr[t].down0].up0 = t;
                tr[tn].down0 = tr[tn].down1 = -1;
                }
                else
                {
                        /* R-L downward cusp */
                tr[tr[tn].down0].up1 = tn;
                tr[t].down0 = tr[t].down1 = -1;
                }
                }
            else
                {
                if ((tr[tr[t].down0].up0 > 0) && (tr[tr[t].down0].up1 > 0))
                {
                if (tr[tr[t].down0].up0 == t) /* passes thru LHS */
                    {
                    tr[tr[t].down0].up2 = tr[tr[t].down0].up1;
                    tr[tr[t].down0].up2Side = Side::Left;
                    }
                else
                    {
                    tr[tr[t].down0].up2 = tr[tr[t].down0].up0;
                    tr[tr[t].down0].up2Side = Side::Right;
                    }		    
                }
                tr[tr[t].down0].up0 = t;
                tr[tr[t].down0].up1 = tn;
                }
            
            t = tr[t].down0;
            }


            else if ((tr[t].down0 <= 0) && (tr[t].down1 > 0))
            {			/* Only one trapezoid below */
            if ((tr[t].up0 > 0) && (tr[t].up1 > 0))
                {			/* continuation of a chain from abv. */
                if (tr[t].up2 > 0) /* three upper neighbours */
                {
                if (tr[t].up2Side == Side::Left)
                    {
                    tr[tn].up0 = tr[t].up1;
                    tr[t].up1 = -1;
                    tr[tn].up1 = tr[t].up2;
                    
                    tr[tr[t].up0].down0 = t;
                    tr[tr[tn].up0].down0 = tn;
                    tr[tr[tn].up1].down0 = tn;
                    }
                else		/* intersects in the right */
                    {
                    tr[tn].up1 = -1;
                    tr[tn].up0 = tr[t].up1;
                    tr[t].up1 = tr[t].up0;
                    tr[t].up0 = tr[t].up2;

                    tr[tr[t].up0].down0 = t;
                    tr[tr[t].up1].down0 = t;
                    tr[tr[tn].up0].down0 = tn;		      
                    }
                
                tr[t].up2 = tr[tn].up2 = 0;
                }
                else		/* No usave.... simple case */
                {
                tr[tn].up0 = tr[t].up1;
                tr[t].up1 = tr[tn].up1 = -1;
                tr[tr[tn].up0].down0 = tn;
                }
                }
            else 
                {			/* fresh seg. or upward cusp */
                unsigned int tmp_u = tr[t].up0;
                unsigned int td0, td1;
                if (((td0 = tr[tmp_u].down0) > 0) && 
                ((td1 = tr[tmp_u].down1) > 0))
                {		/* upward cusp */
                if ((tr[td0].rightSegment > 0) &&
                    !isLeftOf(seg[tr[td0].rightSegment], s.end))
                    {
                    tr[t].up0 = tr[t].up1 = tr[tn].up1 = -1;
                    tr[tr[tn].up0].down1 = tn;
                    }
                else 
                    {
                    tr[tn].up0 = tr[tn].up1 = tr[t].up1 = -1;
                    tr[tr[t].up0].down0 = t;
                    }
                }
                else		/* fresh segment */
                {
                tr[tr[t].up0].down0 = t;
                tr[tr[t].up0].down1 = tn;
                }
                }
            
            if (tr[t].low == tr[tlast].low && tribot) {
                /* bottom forms a triangle */
                unsigned int tmpseg;

                if (isSwapped)
                tmptriseg = seg[segmentIndex].prev;
                else
                tmptriseg = seg[segmentIndex].next;

                if ((tmpseg > 0) && isLeftOf(seg[tmpseg], s.begin))
                {
                /* L-R downward cusp */
                tr[tr[t].down1].up0 = t;
                tr[tn].down0 = tr[tn].down1 = -1;
                }
                else
                {
                /* R-L downward cusp */
                tr[tr[tn].down1].up1 = tn;
                tr[t].down0 = tr[t].down1 = -1;
                }
                }		
            else
                {
                if ((tr[tr[t].down1].up0 > 0) && (tr[tr[t].down1].up1 > 0))
                {
                if (tr[tr[t].down1].up0 == t) /* passes thru LHS */
                    {
                    tr[tr[t].down1].up2 = tr[tr[t].down1].up1;
                    tr[tr[t].down1].up2Side = Side::Left;
                    }
                else
                    {
                    tr[tr[t].down1].up2 = tr[tr[t].down1].up0;
                    tr[tr[t].down1].up2Side = Side::Right;
                    }		    
                }
                tr[tr[t].down1].up0 = t;
                tr[tr[t].down1].up1 = tn;
                }
            
            t = tr[t].down1;
            }

            /* two trapezoids below. Find out which one is intersected by */
            /* this segment and proceed down that one */
            
            else
            {
            unsigned int tmpseg = tr[tr[t].down0].rightSegment;
            double y0, yt;
            libMesh::Point2 tmppt;
            unsigned int tnext, i_d0, i_d1;

            i_d0 = i_d1 = false;
            if (libMesh::Math::Equals(tr[t].low.y, s.begin.y))
                {
                if (tr[t].low.x > s.begin.x)
                i_d0 = true;
                else
                i_d1 = true;
                }
            else
                {
                tmppt.y = y0 = tr[t].low.y;
                yt = (y0 - s.begin.y)/(s.end.y - s.begin.y);
                tmppt.x = s.begin.x + yt * (s.end.x - s.begin.x);
                
                if (lessThan(tmppt, tr[t].low))
                i_d0 = true;
                else
                i_d1 = true;
                }
            
            /* check continuity from the top so that the lower-neighbour */
            /* values are properly filled for the upper trapezoid */

            if ((tr[t].up0 > 0) && (tr[t].up1 > 0))
                {			/* continuation of a chain from abv. */
                if (tr[t].up2 > 0) /* three upper neighbours */
                {
                if (tr[t].up2Side == Side::Left)
                    {
                    tr[tn].up0 = tr[t].up1;
                    tr[t].up1 = -1;
                    tr[tn].up1 = tr[t].up2;
                    
                    tr[tr[t].up0].down0 = t;
                    tr[tr[tn].up0].down0 = tn;
                    tr[tr[tn].up1].down0 = tn;
                    }
                else		/* intersects in the right */
                    {
                    tr[tn].up1 = -1;
                    tr[tn].up0 = tr[t].up1;
                    tr[t].up1 = tr[t].up0;
                    tr[t].up0 = tr[t].up2;

                    tr[tr[t].up0].down0 = t;
                    tr[tr[t].up1].down0 = t;
                    tr[tr[tn].up0].down0 = tn;		      
                    }
                
                tr[t].up2 = tr[tn].up2 = 0;
                }
                else		/* No usave.... simple case */
                {
                tr[tn].up0 = tr[t].up1;
                tr[tn].up1 = -1;
                tr[t].up1 = -1;
                tr[tr[tn].up0].down0 = tn;
                }
                }
            else 
                {			/* fresh seg. or upward cusp */
                int tmp_u = tr[t].up0;
                int td0, td1;
                if (((td0 = tr[tmp_u].down0) > 0) && 
                ((td1 = tr[tmp_u].down1) > 0))
                {		/* upward cusp */
                if ((tr[td0].rightSegment > 0) &&
                    !isLeftOf(seg[tr[td0].rightSegment], s.end))
                    {
                    tr[t].up0 = tr[t].up1 = tr[tn].up1 = -1;
                    tr[tr[tn].up0].down1 = tn;
                    }
                else 
                    {
                    tr[tn].up0 = tr[tn].up1 = tr[t].up1 = -1;
                    tr[tr[t].up0].down0 = t;
                    }
                }
                else		/* fresh segment */
                {
                tr[tr[t].up0].down0 = t;
                tr[tr[t].up0].down1 = tn;
                }
                }
            
            if (tr[t].low == tr[tlast].low && tribot) {
                /* this case arises only at the lowest trapezoid.. i.e.
                tlast, if the lower endpoint of the segment is
                already inserted in the structure */
                
                tr[tr[t].down0].up0 = t;
                tr[tr[t].down0].up1 = -1;
                tr[tr[t].down1].up0 = tn;
                tr[tr[t].down1].up1 = -1;

                tr[tn].down0 = tr[t].down1;
                tr[t].down1 = tr[tn].down1 = -1;
                
                tnext = tr[t].down1;
                }
            else if (i_d0)
                        /* intersecting d0 */
                {
                tr[tr[t].down0].up0 = t;
                tr[tr[t].down0].up1 = tn;
                tr[tr[t].down1].up0 = tn;
                tr[tr[t].down1].up1 = -1;
                
                /* new code to determine the bottom neighbours of the */
                /* newly partitioned trapezoid */
                
                tr[t].down1 = -1;

                tnext = tr[t].down0;
                }
            else			/* intersecting d1 */
                {
                tr[tr[t].down0].up0 = t;
                tr[tr[t].down0].up1 = -1;
                tr[tr[t].down1].up0 = t;
                tr[tr[t].down1].up1 = tn;

                /* new code to determine the bottom neighbours of the */
                /* newly partitioned trapezoid */
                
                tr[tn].down0 = tr[t].down1;
                tr[tn].down1 = -1;
                
                tnext = tr[t].down1;
                }	    
            
            t = tnext;
            }
            
            tr[t_sav].rightSegment = tr[tn_sav].leftSegment = segmentIndex;
            } /* end-while */
        
        /* Now combine those trapezoids which share common segments. We can */
        /* use the pointers to the parent to connect these together. This */
        /* works only because all these new trapezoids have been formed */
        /* due to splitting by the segment, and hence have only one parent */

        tfirstl = tfirst; 
        tlastl = tlast;
        mergeTrapezoids(segmentIndex, tfirstl, tlastl, Side::Left, qs, tr);
        mergeTrapezoids(segmentIndex, tfirstr, tlastr, Side::Right, qs, tr);

        seg[segmentIndex].isInserted = true;
        return 0;
    }

    /* Update the roots stored for each of the endpoints of the segment.
    * This is done to speed up the location-query for the endpoint when
    * the segment is inserted into the trapezoidation subsequently
    */
    void findNewRoots(SegmentStructure &seg, unsigned int segmentNumber, QueryStructure &qs, TrapezoidStructure &tr) {
        Segment &s = seg[segmentNumber];
        if (s.isInserted) {
            return;
        }

        s.root0 = locateEndpoint(s.begin, s.end, s.root0, seg, qs);
        s.root0 = tr[s.root0].sink;

        s.root1 = locateEndpoint(s.end, s.begin, s.root1, seg, qs);
        s.root1 = tr[s.root1].sink;

    }

    void constructTrapezoids(SegmentStructure &seg, unsigned int numSegments, QueryStructure &qs, TrapezoidStructure &trap) {
        unsigned int root = initQueryStructure(seg, seg.chooseSegment(), qs, trap);
        unsigned int i;

        for(i = 1; i <= numSegments; i++) {
            seg[i].root0 = seg[i].root1 = root;
        }
        for(unsigned int h = 1; h <= mathLogStarN(numSegments); h++) {
            for (i = mathN(numSegments, h - 1) + 1; i < mathN(numSegments, h); i++) {
                addSegment(seg, seg.chooseSegment(), qs, trap);
            }
            for (i = 1; i <= numSegments; i++) {
                findNewRoots(seg, i, qs, trap);
            }
        }
        for (i = mathN(numSegments, mathLogStarN(numSegments)) + 1; i <= numSegments; i++) {
            addSegment(seg, seg.chooseSegment(), qs, trap);
        }
    }

    unsigned int monotonateTrapezoids(TrapezoidStructure &tr, SegmentStructure &seg, unsigned int numSegments) {
        return 0u;
    }

    libMesh::Mesh *earClipping(libMesh::PolyLine &polygon) {
        return nullptr;
    }

    libMesh::Mesh *siedelTriangulation(libMesh::PolyLine &polygon) {
        // Using Siedel 1991 algorithm
        // See: http://gamma.cs.unc.edu/SEIDEL/

        unsigned int nPoints = polygon.count();
        SegmentStructure seg = SegmentStructure(nPoints);
        QueryStructure qs = QueryStructure(8 * nPoints);
        TrapezoidStructure trap = TrapezoidStructure(4 * nPoints);

        unsigned int contours = nPoints;
        unsigned int n = initializeSegments(seg, polygon, &contours, 1u);
        initQueryStructure(seg, n, qs, trap);

        // Step 1: Trapezoidation
        constructTrapezoids(seg, n, qs, trap);
        // Step 2: Generation of monotome mountains
        unsigned int nMonpoly = monotonateTrapezoids(trap, seg, n);
        // Step 3: Ear clipping
        return earClipping(polygon);
    }
}