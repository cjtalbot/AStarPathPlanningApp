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


#include <math.h>
#include <iostream>

#ifdef WIN32
#include <windows.h>
#endif
#ifdef __MACOSX__
#include <OpenGL/glu.h>
#endif
#ifdef __UNIX__
#include <GL/glu.h>
#endif

#include "glWidget.h"

#include <fstream>
#include <stdlib.h>
#include <float.h>
#include "mainWindow.h"
#include <string>
#include "ai.h"
#include <cstring>
#include <FL/fl_ask.h>
#include <FL/Fl_File_Chooser.h>

GLWidget* GLWidget::current = NULL;
bool start_defined = false;
bool goal_defined = false;

GLWidget::Point start;
GLWidget::Point goal;

// ---------------------------------------------------------------------------------------------
//  Name:  GLWidget
//
//	Description:
//		Constructor that creates a window
//	Input:
//		int - bottom left x coordinate
//		int - bottom left y coordinate
//		int - width of window
//		int - height of window
//		char* - window title
//	Output:
//		new window
//
// ---------------------------------------------------------------------------------------------
GLWidget::GLWidget( int x, int y, int w, int h, const char *l )
: Fl_Gl_Window( x, y, w, h, l )
{

	vector<Polygon> poly_vec; // saves drawn polygons
	vector<Point> tmppts; // holds points as chosen - use size for num_points
	DrawColor color = YELLOW; // initialize to draw in yellow
	vector<Line> grid_lines; // saves grid lines
	
	// handle to object
	current = this;
}

// ---------------------------------------------------------------------------------------------
//  Name:  runprogram
//
//	Description:
//		Runs the programs for the robot
//
// ---------------------------------------------------------------------------------------------
void GLWidget::runprogram()
{

	// all the program stuff for the robots
	AI *x = new AI();
	x->AI::init(start, goal, poly_vec);
	Astarptvector.resize(0);
	Astarptvector.clear();

	x->AI::runAstar(start, goal, poly_vec);
	LRTAstarptvector.resize(0);
	LRTAstarptvector.clear();
	x->AI::runLRTAstar(start, goal, poly_vec);
	// give results
	
	changeMode(REDRAW);
	savetofile("results.txt");
	
	
	// update message bar
	MainWindow::getTheApp()->setLabel ("Program done running.  Results should be in a popup and on the screen.");
	MainWindow::getTheApp()->setfile();
}

// ---------------------------------------------------------------------------------------------
//  Name:  init_clip
//
//	Description:
//		Initializes and resets the clipping windows
//
// ---------------------------------------------------------------------------------------------
void GLWidget::init_grid()
{

	//own function so can call when clear screen too
	Line tmp;
	
	int i = 50;
	// draw grid lines & save
	while(i < x_max)

	{
		tmp.pt1.x = i;
		tmp.pt1.y = 0;
		tmp.pt2.x = i;
		tmp.pt2.y = y_max;
		tmp.color = WHITE;
		
		grid_lines.push_back(tmp);
		i = i+50;
	}
	int j = 50;
	// repeat for y values
	while(j < y_max)
	{
		tmp.pt1.x = 0;
		tmp.pt1.y = j;
		tmp.pt2.x = x_max;
		tmp.pt2.y = j;
		tmp.color = WHITE;
		
		grid_lines.push_back(tmp);
		j = j+50;
	}
	

}

// ---------------------------------------------------------------------------------------------
//  Name:  init
//
//	Description:
//		Initialization method to setup the window & 
//			object coordinates and related clipwindows
//
// ---------------------------------------------------------------------------------------------
void GLWidget::init()
{

	// some GL initializations

	mode( FL_DOUBLE | FL_RGB );

	setWindowExtents();

	// Set the clear color to black

	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	color = YELLOW;
	setcolor(color);
	
	selectedPoly = NULL;

	drawmode = DRAW_POLY_PTS; // puts us in drawmode initially
		
	init_grid(); // draw grid lines
	// clean variables
	Astarptvector.resize(0);
	Astarptvector.clear();
	LRTAstarptvector.resize(0);
	LRTAstarptvector.clear();
	poly_vec.resize(0);
	poly_vec.clear();
	
	changeMode( REDRAW );
	
}

