// Christine Talbot
//
// Name:
//		main / robot
//
// Description:
//		This includes the main function which drives the application
//
// Method Summary:
// 		int main (int argc, char **argv)
//			Creates the window, then handles events
//
// ---------------------------------------------------------------------------------------------

#include <FL/Fl.h>

#include "mainWindow.h"
#include "glWidget.h"
#include <FL/fl_ask.h>
#include <sstream>
#include <iostream>

// ---------------------------------------------------------------------------------------------
//  Name:  main
//
//	Description:
//		main function which drives the application
//	Inputs:
//		int - not used?
//		char** - not used?
//
// ---------------------------------------------------------------------------------------------

int main( int argc, char **argv )
{
	MainWindow mainwin;

	// Turn on double buffering
	Fl::visual( FL_DOUBLE | FL_INDEX );

	// Show the window
	mainwin.show( argc, argv );

	// initialize the GL widget, get it ready for GL calls
	mainwin.getGlWidget()->init();
	
	// Enter the main loop
	return Fl::run();
}

