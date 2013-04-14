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

#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <string>
#include <sstream>
#include <cfloat>
#include <vector>
#include <iostream>
using namespace std;

Graph::Graph()
{
	// clears & sets up the graph
	glist.clear();
	numnodes = 0;
	numedges = 0;

}

Graph::~Graph()
{
	// destroys the graph
	glist.resize(0);
	glist.clear();

}

void Graph::addNode(float x, float y)
{
	// creates the node with these values
	Node n = Node(x, y);
	NodeEdges res;// = NodeEdge();
	res.n = n;
	res.e.resize(0);
	res.e.clear();
	// adds it to the list of nodes in the graph
	glist.push_back(res);
	numnodes++;
}
void Graph::addEdge(float x1, float y1, float x2, float y2)
{
	int ctr = 0;
	Edge e = Edge(x1, y1, x2, y2);

	// find node1 & add edge
	for (vector<NodeEdges>::iterator i = glist.begin(); i != glist.end(); i++)
	{
		if(i != glist.end())
		{
			NodeEdges n = *i;

			if (e.getstart() == n.n)
			{

				glist[ctr].e.push_back(Edge(x1,y1,x2,y2));

				break;
			}
		}
		ctr++;
	}
	ctr = 0;
	// find node2 & add edge
	for (vector<NodeEdges>::iterator i = glist.begin(); i != glist.end(); i++)
	{
		if(i != glist.end())
		{
			NodeEdges n = *i;

			if (e.getend() == n.n)
			{

				glist[ctr].e.push_back(Edge(x2,y2,x1,y1));
				break;
			}
		}
		ctr++;
	}
	numedges++;
}
Node Graph::getMinSuccessor(float x, float y)
{
	
	Node n = Node(x,y);
	float curmincost = FLT_MAX;
	Node retnode;
	// go through each node in the graph
	for(vector<NodeEdges>::iterator i = glist.begin(); i != glist.end(); i++)
	{
		if (i != glist.end())
		{
			NodeEdges ne = *i;
			if (n == ne.n)
			{
				// found the node, so let's iterate through the successors
				for (vector<Edge>::iterator e = ne.e.begin(); e != ne.e.end(); e++)
				{
					if (e != ne.e.end())
					{
						Edge ed = *e;
						// check which successor has the min cost
						if (ed.getweight() < curmincost)
						{
							curmincost = ed.getweight();
							if(ed.getstart() == n)
								retnode = ed.getend();
							else
								retnode = ed.getstart();
						}
					}
				}
			}
		}
	}
	if (curmincost != FLT_MAX)
	{
		return retnode;
	}
	else // shouldn't ever hit this
	{
		//return (Node)NULL;
	}
}

Graph::NodeEdges Graph::getNode(float x, float y)
{
	// go through the nodes in the graph and find the one at this point
	for (vector<NodeEdges>::iterator i = glist.begin(); i != glist.end(); i++)
	{
		if (i != glist.end())
		{
			NodeEdges ne = *i;
			if (ne.n.getx() == x and ne.n.gety() == y)
				return ne;
		}
	}
}
string Graph::toString()
{
	// formats the graph to a pretty output for debugging
	stringstream retstring;
	retstring << "Graph has " << numnodes << " nodes and " << numedges << " edges.\n";
	for(vector<NodeEdges>::iterator i = glist.begin(); i != glist.end(); i++)
	{
		if (i != glist.end())
		{
			NodeEdges ne = *i;
			retstring << "Node:\n" << ne.n.toString() << "\n\nEdges:\n";
			for(vector<Edge>::iterator e = ne.e.begin(); e!= ne.e.end(); e++)
			{
				if (e!= ne.e.end())
				{
					Edge ed = *e;
					retstring << ed.toString() << "\n";
				}
			}
			retstring << "==============\n";
		}
	}
	if(retstring.str().empty())
	{
		retstring << "No data in graph.\n";
	}
	return retstring.str();
}
