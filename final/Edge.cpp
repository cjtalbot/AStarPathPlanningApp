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

#include "Edge.h"
#include "Node.h"
#include <math.h>
#include <string>
#include <sstream>
using namespace std;

Edge::Edge(float x1, float y1, float x2, float y2) {
	// set the starting and ending point to the values given
	start = Node::Node(x1, y1);
	end = Node::Node(x2, y2);
	// calculate the true distance between the two points for the weight of the edge
	weight = fabs(sqrt(((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1))));
}

Edge::~Edge() {
	// default destructor
}
Edge::Edge()
{
	// do nothing
}

float Edge::getweight()
{
	return weight;
}
Node Edge::getstart()
{
	return start;
}
Node Edge::getend()
{
	return end;
}
Edge& Edge::operator=(const Edge &e)
{
	// copy e to this object
	if(this == &e)
		return *this;
	start = e.start;
	end = e.end;
	weight = e.weight;
	return *this;
}

string Edge::toString()
{
	// formats the edge data for printing
	stringstream retstring;
	retstring << "[" << start.toString() << ", " << end.toString() << "]";
	return retstring.str();
}