// ---------------------------------------------------------------------------------------------
//  Name:  setcolor
//
//	Description:
//		Changes the drawing color for objects
//	Input:
//		DrawColor - color to change the drawing of objects to
//
// ---------------------------------------------------------------------------------------------
void GLWidget::setcolor ( DrawColor col_val )
{
	// sets the color for drawing
	switch (col_val)
	{
		case YELLOW:
			glColor3f( 1.0, 1.0, 0.0);
			break;
		case BLUE:
			glColor3f( 0.0, 0.0, 1.0);
			break;
		case GREEN:
			glColor3f( 0.0, 1.0, 0.0);
			break;
		case RED:
			glColor3f( 1.0, 0.0, 0.0);
			break;
		case WHITE:
			glColor3f( 1.0, 1.0, 1.0);
			break;
		case PURPLE:
			glColor3f( 0.8, 0.0, 1.0);
			break;
		case ORANGE:
			glColor3f( 1.0, 0.5, 0.0);
			break;
		default:
			glColor3f( 0.1, 0.1, 0.1); // if a prob, use dk gray
			break;
	}
}

// ---------------------------------------------------------------------------------------------
//  Name:  distance
//
//	Description:
//		Calculates the distance between two points
//	Input:
//		float - 1st x coordinate
//		float - 1st y coordinate
//		float - 2nd x coordinate
//		float - 2nd y coordinate
//	Output:
//		float - distance between the 1st and 2nd point
//
// ---------------------------------------------------------------------------------------------
float GLWidget::distance(Point p1, Point p2)
{
	// calculates the real distance between the two points
	return sqrt(((p1.x-p2.x)*(p1.x-p2.x)) + ((p1.y-p2.y)*(p1.y-p2.y)));

}

