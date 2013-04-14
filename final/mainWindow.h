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

//--------------------------------------------------
// MainWindow.h -- declarations for the main window
//--------------------------------------------------

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>

#include <FL/Fl.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Output.h>
#include <FL/Fl_Multiline_Output.h>

#include "glWidget.h"
#include <string>
#include <FL/Fl_Counter.h>
#include <FL/Fl_Menu_Bar.h>


class MainWindow
{
	private:

		Fl_Window *win;
		Fl_Output *label;
		GLWidget *glwidget;
		Fl_Multiline_Output *aoutput;
		Fl_Multiline_Output *lrtaoutput;

		static MainWindow* theApp;
		bool labelset;
		Fl_Menu_Bar* menu;
		bool fileloaded;

	public:

		MainWindow();
		~MainWindow();
		void show( int argc, char **argv );

		GLWidget *getGlWidget(void) { return glwidget; }
		void changeMode( int );

		static MainWindow* getTheApp() { return theApp; }
		void setLabel(string);
		void setALabel(float time, float distance);
		void setLRTALabel(float time, float distance);
		void setfile();

};

#endif // MAINWINDOW_H
