// Christine Talbot
//
// Class Name(s):
//		MainWindow
//
// Description:
//		Implementation of MainWindow class which provides the window,
//			menu, and status bars
//
// Method Summary:
// 		void quit_cb( Fl_Widget*, void* )
//			Exits the application
//		void changeMode_cb( Fl_Widget*, int newmode )
//			Changes the mode of the application via the menus
//		MainWindow()
//			Constructor that setups the window, menu, and status bars
//		~MainWindow()
//			Destructor for the class
//		void show( int argc, char **argv )
//			Passes show command on to the window
//		void changeMode( int newmode )
//			Invokes the change mode method & updates the status bar
//		void setLabel(string newlabel)
//			Allows us to set the status bar for errors from the glWidget class
//
// ---------------------------------------------------------------------------------------------


#include <iostream>
using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <FL/Fl_Menu_Bar.h>
#include <FL/Fl_Group.h>
#include <FL/fl_ask.h>
#include <FL/Fl_Counter.h>
#include <FL/Fl_Multiline_Output.h>

#include "mainWindow.h"
#include <string>


MainWindow* MainWindow::theApp = NULL;

// ---------------------------------------------------------------------------------------------
//  Name:  quit_db
//
//	Description:
//		Exits the application
//	Input:
//		Fl_Widget* - window
//		void* - ??
//
// ---------------------------------------------------------------------------------------------
void quit_cb( Fl_Widget*, void* )
{
	exit( 0 );
}

// ---------------------------------------------------------------------------------------------
//  Name:  changeMode_cb
//
//	Description:
//		Changes the mode of the application via the menus
//	Input:
//		Fl_Widget* - application window
//		int - the new mode for the application
//
// ---------------------------------------------------------------------------------------------
void changeMode_cb( Fl_Widget*, int newmode )
{
	// Since changeMode_cb is a stand-alone function,
	// we have to use MainWindow::getTheApp() to get
	// a pointer to the running instance of MainWindow

	(MainWindow::getTheApp())->changeMode( newmode );
}

// ---------------------------------------------------------------------------------------------
//  Name:  MainWindow
//
//	Description:
//		Constructor that setups the window, menu, and status bars
//
// ---------------------------------------------------------------------------------------------
MainWindow::MainWindow()
{
	// Most FL_ constructors are ( x position, y position, width, height, ...)

	// Every Fl object created between here and win->end() is
	// placed in the window
	win = new Fl_Window( 200, 200, 920, 590, "Robot Planning" );

	// Create the OpenGL drawing area
	glwidget = new GLWidget( 10, 40, 700, 500 );
	glwidget->invalidate();

	// Create the menu bar
	menu = new Fl_Menu_Bar( 10, 0, 900, 30 );
	menu->box( FL_PLASTIC_UP_BOX );

	// add( "menu/item", shortcut, callback, data for the callback )
	
	menu->add( "&File/S&ave File", FL_CTRL + 'a',
		(Fl_Callback *)changeMode_cb, (void*)GLWidget::SAVE);
	menu->add( "&File/Load &File", FL_CTRL + 'f',
		(Fl_Callback *)changeMode_cb, (void*)GLWidget::LOAD);
	menu->add( "&File/C&lear Screen", FL_CTRL + 'l',
		(Fl_Callback *)changeMode_cb, (void*)GLWidget::CLEAR_SCREEN );
	menu->add( "&File/&Redraw", FL_CTRL + 'r',
		(Fl_Callback *)changeMode_cb, (void*)GLWidget::REDRAW);
	menu->add( "&File/&Quit", FL_CTRL + 'q',
		(Fl_Callback *)quit_cb );
		
	

	menu->add( "&Draw Polygons/Choose &Polygon Points", FL_CTRL + 'p',
		(Fl_Callback *)changeMode_cb, (void*)GLWidget::DRAW_POLY_PTS );
	menu->add( "&Draw Polygons/&Draw Polygon", FL_CTRL + 'd',
		(Fl_Callback *)changeMode_cb, (void*)GLWidget::DRAW_POLY );
	
	menu->add( "Choose &Start Point", FL_CTRL + 's', 
		(Fl_Callback *)changeMode_cb, (void*)GLWidget::DRAW_START);
	menu->add( "Choose &Goal Point", FL_CTRL + 'g',
		(Fl_Callback *)changeMode_cb, (void*)GLWidget::DRAW_GOAL);
		
	menu->add( "&Manipulate Objects/&Choose Object", FL_CTRL + 'c',
		(Fl_Callback *)changeMode_cb, (void*)GLWidget::CHOOSE_OBJECT );
	menu->add( "&Manipulate Objects/D&elete Object", FL_CTRL + 'e',
		(Fl_Callback *)changeMode_cb, (void*)GLWidget::DELETE_OBJ );
		
	menu->add( "&BEGIN", FL_CTRL + 'b',
		(Fl_Callback *)changeMode_cb, (void*)GLWidget::BEGIN);

	


	// Create the label under the drawing area
	label = new Fl_Output( 10, 550, 900, 30 );
	label->box( FL_PLASTIC_UP_BOX );
	label->color( FL_BACKGROUND_COLOR );
	label->cursor_color( FL_BACKGROUND_COLOR );
	label->value( "Please select an option from one of the menus." );
	
	
	
	// Create the output box to the right of the drawing area
	aoutput = new Fl_Multiline_Output( 720, 40, 190,  500 );
	//aoutput = new Fl_Multiline_Output( 720, 40, 190, 250 );
	aoutput->box(FL_PLASTIC_UP_BOX);
	aoutput->color(FL_BACKGROUND_COLOR);
	aoutput->cursor_color(FL_BACKGROUND_COLOR);
	aoutput->value("A* Path Planning\n\tStatistics:");
	
	//lrtaoutput = new Fl_Multiline_Output( 720, 290, 190, 250 );
	lrtaoutput = new Fl_Multiline_Output( 0, 0, 0, 0 );
	lrtaoutput->box(FL_PLASTIC_UP_BOX);
	lrtaoutput->color(FL_BACKGROUND_COLOR);
	lrtaoutput->cursor_color(FL_BACKGROUND_COLOR);
	lrtaoutput->value("LRTA* Path Planning\n\tStatistics:");
	
	
	// We're done adding items to the window
	win->end();
	win->size_range(100,100);


	// Point our static MainWindow pointer to
	// this running instance
	theApp = this;
	labelset = false; // reset the labelset
	fileloaded = false;
}