// ---------------------------------------------------------------------------------------------
//  Name:  handle
//
//	Description:
//		Handles mouse events in the application
//	Input:
//		int - what event was captured
//	Output:
//		int - whether the event was handled or not
//
// ---------------------------------------------------------------------------------------------
int GLWidget::handle( int event )
{
	// The handle() function, reimplemented from
	// Fl_Gl_Window, notifies us of any user input
	// in our OpenGL drawing area. There are various
	// Fl::event_ functions to get event parameters like
	// which button or key pressed, mouse x and y
	// position, etc.

	
	Point eventp;
	Point temp;
	
	eventp.x = Fl::event_x();
	eventp.y = Fl::event_y();
	
	
	switch( event )
	{
		// Mouse button pressed
		case FL_PUSH:
		
			// save prior point to help with the drag functions
			prevpoint = eventp;

			// If it was the right button, clear the screen
			if( Fl::event_button3() )
			{
				if (drawmode == DRAW_POLY_PTS) // to make it easier to draw polys
				{
					changeMode (DRAW_POLY);	
				} 
				else if (drawmode == DELETE_OBJ)
				{
					changeMode(CHOOSE_OBJECT); // to choose another object
				}
				else 
				{
					changeMode( CLEAR_SCREEN ); // default
				}
			}
			else if( drawmode == DRAW_POLY_PTS )
			{
				// Get the point for polygon drawing
				tmppts.push_back(eventp);
				redraw();
			}
			else if (drawmode == DRAW_POLY)
			{
				// save the last point, then switch back to drawing polygons
				tmppts.push_back(eventp);
				changeMode(DRAW_POLY_PTS);
			}
			else if (drawmode == CHOOSE_OBJECT)
			{
				// Find the object with a point closest to the pointer
				
				Polygon * pfound = NULL;
				Point * ptfound = NULL;
				float mindist = FLT_MAX;
							
				
				// Loop through all polygons & find closest 
				
				for (vector<Polygon>::iterator i = poly_vec.begin(); i != poly_vec.end(); i++)
				{
					if ( i != poly_vec.end())
					{
						Polygon p = *i;
						if (p.color == ORANGE)
						{
							// reset the color - need to save original color?
							i->color = p.oldcolor;
						}
						// loop through each point for each polygons

						for (vector<Point>::iterator j = p.pts.begin(); j != p.pts.end(); j++)
						{
							if (j != p.pts.end())
							{
								Point pt = *j;

								if (distance(pt, eventp) < mindist)
								{
									pfound = &(*i);
									mindist = (distance(pt, eventp));
								}
							}
						}
					}
				}
				start.color = RED;
				goal.color = GREEN;
				// check the start point
				if (start_defined and distance(start, eventp) < mindist)
				{
					pfound = NULL;
					ptfound = &start;
					mindist = (distance(start, eventp));
					
				}
				// check the goal point
				if (goal_defined and distance(goal, eventp) < mindist)
				{
					pfound = NULL;
					ptfound = &goal;
					mindist = (distance(start, eventp));
					
				}

				if (pfound != NULL) // if closest object was a poly
				{
					selectedPoly = pfound;
					selectedPt = NULL;
					pfound->color = ORANGE;
				}
				else if (ptfound !=NULL)
				{
					selectedPt = ptfound;
					selectedPoly = NULL;
					ptfound->color = ORANGE;
				}
				else
				{
					// do nothing
					MainWindow::getTheApp()->setLabel ("ERROR - No closest object found.");
				}
				redraw();
			}
			else if (drawmode == DRAW_START)
			{
				start_defined = true;
				start.x = eventp.x;
				start.y = eventp.y;
				start.color = RED;
				redraw();
			}
			else if (drawmode == DRAW_GOAL)
			{
				goal_defined = true;
				goal.x = eventp.x;
				goal.y = eventp.y;
				goal.color = GREEN;
				redraw();
			}

			// Must return 1 to let FLTK know we handled the
			// event. Otherwise, it may get sent to another
			// widget.
			return 1;
		
	}

	// Send any event we didn't use to the default handler
	return Fl_Gl_Window::handle( event );
}

// ---------------------------------------------------------------------------------------------
//  Name:  setWindowExtents
//
//	Description:
//		Sets up the window & object coordinate systems
//
// ---------------------------------------------------------------------------------------------
void GLWidget::setWindowExtents()
{

	// Fl::event_x() and Fl::event_y() return mouse position
	// in pixels from top-left of drawing area. So we'll set
	// GL's window extents to match:
	// x = 0 (left) to 700 (right), y = 500 (bottom) to 0 (top)
 gluOrtho2D( 0, 700, 500, 0 );

 	x_max = 700;
 	y_max = 500;

}


// ---------------------------------------------------------------------------------------------
//  Name:  getCenter
//
//	Description:
//		Calculates the center of the Polygon & returns that point
//	Input:
//		Polygon - the polygon to calculate the center of
//	Output:
//		Point - point which represents the center of the polygon
//
// ---------------------------------------------------------------------------------------------
GLWidget::Point GLWidget::getCenter(Polygon p)
{
	// initialize sums & counters
	float sumx = 0;
	float sumy = 0;
	int count = 0;
	
	// for each point of the polygon
	for (vector<Point>::iterator i = p.pts.begin(); i != p.pts.end(); i++)
	{
		if (i != p.pts.end())
		{
			Point pt = *i;
			sumx = sumx + pt.x; // sum up the x coordinates
			sumy = sumy + pt.y; // sum up the y coordinates
			count = count + 1; // keep track of how many points in this polygon
		}
	}
	Point res;
	res.x = sumx / count; // average the x coordinates
	res.y = sumy / count; // average the y coordinates
	
	return res;
}

