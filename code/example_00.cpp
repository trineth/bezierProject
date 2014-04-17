// CS184 Simple OpenGL Example
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265
float posX = 0.0f;
float posY = 0.0f;


using namespace std;

//****************************************************
// Some Classes
//****************************************************
class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport    viewport;

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport(0,0,viewport.w,viewport.h);// sets the rectangle that will be the window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                // loading the identity matrix for the screen

  //----------- setting the projection -------------------------
  // glOrtho sets left, right, bottom, top, zNear, zFar of the chord system


  // glOrtho(-1, 1 + (w-400)/200.0 , -1 -(h-400)/200.0, 1, 1, -1); // resize type = add
  // glOrtho(-w/400.0, w/400.0, -h/400.0, h/400.0, 1, -1); // resize type = center

  glOrtho(-1, 1, -1, 1, 1, -1);    // resize type = stretch

  //------------------------------------------------------------
}


//****************************************************
// sets the window up
//****************************************************
void initScene(){
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent

  myReshape(viewport.w,viewport.h);
}


//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {


  //----------------------- ----------------------- -----------------------
  // animation
  static float stickPos = 0.6f;
  float end = -0.75f;
  float beg = 0.95f;
  float incr = 0.005f;

  stickPos = stickPos - incr;
  if (stickPos < end) {
    stickPos = beg;
  }
  //----------------------- ----------------------- -----------------------


  glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)

  glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
  glLoadIdentity();                            // make sure transformation is "zero'd"

  //----------------------- code to draw objects --------------------------
  // Car Code
  // Trapezoid Code
  //glColor3f(red component, green component, blue component);
  glColor3f(1.0f,0.0f,0.0f);                   // setting the color to pure red 90% for the rect

  glBegin(GL_POLYGON);                         // draw trapezoid 
  //glVertex3f(x val, y val, z val (won't change the point because of the projection type));
  glVertex3f(-0.4f + posX, 0.2f + posY, 0.0f);               // bottom left corner of trapezoid
  glVertex3f(-0.2f + posX, 0.4f + posY, 0.0f);               // top left corner of trapezoid
  glVertex3f( 0.2f + posX, 0.4f + posY, 0.0f);               // top right corner of trapezoid
  glVertex3f( 0.4f + posX, 0.2f + posY, 0.0f);               // bottom right corner of trapezoid
  glEnd();

  // Rectangle Code
  glColor3f(1.0f,0.0f,0.0f);

  glBegin(GL_POLYGON);
  glVertex3f(-0.5f + posX, 0.2f + posY, 0.0f);               // top left corner of the rectangle
  glVertex3f(-0.5f + posX, 0.0f + posY, 0.0f);               // bottom left corner of the rectangle
  glVertex3f( 0.7f + posX, 0.0f + posY, 0.0f);               // bottom right corner of the rectangle
  glVertex3f( 0.7f + posX, 0.2f + posY, 0.0f);               // top right corner of the rectangle
  glEnd();

  //Wheels Code
  // Circle1 Code
  glColor3f(0.1f,0.1f,0.5f);

  glBegin(GL_POLYGON);
  glVertex3f(-0.3f + posX, 0.0f + posY, 0.0f);               // top left corner of the rectangle
  glVertex3f(-0.3f + posX,-0.1f + posY, 0.0f);               // bottom left corner of the rectangle
  glVertex3f(-0.2f + posX,-0.1f + posY, 0.0f);               // bottom right corner of the rectangle
  glVertex3f(-0.2f + posX, 0.0f + posY, 0.0f);               // top right corner of the rectangle
  glEnd();

  // Circle2 Code
  glColor3f(0.1f,0.1f,0.5f);

  glBegin(GL_POLYGON);
  glVertex3f( 0.3f + posX, 0.0f + posY, 0.0f);               // top left corner of the rectangle
  glVertex3f( 0.3f + posX,-0.1f + posY, 0.0f);               // bottom left corner of the rectangle
  glVertex3f( 0.4f + posX,-0.1f + posY, 0.0f);               // bottom right corner of the rectangle
  glVertex3f( 0.4f + posX, 0.0f + posY, 0.0f);               // top right corner of the rectangle
  glEnd();


  // Ground Code
  glColor3f(0.6f,0.6f,0.6f);                   // setting the color to orange for the triangle

  glBegin(GL_POLYGON);
  glVertex3f(-0.7f,-0.1f, 0.0f);               // top left corner of the rectangle
  glVertex3f(-0.7f,-0.2f, 0.0f);               // bottom left corner of the rectangle
  glVertex3f( 0.9f,-0.2f, 0.0f);               // bottom right corner of the rectangle
  glVertex3f( 0.9f,-0.1f, 0.0f);               // top right corner of the rectangle
  glEnd();

  // Moving Stick Code
  glColor3f(1.39f,0.69f,0.19f);

  glBegin(GL_POLYGON);
  glVertex3f(stickPos,0.6f, 0.1f);               // top left corner of the rectangle
  glVertex3f(stickPos,-0.1f, 0.1f);               // bottom left corner of the rectangle
  glVertex3f(stickPos + 0.1f,-0.1f, 0.1f);               // bottom right corner of the rectangle
  glVertex3f(stickPos + 0.1f,0.6f, 0.1f);               // top right corner of the rectangle
  glEnd();
  //-----------------------------------------------------------------------

  glFlush();
  glutSwapBuffers();                           // swap buffers (we earlier set double buffer)
}


//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
  //nothing here for now
#ifdef _WIN32
  Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
#endif
  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}

void processSpecialKeys(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_UP :
      posY += 0.01f; break;
    case GLUT_KEY_DOWN :
      posY -= 0.01f; break;
    case GLUT_KEY_LEFT :
      posX -= 0.01f; break;
    case GLUT_KEY_RIGHT :
      posX += 0.01f; break;
  }
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("CS184!");

  initScene();                                 // quick function to set up scene

  glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
  glutReshapeFunc(myReshape);                  // function to run when the window gets resized
  glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
  glutSpecialFunc(processSpecialKeys);         // moves the car upon arrow key press
  glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

  return 0;
}








