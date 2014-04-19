CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Icode/glut-3.7.6-bin
	LDFLAGS = -lglut -lGL -lGLU
endif
	
RM = /bin/rm -f 
all: main
main: code/main.o code/parser.o code/patch.o code/point.o code/bezier.o code/quad.o
	$(CC) $(CFLAGS) -o main code/point.o code/patch.o \
	code/parser.o code/bezier.o code/quad.o code/main.o $(LDFLAGS)

code/main.o: code/main.cpp code/patch.cpp code/point.cpp code/parser.cpp code/bezier.cpp code/quad.cpp
	$(CC) $(CFLAGS) -c code/main.cpp -o code/main.o

code/parser.o: code/parser.cpp code/patch.cpp code/point.cpp
	$(CC) $(CFLAGS) -c code/parser.cpp -o code/parser.o

code/bezier.o:  code/bezier.cpp code/patch.cpp code/point.cpp
	$(CC) $(CFLAGS) -c code/bezier.cpp -o code/bezier.o

code/patch.o: code/patch.cpp code/point.cpp
	$(CC) $(CFLAGS) -c code/patch.cpp -o code/patch.o

code/point.o: code/point.cpp
	$(CC) $(CFLAGS) -c code/point.cpp -o code/point.o

code/quad.o: code/quad.cpp code/point.cpp
	$(CC) $(CFLAGS) -c code/quad.cpp -o code/quad.o

clean: 
	$(RM) *.o code/*.o main
 