// ---------------------------------------------------------------------------------------------
//  Name:  getCenter
//
//	Description:
//		Calculates the midpoint of the Line & returns that point
//	Input:
//		Line - the line to calculate the midpoint of
//	Output:
//		Point - point which represents the midpoint of the line
//
// ---------------------------------------------------------------------------------------------
GLWidget::Point GLWidget::getCenter(Line l)
{
	Point res;
	res.x = (l.pt1.x + l.pt2.x) / 2; // average the x coordinates
	res.y = (l.pt1.y + l.pt2.y) / 2; // average the y coordinates

	return res;
}

void GLWidget::savetofile(string sname)
{
	// allows the file to be saved so it can be reloaded later for viewing or re-running
	const char* name = fl_input( "Please enter the name of the file to save to:", sname.c_str() );
		
	char newlabel[128] = "";

	sprintf(newlabel, "File %s saved.", name);

	MainWindow::getTheApp()->setLabel( newlabel );
	
	std::ofstream out(name);
	// save the regular variables for counts for vectors
	out.write(reinterpret_cast<char*>(&start_defined), sizeof(start_defined));
	out.write(reinterpret_cast<char*>(&goal_defined), sizeof(goal_defined));
	out.write(reinterpret_cast<char*>(&start), sizeof(start));
	out.write(reinterpret_cast<char*>(&goal), sizeof(goal));
	
	int psize = poly_vec.size();
	out.write(reinterpret_cast<char*>(&psize), sizeof(psize));
	// save each of the polygons with a starter
	char str[] = "STARTPOLYS\n";
	out.write(str, strlen(str));

	for (vector<Polygon>::iterator j = poly_vec.begin(); j != poly_vec.end(); j++)
	{
		Polygon m = *j;
		// save the data about the polygons
		out.write(reinterpret_cast<char*>(&m.color), sizeof(m.color));
		out.write(reinterpret_cast<char*>(&m.oldcolor), sizeof(m.oldcolor));
		// save how many points in the polygon
		int ptsize = m.pts.size();
		out.write(reinterpret_cast<char*>(&ptsize), sizeof(ptsize));
		char str2[] = "STARTPTS\n";
		out.write(str2, strlen(str2));
		// save the points for the polygon
		for (vector<Point>::iterator i = m.pts.begin(); i != m.pts.end(); i++)
		{
			if (i != m.pts.end())
			{
				Point pt = *i;
				out.write(reinterpret_cast<char*>(&pt), sizeof(pt));
			}
		}
		char str3[] = "ENDPTS\n";
		out.write(str3, strlen(str3));
	}
	char str4[] = "ENDPOLYS\n";
	out.write(str4, strlen(str4));
	// save the results line if have it
	int Asize = Astarptvector.size();
	out.write(reinterpret_cast<char*>(&Asize), sizeof(Asize));
	char str5[] = "STARTASTAR\n";
	out.write(str5, strlen(str5));
	// save all the points in the results line
	for (vector<Point>::iterator k = Astarptvector.begin(); k != Astarptvector.end(); k++)
	{
		if(k != Astarptvector.end())
		{
			Point a = *k;
			out.write(reinterpret_cast<char*>(&a), sizeof(a));
		}
	}
	char str6[] = "ENDASTAR\n";
	out.write(str6, strlen(str6));
	// save all the points in the LRTA results line - not used
	int LRTAsize = LRTAstarptvector.size();
	out.write(reinterpret_cast<char*>(&LRTAsize), sizeof(LRTAsize));
	char str7[] = "STARTLRTASTAR\n";
	out.write(str7, strlen(str7));
	for (vector<Point>::iterator n = LRTAstarptvector.begin(); n != LRTAstarptvector.end(); n++)
	{
		if(n != LRTAstarptvector.end())
		{
			Point b = *n;
			out.write(reinterpret_cast<char*>(&b), sizeof(b));
		}
	}
	char str8[] = "ENDLRTASTAR\n";
	out.write(str8, strlen(str8));
	out.close();

}