// ---------------------------------------------------------------------------------------------
//  Name:  ~MainWindow
//
//	Description:
//		Destructor for the class
//
// ---------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
	delete glwidget, label, win;
}

// ---------------------------------------------------------------------------------------------
//  Name:  show
//
//	Description:
//		Passes show command on to the window
//	Input:
//		int - number of arguments
//		int - arguments to be passed
//
// ---------------------------------------------------------------------------------------------
void MainWindow::show( int argc, char **argv )
{
	// Pass the show() command on to our window
	win->show( argc, argv );
}

// ---------------------------------------------------------------------------------------------
//  Name:  changeMode
//
//	Description:
//		Invokes the change mode method & updates the status bar
//	Input:
//		int - the new mode being set
//
// ---------------------------------------------------------------------------------------------
void MainWindow::changeMode( int newmode )
{
	glwidget->changeMode( (GLWidget::DrawMode)newmode );

	char newlabel[128];

	// populate the status bar with text based on the mode we're in
	switch( newmode )
	{
		case GLWidget::DRAW_POLY_PTS:
			sprintf( newlabel, "Pick any points on the canvas, then choose Draw->Draw Polygon to draw the polygon." );
			break;

		case GLWidget::DRAW_POLY:
			sprintf( newlabel, "Polygon drawn.  Please select another option from the menus.");
			break;
		
		case GLWidget::DRAW_START:
			sprintf( newlabel, "Start point selected.  Please select another option from the menus.");
			break;
			
		case GLWidget::DRAW_GOAL:
			sprintf( newlabel, "Goal point selected.  Please select another option from the menus.");
			break;	
		
		case GLWidget::CHOOSE_OBJECT:
			sprintf( newlabel, "Choose an object on the canvas, then choose a menu item from the Manipulate Objects menu.");
			break;
		
		case GLWidget::DELETE_OBJ:
			sprintf( newlabel, "Object has been deleted.  Please choose another option from the menus.");
			break;
			
		case GLWidget::BEGIN:
			sprintf( newlabel, "Robot planning is running...");
			break;
		case GLWidget::SAVE:
			sprintf( newlabel, "File saved as save.txt");
			break;
		case GLWidget::LOAD:
			sprintf( newlabel, "File save.txt loaded");
			break;
		case GLWidget::CLEAR_SCREEN:
		default:
			sprintf( newlabel, "Please select an option from one of the menus." );
			break;
	}

	if (!labelset) label->value( newlabel ); // only set the above default if we didn't have an error
	labelset = false; // reset the error message boolean so the above msgs will be written next time
}

// ---------------------------------------------------------------------------------------------
//  Name:  setLabel
//
//	Description:
//		Allows us to set the status bar for errors from the glWidget class
//	Input:
//		string - text to set in the status bar
//
// ---------------------------------------------------------------------------------------------
void MainWindow::setLabel(string newlabel)
{
	labelset = true; // if set the status outside the changemode, set the boolean so it won't be overwritten
	label->value( newlabel.c_str());
}


void MainWindow::setALabel(float time, float distance)
{
	// prints the results to the screen for the A* algorithm
	char newlabel[128];
	if (time == 0.0 and distance == 0.0)
	{
		aoutput->value("A* Path Planning\n\tStatistics:");
	}
	else
	{
		sprintf(newlabel, "A* Path Planning\n\tStatistics:\n\nTime Taken:  %0.2f ms\nTotal Distance:  %0.2f", time, distance);
		aoutput->value(newlabel);
	}
	
	
}

void MainWindow::setLRTALabel(float time, float distance)
{
	// prints the results to the screen for the LRTA* algorithm (not used)
	char newlabel[128];
	if (time == 0.0 and distance == 0.0)
	{
		lrtaoutput->value("LRTA* Path Planning\n\tStatistics:");
	}
	else
	{
		sprintf(newlabel, "LRTA* Path Planning\n\tStatistics:\n\nTime Taken:  %0.2f ms\nTotal Distance:  %0.2f", time, distance);
		lrtaoutput->value(newlabel);
	}
	
	
}

void MainWindow::setfile()
{
	// disables the ability to re-run or load a file after you've run one (avoids the bug where it crashes on the second run)
	fileloaded = true;
	Fl_Menu_Item *m = (Fl_Menu_Item*)menu->find_item("&File/Load &File");
	if ( ! m ) 
		cout << "ERROR\n";
	else 
		m->deactivate();

	Fl_Menu_Item *n = (Fl_Menu_Item*)menu->find_item("&BEGIN");
	if ( ! n ) 
		cout << "ERROR\n";
	else 
		n->deactivate();
}

