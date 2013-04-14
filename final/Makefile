# -------------------------------------------------------------------
# Christine Talbot
#
# Compiles the robot, mainWindow, glWidget, and other files
# and links, as appropriate.  Generates the executable jdraw
#--------------------------------------------------------------------

CPP = g++

TARGET = robot

SOURCES  = robot.cpp Node.cpp Edge.cpp Graph.cpp State.cpp PQ.cpp ai.cpp mainWindow.cpp glWidget.cpp 
INCLUDES = mainWindow.h glWidget.h Node.cpp Edge.h Graph.h State.h PQ.h ai.h 
OBJS     = $(SOURCES:.cpp=.o)

#--------------------------------
# MacOS (Snow Leopard) Section
# For fltk-aqua
#
#CPPFLAGS = -g -m32 $(INCDIR) -D__MACOSX__
#INCL_FLAGS = -I/sw/include -I/sw/lib/fltk-aqua/include
#LIB_FLAGS = -L/sw/lib/fltk-aqua/lib -framework AGL -framework OpenGL
#LIBS = -lfltk -lfltk_gl 

#--------------------------------
# Linux Section
#
CPPFLAGS = -g $(INCDIR) -D__UNIX__
INCL_FLAGS = -I/usr/local/include
LIB_FLAGS = -L/usr/local/lib
LIBS = -lfltk -lfltk_gl -lGLU -lGL

all: $(TARGET)

.SUFFIXES: .cpp .cc .cxx .o

.cpp.o:
	$(CPP) $(CPPFLAGS) $(INCL_FLAGS) -c  $<

$(TARGET): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $(TARGET) $(OBJS) $(LIB_FLAGS) $(LIBS)

clean:
	rm -rf $(TARGET) $(OBJS) core

backup:
	tar cvfz robot.tgz $(SOURCES) $(INCLUDES) Makefile 