void GLWidget::loadfile()
{
	// load a saved file so can be re-run
	start_defined = false;
	goal_defined = false;
	poly_vec.resize(0);
	poly_vec.clear();
	MainWindow::getTheApp()->setLRTALabel (0.0, 0.0);
	MainWindow::getTheApp()->setALabel (0.0, 0.0);
	
	// allow user to pick using a file browser
	const char* name;
	char temp[128] = "";
	if(true)
	{	Fl_File_Chooser filepick(".", "*.txt", Fl_File_Chooser::SINGLE, "Select the file to be loaded");
		filepick.show();
		while(filepick.shown())
			Fl::wait();
		if (filepick.value() == NULL)
			sprintf(temp, "save.txt");
		else
			{ strcat(temp, (char*)filepick.value());}
	
		filepick.hide();
		// destroy filepick 
	}
	name = temp;

	char newlabel[128] = "";

	sprintf(newlabel, "File %s loaded.", name);

	// read the static vars first
	MainWindow::getTheApp()->setLabel( newlabel );
	std::ifstream in(name);
	in.read(reinterpret_cast<char*>(&start_defined), sizeof(start_defined));
	in.read(reinterpret_cast<char*>(&goal_defined), sizeof(goal_defined));
	in.read(reinterpret_cast<char*>(&start), sizeof(start));
	in.read(reinterpret_cast<char*>(&goal), sizeof(goal));
	// read how many polys we have
	int numpolys;
	in.read(reinterpret_cast<char*>(&numpolys), sizeof(numpolys));
	char str[] = "STARTPOLYS\n";
	in.read(str, strlen(str));
	Polygon p;
	// read the polys
	for(int i = 0; i < numpolys; i++)
	{
		int numpts;
	
		in.read(reinterpret_cast<char*>(&p.color), sizeof(p.color));
		in.read(reinterpret_cast<char*>(&p.oldcolor), sizeof(p.oldcolor));
		in.read(reinterpret_cast<char*>(&numpts), sizeof(numpts));
		char str2[] = "STARTPTS\n";
		in.read(str2, strlen(str2));
		// read the points in the polys
		for(int j = 0; j < numpts; j++)
		{

			Point temp;
			in.read(reinterpret_cast<char*>(&temp), sizeof(temp));
			p.pts.push_back(temp);

		}
		// when hit ENDPTS:
		poly_vec.push_back(p);
		p.pts.resize(0);
		p.pts.clear();
		
		char str3[] = "ENDPTS\n";
		in.read(str3, strlen(str3));
	}
	char str4[] = "ENDPOLYS\n";
	in.read(str4, strlen(str4));
	// if more data, then continue to read
	if (!in.eof())
	{
		int numA;
		in.read(reinterpret_cast<char*>(&numA), sizeof(numA));
		char str5[] = "STARTASTAR\n";
		in.read(str5, strlen(str5));
		// read all the points for the A* solution
		for (int j=0; j < numA; j++)
		{
			Point temp;
			in.read(reinterpret_cast<char*>(&temp), sizeof(temp));
			Astarptvector.push_back(temp);
		}
		char str6[] = "ENDASTAR\n";
		in.read(str6, strlen(str6));
		// not used - for LRTA* results
		if (!in.eof())
		{
			int numLRTA;
			in.read(reinterpret_cast<char*>(&numLRTA), sizeof(numLRTA));
			char str7[] = "STARTLRTASTAR\n";
			in.read(str7, strlen(str7));
			for (int j=0; j < numA; j++)
			{
				Point temp;
				in.read(reinterpret_cast<char*>(&temp), sizeof(temp));
				LRTAstarptvector.push_back(temp);
			}
			char str8[] = "ENDLRTASTAR\n";
			in.read(str8, strlen(str8));
		}
	}
	in.close();
	
}

