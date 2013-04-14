// Christine Talbot
//
// Class Name(s):
//		AI
//
// Description:
//		Implementation of AI algorithms and supporting methods such as A*
//
// Method Summary:
// 		AI();
//			dummy constructor
//		~AI();
//			dummy destructor
//		void createGraph();
//			reads all the polygons, start point, and end point and creates a
//			graph connecting all points that are within sight of each other
//		void init(GLWidget::Point a, GLWidget::Point b, vector<GLWidget::Polygon> c);
//			initializes the globals for this class, such as the graph & other vectors
//		void runAstar(GLWidget::Point a, GLWidget::Point b, vector<GLWidget::Polygon> c);
//			runs the A* algorithm to find the shortest path using the graph that was generated
//		void runLRTAstar(GLWidget::Point a, GLWidget::Point b, vector<GLWidget::Polygon> c);
//			not used currently
//		void converttolines();
//			takes every point of the polygons & start/goal points and connects them to determine
//			which points are within direct site of each other
//		bool checkintersections(GLWidget::Line a);
//			checks to see if this line between two random points in the environment are 
//			intersected by another polygon
//		float min(float a, float b);
//			returns whichever one is less
//		float max(float a, float b);
//			returns whichever one is greater
//		bool intersect(GLWidget::Point p1, GLWidget::Point p2, GLWidget::Point p3, GLWidget::Point p4);
//			determines if the lines from p1 to p2 and p3 to p4 intersect each other or not
//		int Astar();
//			A* algorithm that uses the graph to find the shortest path from the start to the goal point
//		int LRTAstar();
//			not used currently
//		void generateSuccessors(int s);
//			uses the generated graph to determine points that are within site of the current point
//		void checkfordupes(int anc, int newpt );
//			checks to see if any of the successors are already in the queue or in the ancestors
//		float diffclock(clock_t clock1,clock_t clock2)
//			calculates duration times in milliseconds
//
// ---------------------------------------------------------------------------------------------
#include "ai.h"
#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include "glWidget.h"
#include <iostream>
#include <vector>
#include "PQ.h"
#include "State.h"
#include <cfloat>
#include <map>
#include "mainWindow.h"
#include <time.h>


GLWidget::Point AI::start1;
GLWidget::Point AI::goal1;
vector<GLWidget::Polygon> AI::poly_vec1;
vector<GLWidget::Line> AI::line_vec1; // to hold all the lines that represent edges of polygons
vector<GLWidget::Point> AI::point_vec1; // to hold all the points that are parts of the polygons
Graph::Graph AI::g; // holds the graph to help with successors
PQ::PQ AI::pq; // priority queue that was manually written
vector<GLWidget::Point> GLWidget::Astarptvector; // list of points to represent the solution of the A* algorithm
vector<GLWidget::Point> GLWidget::LRTAstarptvector; // not used
map<int, State*> AI::closed; // helps with pointers - keeps a record of all the states / nodes generated with the A* algorithm
int AI::numclosed;



AI::AI()
{
	//current = this;
}

AI::~AI()
{

}

void AI::createGraph()
{
	// create empty graph
	
	
	// add all the vertices to the graph
	g.addNode(start1.x, start1.y);
	g.addNode(goal1.x, goal1.y);
	//add all points from the polygons
	for(vector<GLWidget::Polygon>::iterator i=poly_vec1.begin(); i != poly_vec1.end(); i++)
	{
		GLWidget::Polygon p = *i;
		if(i != poly_vec1.end())
		{
			for(vector<GLWidget::Point>::iterator j = p.pts.begin(); j != p.pts.end(); j++)
			{
				GLWidget::Point pt = *j;
				if(j != p.pts.end())
					g.addNode(pt.x, pt.y);
			}
		}
	}

	// now let's add all our edges that are within sight of each other
	converttolines();

	for (vector<GLWidget::Point>::iterator k=point_vec1.begin();k!=point_vec1.end();k++)
	{
		if (k!=point_vec1.end())
		{
			GLWidget::Point p1 = *k;
			for(vector<GLWidget::Point>::iterator m=point_vec1.begin();m!=point_vec1.end();m++)
			{
				if(m!=point_vec1.end() and m!=k)
				{
					GLWidget::Point p2 = *m;
					if (!(p1.x == p2.x and p1.y == p2.y))
					{
						GLWidget::Line l;
						l.pt1.x = p1.x;
						l.pt1.y = p1.y;
						l.pt2.x = p2.x;
						l.pt2.y = p2.y;
						if (!(checkintersections(l)))
						{
							// two points are within sight of each other, so let's add them as an edge
							
							g.addEdge(p1.x, p1.y, p2.x, p2.y);
							
						}
					}
				}
			}
		}
	}
	


}

float diffclock(clock_t clock1,clock_t clock2)
{
	float diffticks=clock1-clock2;
	float diffms=(diffticks*1000)/CLOCKS_PER_SEC; // calcs ms for duration
	return diffms;
} 


