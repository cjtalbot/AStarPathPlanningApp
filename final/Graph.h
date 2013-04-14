// Christine Talbot
//
// Class Name(s):
//		Graph
//
// Description:
//		Implementation of Graph class which provides the graph to help
//		identify successors for the A* algorithm
//
// Method Summary:
// 		Graph();
//			constructor
//		virtual ~Graph();
//			destructor
//		void addNode(float x, float y);
//			adds a new node to the existing graph
//		void addEdge(float x1, float y1, float x2, float y2);
//			finds the starting and ending nodes & adds an edge to both to
//			represent they are successors of each other
//		Node getMinSuccessor(float x, float y);
//			finds the min f(n) for all successors of the node at this point
//		NodeEdges getNode(float x, float y);
//			returns the node with this point
//		string toString();
//			prints the graph in a pretty format
//
// -------------------------------------------------------------------------------------------

#ifndef GRAPH_H_
#define GRAPH_H_

#include <string>
#include "Node.h"
#include "Edge.h"
#include <vector>
using namespace std;

class Graph {

	public:
		Graph();
		virtual ~Graph();
		struct NodeEdges
		{
			Node n;
			vector<Edge> e;
		};
		void addNode(float x, float y);
		void addEdge(float x1, float y1, float x2, float y2);
		Node getMinSuccessor(float x, float y);
		NodeEdges getNode(float x, float y);
		string toString();
	
	private:
		int numnodes;
		int numedges;
	
		vector<NodeEdges> glist;
};

#endif /* GRAPH_H_ */
