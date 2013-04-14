// Christine Talbot
//
// Class Name(s):
//		GLWidget
//
// Description:
//		Implementation of GLWidget class which is inherited from the
//			Fl_Gl_Window class for viewing & drawing in the 
//			application
//
// Method Summary:
// 		GLWidget( int x, int y, int w, int h, const char *l ): 
//			Fl_Gl_Window( x, y, w, h, l )
//				Constructor that creates a window
//		void init()
//				Initialization method to setup the window & 
//				object coordinates and related clipwindows
//		void setcolor ( DrawColor col_val )
//				Changes the drawing color for objects
//		float distance(float x0, float y0, float x1, float y1)
//				Calculates the distance between two points
//		int handle( int event )
//				Handles mouse events in the application
//		void setWindowExtents()
//				Sets up the window & object coordinate systems
//		Point toNDC(Point p)
//				Converts the window coordinate system to the
//				Normalized device coordinates from (-1,-1) to
//				(1,1)
//		void changeMode( DrawMode newmode )
//				Handles the menu choices within the application
//		void draw()
//				Handles all drawing for the application
//		void drawviewports(Polygon p, Polygon *win)
//				Is called by the draw() function and does all
//				coordinate translations & drawing of objects
//				within the clip window win into the 
//				corresponding viewport
//		void init_clip()
//				Initializes and resets the clipping windows
//		void drawpolys()
//				Is called by the draw() function and loops 
//				through all saved polygons & draws them
//		void drawlines()
//				Is called by the draw() function and loops 
//				through all saved lines & draws them
//		void drawwinobjs()
//				Is called by the draw() function and draws the
//				two clip windows
//		void drawviewports(Line l, Polygon *win)
//				Checks if any part of the line is in the clipwindow & 
//				if so, draws that piece	of the line
//		void drawviewports(Polygon p, Polygon *win)
//				Checks if any part of the polygon is in the clipwindow & 
//				if so, draws those pieces of the outline of the polygon
//		Point getCenter(Line l)
//				Calculates the midpoint of the Line & returns that point
//		Point getCenter(Polygon p)
//				Calculates the center of the Polygon & returns that point
//		Line lineClip(Point winMin, Point winMax, Point p1, Point p2)
//				Returns the clipped part of the line that resides within the clip window &
//				colors the line ORANGE (for reference only) if any part of the line
//				is within the clip window
//		int cliptest(float p, float q, float * u1, float * u2)
//				Returns the clipped part of the line that resides within the clip window &
//				colors the line ORANGE (for reference only) if any part of the line
//				is within the clip window
//
// ---------------------------------------------------------------------------------------------

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <FL/Fl.h>
#include <FL/Fl_Gl_Window.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <FL/gl.h>
#include <vector>
#include <string>

using namespace std;


class GLWidget : public Fl_Gl_Window
{
	public:

		enum DrawMode { DRAW_POLY_PTS, DRAW_POLY, DRAW_START, DRAW_GOAL, CLEAR_SCREEN, BEGIN, CHOOSE_OBJECT, DELETE_OBJ, REDRAW, SAVE, LOAD };
		enum DrawColor { RED, GREEN, BLUE, YELLOW, WHITE, PURPLE, ORANGE };


		GLWidget( int x, int y, int w, int h, const char *title = NULL );

		void setWindowExtents();
		int handle( int event );
		void draw();
		void changeMode( DrawMode );
		void init();
		void init_grid();
		void setcolor( DrawColor );
		
		struct Point { float x, y; DrawColor color; };
		struct Line { Point pt1, pt2; DrawColor color; DrawColor oldcolor;};
		struct Polygon { vector<Point> pts; DrawColor color; DrawColor oldcolor;};

		vector<Line> grid_lines; // saves drawn lines
		vector<Polygon> poly_vec; // saves drawn polygons
		static vector<Point> Astarptvector;
		static vector<Point> LRTAstarptvector;
		Polygon *selectedPoly;
		Point *selectedPt;

		Point prevpoint;

		void drawlines();
		void drawpolys();
		void drawpoints();
		void drawclicks();
		void drawpaths();
		void savetofile(string name);
		void loadfile();

		Point getCenter(Polygon);
		Point getCenter(Line);
		float distance(Point p1, Point p2);
		void runprogram();
		


		// handle
		static GLWidget *current;

		static GLWidget*  getObjHandle(void)
		{
		return current;
		}

	private:

		vector<Point> tmppts; // hold the points so far
		DrawColor color; // current color
		int num_points;
		float x_max;
		float y_max;

		DrawMode drawmode;
};



#endif // GLWIDGET_H
