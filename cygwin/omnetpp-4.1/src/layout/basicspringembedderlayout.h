//==========================================================================
//  BASICSPRINGEMBEDDERLAYOUT.H - part of
//                     OMNeT++/OMNEST
//            Discrete System Simulation in C++
//
//  Author: Andras Varga
//
//==========================================================================

/*--------------------------------------------------------------*
  Copyright (C) 2002-2008 Andras Varga
  Copyright (C) 2006-2008 OpenSim Ltd.

  This file is distributed WITHOUT ANY WARRANTY. See the file
  `license' for details on this and other legal matters.
*--------------------------------------------------------------*/

#ifndef __BASICSPRINGEMBEDDERLAYOUT_H
#define __BASICSPRINGEMBEDDERLAYOUT_H

#include <vector>
#include <string>

#include "graphlayouter.h"

NAMESPACE_BEGIN


/**
 * Implementation of the Spring Embedder algorithm. This class is the layouter
 * used in OMNeT++ 3.x.
 *
 * Simplifications:
 *  - ignores node sizes (this is visible when item is very long, e.g.
 *    500 x 10 pixels)
 *  - ignores connections to the parent module (nodes which are connected
 *    to the parent may end up in the middle of the layout)
 */
class LAYOUT_API BasicSpringEmbedderLayout : public GraphLayouter
{
  protected:
    struct Anchor
    {
        std::string name; // anchor name
        double x, y;      // position
        int refcount;     // how many nodes are anchored to it
        double x1off, y1off, x2off, y2off; // bounding box of anchored nodes, relative to (x,y)
        double vx, vy;    // internal: distance moved at each step (preserved between relax() calls)
    };

    struct Node
    {
        cModule *key;      // node "handle"
        bool fixed;        // allowed to move?
        Anchor *anchor;    // non-NULL for anchored nodes
        double x, y;       // position (of the center of the shape)
        int offx, offy;    // anchored nodes: offset to anchor point (and x,y are calculated)
        int sx, sy;        // half width/height

        double vx, vy;     // internal: distance moved at each step (preserved between relax() calls)
        int color;         // internal: connected nodes share the same color
        bool connectedToFixed; // internal: whether the node is connected (maybe indirectly) to a fixed node
    };

    struct Edge
    {
        Node *src;
        Node *target;
        double len;        // preferred length; making it "double" reduces the number of int->double conversions during layouting
    };

    // AnchorList and NodeList must be vectors because iteration on std::map is very slow
    typedef std::vector<Anchor*> AnchorList;
    typedef std::vector<Node*> NodeList;
    typedef std::vector<Edge> EdgeList;

    AnchorList anchors;
    NodeList nodes;
    EdgeList edges;

    int defaultEdgeLen;

    bool haveFixedNode;
    bool haveAnchoredNode;
    bool allNodesAreFixed;

    double minx, miny, maxx, maxy;

    int maxIterations;
    double repulsiveForce;
    double attractionForce;

  protected:
    // utility
    Node *findNode(cModule *mod);

    // mark connected nodes with same color; return number of colors used
    virtual int doColoring();

    // fill in the x1off, y1off, x2off, y2off fields of the anchors
    virtual void computeAnchorBoundingBoxes();

    // assign an initial layout that relax() will improve
    virtual void assignInitialPositions();

    // fill in the connectedToFixed fields of nodes
    virtual void markNodesConnectedToFixed(int numColors);

    // calculate bounding box (x1,y1,x2,y2)
    virtual void computeBoundingBox(double& x1, double& y1, double& x2, double& y2, bool (*predicate)(Node*));

    // main algorithm (modified spring embedder)
    virtual double relax();

    // for debugging: draw whole thing in a window
    void debugDraw(int step);

    // predicates for computeBoundingBox()
    static bool anyNode(Node *n) {return true;}
    static bool isFixedNode(Node *n) {return n->fixed;}
    static bool isNotConnectedToFixed(Node *n) {return !n->connectedToFixed;}

  public:
    /**
     * Ctor, dtor
     */
    //@{
    BasicSpringEmbedderLayout();
    virtual ~BasicSpringEmbedderLayout();
    //@}

    /** @name Redefined GraphLayouter methods */
    //@{
    virtual void setEnvironment(GraphLayouterEnvironment *environment);
    void addMovableNode(cModule *mod, int width, int height);
    void addFixedNode(cModule *mod, int x, int y, int width, int height);
    void addAnchoredNode(cModule *mod, const char *anchorname, int offx, int offy, int width, int height);
    void addEdge(cModule *src, cModule *target, int len=0);
    void addEdgeToBorder(cModule *src, int len=0);
    virtual void execute();
    void getNodePosition(cModule *mod, int& x, int& y);
    //@}
};

NAMESPACE_END


#endif