void AI::runAstar(GLWidget::Point a, GLWidget::Point b, vector<GLWidget::Polygon> c)
{

	GLWidget::Astarptvector.resize(0);
	GLWidget::Astarptvector.clear();
	MainWindow::getTheApp()->setALabel (0.0, 0.0);
	// start timer
	// run A*
	clock_t begin=clock();
	int res = Astar();
	clock_t end=clock();
	float elapsedtime = diffclock(end,begin);
	float dist = closed[res]->getfn();
	// end timer
	int ctr = 0;
	// draw & save results
	while (closed[res]->getparent() != -1)
	{
		GLWidget::Point p;
		p.x = closed[res]->getx();
		p.y = closed[res]->gety();
		GLWidget::Astarptvector.push_back(p);
		res = closed[res]->getparent();
		ctr++;
	}
	if(closed[res]->gethn() != FLT_MAX)
	{
		GLWidget::Point t;
		t.x = closed[res]->getx();
		t.y = closed[res]->gety();
		GLWidget::Astarptvector.push_back(t);
	}
	MainWindow::getTheApp()->setALabel (elapsedtime, dist);

}

void AI::runLRTAstar(GLWidget::Point a, GLWidget::Point b, vector<GLWidget::Polygon> c)
{

	MainWindow::getTheApp()->setLRTALabel (0.0, 0.0);
	
	GLWidget::LRTAstarptvector.resize(0);
	GLWidget::LRTAstarptvector.clear();
	// start timer
	clock_t begin=clock();
	// run LRTA*
	int res = LRTAstar();
	// end timer
	clock_t end=clock();
	float elapsedtime = diffclock(end,begin);
	float dist = 0;//closed[res]->getfn();
	// draw & save results
	MainWindow::getTheApp()->setLRTALabel (elapsedtime, dist);

}
void AI::init(GLWidget::Point a, GLWidget::Point b, vector<GLWidget::Polygon> c)
{
	// initialize variables & create graph
	start1.x = a.x;
	start1.y = a.y;
	goal1.x = b.x;
	goal1.y = b.y;
	poly_vec1.resize(0);
	poly_vec1.clear(); 
	poly_vec1 = c;
	g.~Graph();
	g = Graph();
	createGraph();
}

void AI::converttolines()
{
	// for each polygon, add points to the point vector and generate all the lines with that point
	for(vector<GLWidget::Polygon>::iterator i = poly_vec1.begin(); i != poly_vec1.end(); i++)
	{
		if (i != poly_vec1.end())
		{
			GLWidget::Polygon p = *i;
			for (vector<GLWidget::Point>::iterator j = p.pts.begin(); j != p.pts.end(); j++)
			{
				if (j != p.pts.end())
				{
					GLWidget::Point pt = *j;
				
					point_vec1.push_back(pt);
					
					GLWidget::Line l;
					if (j+1 == p.pts.end())
					{
						l.pt1.x = pt.x;
						l.pt1.y = pt.y;
						l.pt2.x = (*(p.pts.begin())).x;
						l.pt2.y = (*(p.pts.begin())).y;
						line_vec1.push_back(l);
				
					}
					else
					{
						l.pt1.x = pt.x;
						l.pt1.y = pt.y;
						l.pt2.x = (*(j+1)).x;
						l.pt2.y = (*(j+1)).y;
						line_vec1.push_back(l);
				
					}
				}
			}
		}
	}	
	point_vec1.push_back(start1);

	point_vec1.push_back(goal1);

}

bool AI::checkintersections(GLWidget::Line a)
{
	// go through all the possible lines between points in the environment
	for(vector<GLWidget::Line>::iterator i = line_vec1.begin(); i != line_vec1.end(); i++)
	{
		// if any polygon line intersects this line AND isn't one of the points of the line, then return false
		if (i != line_vec1.end())
		{
			GLWidget::Line l = *i;
			
			if(!((l.pt1.x == a.pt1.x and l.pt1.y == a.pt1.y) or (l.pt2.x == a.pt2.x and l.pt2.y == a.pt2.y) or (l.pt1.x == a.pt2.x and l.pt1.y == a.pt2.y) or (l.pt2.x == a.pt1.x and l.pt2.x == a.pt1.y)))
			{
				// not one of the endpoints, so check for an intersection now
				
				if ((intersect(l.pt1, l.pt2, a.pt1, a.pt2)))
					return true; // exit since there is an intersection
				
			}
		}
		
	}
	return false;
}

