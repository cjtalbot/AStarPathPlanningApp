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
//
// ---------------------------------------------------------------------------------------------
#ifndef AI_H
#define AI_H

#include "glWidget.h"
#include "Graph.h"
#include "State.h"
#include "PQ.h"


class AI
{
	public:
		AI();
		~AI();
		void createGraph();
		static AI *current;
		void init(GLWidget::Point a, GLWidget::Point b, vector<GLWidget::Polygon> c);
		void runAstar(GLWidget::Point a, GLWidget::Point b, vector<GLWidget::Polygon> c);
		void runLRTAstar(GLWidget::Point a, GLWidget::Point b, vector<GLWidget::Polygon> c);
		void converttolines();
		bool checkintersections(GLWidget::Line a);
		float min(float a, float b);
		float max(float a, float b);
		bool intersect(GLWidget::Point p1, GLWidget::Point p2, GLWidget::Point p3, GLWidget::Point p4);
		static Graph::Graph g;
		static GLWidget::Point start1;
		static GLWidget::Point goal1;
		static vector<GLWidget::Polygon> poly_vec1;
		static vector<GLWidget::Line> line_vec1; 
		static vector<GLWidget::Point> point_vec1; 
		int Astar();
		int LRTAstar();
		void generateSuccessors(int s);
		void checkfordupes(int anc, int newpt );
		static PQ::PQ pq;
		static map<int, State*> closed;
		static int numclosed;
};
#endif // AI_H