// ---------------------------------------------------------------------------------------------
//  Name:  changeMode
//
//	Description:
//		Handles the menu choices within the application
//	Input:
//		DrawMode - the item chosen from the menu
//
// ---------------------------------------------------------------------------------------------
void GLWidget::changeMode( DrawMode newmode )
{
	DrawMode olddm = drawmode;
	drawmode = (DrawMode)newmode;

	switch (drawmode)
	{
		case CLEAR_SCREEN:
			// If Clear Screen was selected we want to
			// go ahead and redraw.
			
			// delete all objects?
			poly_vec.resize(0);
			poly_vec.clear();
			tmppts.resize(0);
			tmppts.clear();
			if (selectedPoly != NULL) selectedPoly->color = selectedPoly->oldcolor;
			selectedPoly = NULL;
			if (selectedPt != NULL) 
			{
				if(selectedPt == &start) selectedPt->color = RED;
				if(selectedPt == &goal) selectedPt->color = GREEN;
				
			}
			selectedPt = NULL;
			start_defined = false;
			goal_defined = false;
			Astarptvector.resize(0);
			Astarptvector.clear();
			LRTAstarptvector.resize(0);
			LRTAstarptvector.clear();
			// redraw the screen
			redraw();
			break;
		case DRAW_POLY:
			// clear any selected objects
			if (selectedPoly != NULL) selectedPoly->color = selectedPoly->oldcolor;
			selectedPoly = NULL;
			if (selectedPt != NULL) 
			{
				if(selectedPt == &start) selectedPt->color = RED;
				if(selectedPt == &goal) selectedPt->color = GREEN;
				
			}
			selectedPt = NULL;
			// save points as a polygon, then redraw screen
			if(tmppts.size() > 2) {
				Polygon p;
				p.pts.resize(0);
				p.pts.clear();
				for(vector<Point>::iterator i = tmppts.begin(); i != tmppts.end(); i++)
				{
					if (i != tmppts.end())
					{
						Point t = *i;
						p.pts.push_back(t);
					}
				}
				p.color = color;
				p.oldcolor = color;
				poly_vec.push_back(p);

				redraw();
			} else {
				// give error message - nothing to draw yet & return to prior mode
				drawmode = olddm;
				MainWindow::getTheApp()->setLabel ("ERROR - Not enough points to draw a polygon yet.");
				redraw();
			}
			break;
		case DELETE_OBJ:
			// delete the object that is orange
			if (selectedPoly != NULL)
			{
				// remove Poly
				int ctr = 0;
				for (vector<Polygon>::iterator i = poly_vec.begin(); i != poly_vec.end(); )
				{
					if (i != poly_vec.end())
					{
						if (&(*i) == selectedPoly)
						{
							poly_vec.erase(poly_vec.begin() + ctr);
							break;
						} else i++;
						ctr = ctr+1;
					}
				}
				// clear out the deleted object from selected objects
				selectedPoly = NULL;
			} 
			else if (selectedPt != NULL)
			{
				if(selectedPt == &start)
				{
					//start = NULL;
					start_defined = false;
				}
				else if (selectedPt == &goal)
				{
					//goal = NULL;
					goal_defined = false;
				}
				selectedPt = NULL;
			}
			else
			{
				// nothing is selected
				MainWindow::getTheApp()->setLabel ("ERROR - No object is selected.");
			}
			
			redraw();
			drawmode = olddm;
			break;
		case DRAW_START:
			if(start_defined)
			{
				MainWindow::getTheApp()->setLabel("Start already defined, so will remove it & draw a new one.");
			}
			//redraw();
			break;
		case DRAW_GOAL:
			if(goal_defined)
			{
				MainWindow::getTheApp()->setLabel("Goal already defined, so will remove it & draw a new one.");
			}
			//redraw();
			break;
		case BEGIN:
			// make sure we have a start & goal defined
			if(!start_defined or !goal_defined)
			{
				MainWindow::getTheApp()->setLabel("Start and Goal must be defined first.");
			}
			else
			{
				runprogram(); // run A* if have a goal & start defined
			}
			//redraw();
			break;
		case SAVE:
			savetofile("save.txt"); // save file by default to save.txt
			break;
		case LOAD: // reads & loads a file to the screen
			poly_vec.resize(0);
			poly_vec.clear();
			tmppts.resize(0);
			tmppts.clear();
			if (selectedPoly != NULL) selectedPoly->color = selectedPoly->oldcolor;
			selectedPoly = NULL;
			if (selectedPt != NULL) 
			{
				if(selectedPt == &start) selectedPt->color = RED;
				if(selectedPt == &goal) selectedPt->color = GREEN;
				
			}
			selectedPt = NULL;
			start_defined = false;
			goal_defined = false;
			Astarptvector.resize(0);
			Astarptvector.clear();
			LRTAstarptvector.resize(0);
			LRTAstarptvector.clear();
			loadfile();
		case REDRAW:
		case DRAW_POLY_PTS:
		case CHOOSE_OBJECT:
		default:
			redraw();
			break;
	}
}



