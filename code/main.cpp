// CS184 Simple OpenGL Example
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "patch.h"
#include "parser.h"
#include "point.h"
#include "bezier.h"
#include "quad.h"

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
float posX = 80.0f;
float posY = -384.0f;
float posZ = -2.0f;
float fov = 1.0f;
float angle = 90;

std::vector<Quad> rquads;

bool wire = false;
bool adaptive = false;

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
Parser parser;

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
void initScene(int argc, char *argv[]) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear to black, fully transparent

  myReshape(viewport.w,viewport.h);
  parser.parse(argc, argv);

  int patchNum = parser.getPatchNumber();
  std::vector<Patch> patches = parser.getPatches();
  float subd = parser.getSubdivision();

  if (!adaptive) {
    for (int i = 0; i < patchNum; i++) {
      Patch patch = patches[i];
      Bezier bez;
      bez.subdividepatch(patch, subd);
      int quadNum = bez.getQuadNum();
      for (int j = 0; j < quadNum; j++) {
        Quad quad = bez.getQuad(j);
        rquads.push_back(quad);
      }
    }
  }
}

void renderVertex(Point points[], int i) {
    float* values = points[i].getValues();
    float* normal = points[i].getNormal(posX,posY,posZ);
    glNormal3f(normal[0], normal[1], normal[2]);
    glVertex3f(values[0], values[1], values[2]);
}

void renderQuadAsTriangles(Point points[]) {
    // float* values;
    // float* normal;
    glBegin(GL_TRIANGLES);
    renderVertex(points, 0);
    renderVertex(points, 2);
    renderVertex(points, 1);
    glEnd();

    glBegin(GL_TRIANGLES);
    renderVertex(points, 0);
    renderVertex(points, 3);
    renderVertex(points, 2);
    glEnd();
}

//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (wire) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  }


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, 1.0, 0.1, 800.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(posX, posY, posZ, 0, 0, 0, 0, 1, 0);
  glRotatef(angle, 0, 1, 0);
  glColor3f(0.5f, 0.5f, 0.5f);

  // Enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  GLfloat lightpos[] = {10, 10, 10, 0.};
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  glPointSize(3.0f);

  // //Rendering just the points
  // glPointSize(6.0f);
  // glBegin(GL_LINES);
  // Point* points;
  // float* values;
  // for (int i = 0; i < patchNum; i++) {
  //   Patch patch = patches[i];
  //   points = patch.getPoints();
  //   for (int j = 0; j < 16; j++) {
  //     values = points[j].getValues();
  //     glVertex3f(values[0], values[1], values[2]);
  //   }

  // }
  // glEnd();


  //uniform tesselation
  if (!adaptive) {
    for (int i = 0; i < rquads.size(); i++) {
      renderQuadAsTriangles(rquads[i].getPoints());
    }
  }

  glFlush();
  glutSwapBuffers();
  std::cout << "Pos X, Y, Z:" << posX << " " << posY << " " << posZ << "\n";
  std::cout << "fov, angle:" << fov << " " << angle << " " << "\n";
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

void processKeys(unsigned char key, int x, int y) {
  switch(key) {
    case 115: // s
      wire = !wire; break;
    case 43: // +
      angle += 1.0f; break;
    case 45: // -
      angle -= 1.0f; break;
  }
}

void processSpecialKeys(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_UP :
      posY += 1.0f; break;
    case GLUT_KEY_DOWN :
      posY -= 1.0f; break;
    case GLUT_KEY_LEFT :
      posX -= 1.0f; break;
    case GLUT_KEY_RIGHT :
      posX += 1.0f; break;
    case GLUT_KEY_PAGE_UP :
      posZ += 1.0f; break;
    case GLUT_KEY_PAGE_DOWN :
      posZ -= 1.0f; break;
    case GLUT_KEY_F11 :
      fov += 1.0f; break;
    case GLUT_KEY_F12 :
      fov -= 1.0f; break;
    case GLUT_KEY_F1 :
      angle += 1.0f; break;
    case GLUT_KEY_F2 :
      angle -= 1.0f; break;
  }
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Initalize theviewport size
  viewport.w = 800;
  viewport.h = 800;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("BezierProject");

  initScene(argc, argv);                                 // quick function to set up scene


  glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
  glutReshapeFunc(myReshape);                  // function to run when the window gets resized
  glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
  glutKeyboardFunc(processKeys);
  glutSpecialFunc(processSpecialKeys);         
  glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

  return 0;
}