bool AI::intersect(GLWidget::Point p1, GLWidget::Point p2, GLWidget::Point p3, GLWidget::Point p4)
{
	// calculate slope & intersects to create equations
	float slope1 = (p1.y - p2.y) / (p1.x-p2.x);
	float slope2 = (p3.y - p4.y) / (p3.x - p4.x);
	float yint1 = p1.y - (slope1 * p1.x);
	float yint2 = p3.y - (slope2 * p3.x);
	// determine if intersection
	if ((slope1 - slope2) == 0 and yint2 != yint1) // as long as not parallel & same line
		return false;
	float xintersect = (yint2 - yint1) / (slope1 - slope2);

	if ((xintersect < min(p1.x, p2.x) or xintersect > max(p1.x, p2.x)) or xintersect < min(p3.x, p4.x) or xintersect > max(p3.x, p4.x))
		return false;
	return true;
	
}

float AI::min(float a, float b)
{
	// return whichever is less than the other
	if (a < b)
		return a;
	if (b < a)
		return b;
	return a;
}

float AI::max(float a, float b)
{
	// return whichever is more than the other
	if (a < b)
		return b;
	if (b < a)
		return a;
	return a;
}


void AI::generateSuccessors(int s)
{
	// go through each edge for this node in the graph
	Graph::NodeEdges ne = g.getNode(closed[s]->getx(), closed[s]->gety());
	for(vector<Edge>::iterator i = ne.e.begin(); i!= ne.e.end(); i++)
	{
		if(i != ne.e.end())
		{
			// add to map for pointer references
			numclosed++;
			closed[numclosed] = new State(i->getend().getx(), i->getend().gety(), s, numclosed);
			// check if already an ancestor or in queue, else add to queue
			checkfordupes(s, numclosed);
		}
	}
}
void AI::checkfordupes(int anc, int newpt )
{
	// find State in Graph

	// add node for each edge's end point to queue if not already in queue
	int ctr = 0;
	int tracker = 0;
	int ancestor = anc;//State();
	bool match = false;
	bool inqueue = false;

	{
		{
			match = false;
			inqueue = false;
			ctr = 0;
			while ((ancestor != -1) and !match) // go through all ancestors
			{
				if(closed[newpt]->getx() == closed[ancestor]->getx() and closed[newpt]->gety() == closed[ancestor]->gety())
				{
				
					match = true;
					break;
				}
				// get next ancestor in the chain
				ancestor = closed[ancestor]->getparent();
				ctr++;
			}
			
		
			if (!match) // not found in ancestors, so check queue
			{
			
				// go through each item in the queue
				State addstate;
				for(vector<int>::iterator i = pq.slist.begin(); i != pq.slist.end(); i++)
				{
					if (i != pq.slist.end())
					{
						int s2 = *i;
						// if find the new node in the queue
						if (closed[s2]->getx() == closed[newpt]->getx() and closed[s2]->gety() == closed[newpt]->gety())
						{
							inqueue = true;

							numclosed++;
							closed[numclosed] = new State(closed[newpt]->getx(), closed[newpt]->gety(), anc, numclosed);
							// check if the queue costs less than this one or not
							if(closed[numclosed]->getgn() < closed[s2]->getgn())
							{
								// remove s2 from queue
															
								pq.push(numclosed);
								
								pq.erase(s2);
								// and add addstate
								
							}
							// else do nothing / don't add addstate since costs more
							break;
						}
					}
				}
				if (!inqueue) // not in queue and not in ancestors, so add
				{
				
					numclosed++;
					closed[numclosed] = new State(closed[newpt]->getx(), closed[newpt]->gety(), anc, numclosed);
					
					pq.push(numclosed);
					
					
				}
			}
		}
		tracker++;
	}
	

}

int AI::LRTAstar()
{
	// not completed yet


}

int AI::Astar()
{
	     // A * algorithm :

     State::goal.x = goal1.x;
     State::goal.y = goal1.y;

     closed.clear();
     numclosed = -1;

      // 1. Make a node INIT for the initial state;
 
       numclosed++;
      closed[numclosed] = new State(start1.x, start1.y, -1, numclosed);
     
      // 2. Set the queue NODES to contain INIT;
      pq.push( numclosed );

      int BestNode = -1;
      // initialize which node is found to be the solution
      int ExpandNode = -1;
      // initialize which node to expand in this round

      // 3. Loop :
      while ( pq.size() != 0 && BestNode == -1 )
      {
         // if NODES is empty then report failure and exit loop

         // move BESTNODE ( which has the lowest f') from the front of NODES;
         
         
         ExpandNode = pq.slist[0];
        
         
         pq.erase(ExpandNode);
         // get first node from the NODES queue
         

         // if BESTNODE is the goal node then
         // return BESTNODE and exit loop /* * a solution * */
         if ( closed[ExpandNode]->isgoal() )
         {
            // if the node to be expanded is the goal, then stop - loop will break out
            BestNode = ExpandNode;
         }
         else
         {

            // generate the successor nodes of BESTNODE;
            
            generateSuccessors( ExpandNode);

         }
         // end; /* * if not goal * */
      }
      // end; /* * loop * */

      return BestNode;
      // if no solution found, but queue is empty, then this will be null


}