// ---------------------------------------------------------------------------------------------
//  Name:  drawpolys
//
//	Description:
//		Is called by the draw() function and loops through all saved polygons & draws them
//
// ---------------------------------------------------------------------------------------------
void GLWidget::drawpolys()
{

	//draw all the polygons
	for (vector<Polygon>::iterator i = poly_vec.begin(); i != poly_vec.end(); i++)
	{
		if (i != poly_vec.end())
		{
			Polygon p = *i;
			glBegin(GL_POLYGON);
			setcolor(p.color);
			// draw all the points in the polygon
			for (vector<Point>::iterator j = p.pts.begin(); j != p.pts.end(); j++)
			{
				Point pt = *j;
				glVertex2f(pt.x, pt.y);
			}
			glEnd();

		}
	}
}



// ---------------------------------------------------------------------------------------------
//  Name:  draw
//
//	Description:
//		Handles all drawing for the application
//
// ---------------------------------------------------------------------------------------------
void GLWidget::draw()
{
	// All drawing code goes in the draw() function, which is
	// reimplemented from Fl_Gl_Window.
	//
	// NOTE: draw() should only be called by the system. To
	// explicitly redraw the screen in your code, call
	// redraw(). This doesn't swap buffers.

	if( !valid() )
	{
		// If our window has been invalidated (for example
		// minimized then restored), we'll have to reset
		// the window parameters.

		init();
	}

	switch( drawmode )
	{
		case DRAW_START:
			// draw the start point if defined
			if(start_defined)
			{
				glBegin(GL_POINTS);
				setcolor(GREEN);
				glVertex2f(start.x, start.y);
				tmppts.pop_back();
				glEnd();
			}
			
			// redraw screen to make sure the selected objects are cleared
			tmppts.resize(0);
			tmppts.clear();
			glClear( GL_COLOR_BUFFER_BIT );
				glFlush();
			drawpolys();
			drawlines();
			drawpoints();
			drawpaths();
			changeMode(REDRAW);
			break;

		case DRAW_GOAL: // draw goal point if defined
			if(goal_defined)
			{
				glBegin(GL_POINTS);
				setcolor(RED);
				glVertex2f(goal.x, goal.y);
				tmppts.pop_back();
				glEnd();
			}
			
			// redraw screen to make sure the selected objects are cleared
			tmppts.resize(0);
			tmppts.clear();
			glClear( GL_COLOR_BUFFER_BIT );
				glFlush();
			drawpolys();
			drawlines();
			drawpoints();
			drawpaths();
			changeMode(REDRAW);
			break;
			
		case DRAW_POLY:
			if (tmppts.size() < 3 ) break; // not a polygon if only has 1 or 2 points
			
			glBegin(GL_POLYGON);
			setcolor(color);

			// draw the polygon points & check for clipwindows
			for (vector<Point>::iterator j = tmppts.begin(); j != tmppts.end(); j++)
			{
				if (j != tmppts.end())
				{
					Point pt = *j;
					glVertex2f(pt.x, pt.y);
				}
			}
			glEnd();

			// redraw objects to clear out any selected items
			tmppts.resize(0);
			tmppts.clear();
			glClear( GL_COLOR_BUFFER_BIT );
				glFlush();
			drawpolys();
			drawlines();
			drawpoints();
			drawpaths();
			break;

		case CLEAR_SCREEN:

			// This clears the drawing area to
			// our preset glClearColor.
			glClear( GL_COLOR_BUFFER_BIT );
			// allow the redraw to occur to keep the windows
		case REDRAW:
		case LOAD:
		case SAVE:
		case CHOOSE_OBJECT:
		case DELETE_OBJ: // all of these just need the redraw for the changes to take effect
			// redraws saved objects in the display list
			{
				// first clear screen
				glClear( GL_COLOR_BUFFER_BIT );
				glFlush();
	
				//draw all the lines
				drawlines();
				
				//draw all the polygons
				drawpolys();
				
				drawpoints();
				drawpaths(); // do I want to clear this once I delete or add objects?
			}
			break;
		case DRAW_POLY_PTS:
		
			drawclicks();
			drawlines();
			drawpolys();
			drawpoints();
			drawpaths();
			break;
	}
}

