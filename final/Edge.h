// Christine Talbot
//
// Class Name(s):
//		Edge
//
// Description:
//		Implementation of Edge class which provides connections between
//		the points in the graph which are in-sight of each other
//
// Method Summary:
// 		Edge();
//			default constructor - not used
//		Edge(float x1, float y1, float x2, float y2);
//			constructor that connects the point at (x1,y1) to (x2,y2)
//		virtual ~Edge();
//			destructor
//		float getweight();
//			returns the distance from the point (x1,y1) to (x2,y2) for this edge
//		Node getstart();
//			returns the starting node for the edge (x1,y1)
//		Node getend();
//			returns the endingn node for the edge (x2,y2)
//		Edge& operator=(const Edge &e);
//			overloads the = operator to allow assigning of Edges to new Edges
//		string toString();
//			for debugging - prints out the edge information
//
// ---------------------------------------------------------------------------------------------

#ifndef EDGE_H_
#define EDGE_H_

#include <string>
#include "Node.h"
using namespace std;

class Edge {
	private:
		Node start;
		Node end;
		float weight;
	public:
		Edge();
		Edge(float x1, float y1, float x2, float y2);
		virtual ~Edge();
		float getweight();
		Node getstart();
		Node getend();
		Edge& operator=(const Edge &e);
		string toString();
};

#endif /* EDGE_H_ */
