CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Icode/glut-3.7.6-bin
	LDFLAGS = -lglut -lGL
endif
	
RM = /bin/rm -f 
all: main 
main: code/bezier.o 
	$(CC) $(CFLAGS) -o bezier code/bezier.o $(LDFLAGS) 
code/bezier.o: code/bezier.cpp
	$(CC) $(CFLAGS) -c code/bezier.cpp -o code/bezier
clean: 
	$(RM) *.o code/*.o bezier
 