// ---------------------------------------------------------------------------------------------
//  Name:  drawlines
//
//	Description:
//		Is called by the draw() function and draws the two clip windows
//
// ---------------------------------------------------------------------------------------------
void GLWidget::drawlines()
{
	// set to dashed line for the grids
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x0101);
	glBegin(GL_LINES);
	
	// draw the saved gridlines
	for (vector<Line>::iterator m = grid_lines.begin(); m != grid_lines.end(); m++)
	{
		if (m != grid_lines.end())
		{
			Line l = *m;
			setcolor(l.color);
			glVertex2f( l.pt1.x, l.pt1.y);
			glVertex2f(l.pt2.x, l.pt2.y);
		}
	}
	
	glEnd();
	
	glDisable(GL_LINE_STIPPLE);
	// be sure to turn off the dashed line drawing!
}

void GLWidget::drawpaths()
{
	
	glBegin(GL_LINES);

	setcolor(BLUE);
	for (vector<Point>::iterator m = Astarptvector.begin(); m != Astarptvector.end(); m++)
	{

		if (m != Astarptvector.end())
		{
			Point l = *m;
			
			glVertex2f( l.x, l.y);
	
			if(m != Astarptvector.begin() and m+1 != Astarptvector.end())
			{
	
				glVertex2f(l.x, l.y); // do twice so it starts the next line too if not the last point
			}
			//glVertex2f(l.pt2.x, l.pt2.y);
		}
	}
	glEnd();
	glBegin(GL_LINES);
	setcolor(PURPLE);
	for (vector<Point>::iterator m = LRTAstarptvector.begin(); m != LRTAstarptvector.end(); m++)
	{
	
		if (m != LRTAstarptvector.end())
		{
			Point l = *m;
			setcolor(BLUE);
			glVertex2f( l.x, l.y);
			if(m != LRTAstarptvector.begin() and m+1 != LRTAstarptvector.end())
				glVertex2f(l.x, l.y); // do twice so it starts the next line too if not the last point
			
		}
	}
	
	glEnd();
	
	
}

// ---------------------------------------------------------------------------------------------
//  Name:  drawpoints
//
//	Description:
//		Is called by the draw() function and draws the two clip windows
//
// ---------------------------------------------------------------------------------------------
void GLWidget::drawpoints()
{
	// draw our two points - start & goal
	glPointSize(10.0);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	// set to dashed line for the grids
	glBegin(GL_POINTS);
	
	setcolor(start.color);
	if(start_defined) glVertex2f( start.x, start.y);
	setcolor(goal.color);
	if(goal_defined) glVertex2f(goal.x, goal.y);
	
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	
}

void GLWidget::drawclicks()
{
	// draw any points that have been chosen so far for drawing a polygon
	glPointSize(5.0);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBegin(GL_POINTS);
	setcolor(color);
	
	for (vector<Point>::iterator m = tmppts.begin(); m != tmppts.end(); m++)
	{
		if (m != tmppts.end())
		{
			Point p = *m;
			glVertex2f( p.x, p.y);
		}
	}
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	
}
