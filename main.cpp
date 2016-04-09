#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "logic.h"

// Global variables //
float nodes = 200;
int angle = 0;
float times = 0;
float EPS = 1000;
float step = 0.001;
std::deque <std::pair<double, double>> points;
//Color values
int state=0;
float R=1.0;
float G=0.0;
float B=0.0;

//Window values
float Width=720.0;
float Height=720.0;

//Playback values
bool display_help = false;
bool animate = true;
bool input = false;

//Manipulation values
std::string cmd, val;
bool done = false;
int wch=0;
bool start = true;

// Functions prototypes //
std::deque<std::pair<double, double> > GenCircle(int);
void DrawData();
void AddColor(float);
void DrawCircle(float);
void DrawNode(double, double, double);

// Color modification //
void AddColor(float val)
{
    if(state == 0)
        B += val;
    if(state == 0 && R >= 1.0 && B >= 1.0)
        state = 1;

    if(state == 1){
        R -= val;
        G += val;
    }
    if(state == 1 && R <= 0.0 && G >= 1.0)
        state = 2;

    if(state == 2){
        B -= val;
    }
    if(state == 2 && R <= 0.0 && B <= 0.0)
        state = 3;

    if(state == 3){
        R += val;
        G -= val;
    }
    if(state == 3 && R >= 1.0 && G <= 0.0)
        state = 4;


    if(state == 4){
        G += val;
    }
    if(state == 4 && G >= 1.0)
        state = 5;

    if(state == 5){
        R -= val;
        B += val;
    }
    if(state == 5 && R <= 0.0 && B >= 1.0)
        state = 6;

    if(state == 6){
        R += val;
        B -= val;
    }
    if(state == 6 && R >= 1.0 && B <= 0.0)
        state = 7;

    if(state == 7){
        G -= val;
    }
    if(state == 7 && G <= 0.0)
        state = 0;

    if(R > 1)
        R = 1;
    else if(R < 0)
        R = 0;
    if(G > 1)
        G = 1.0;
    else if(G < 0)
        G = 0;
    if(B > 1)
        B = 1.0;
    else if(B < 0)
        B = 0;
}

// Generation //
std::deque<std::pair<double, double> > GenCircle(int parts) {
	std::deque<std::pair<double, double> > table;
	double rad = (Width-20)/2, OX = Width/2, OY = Height/2;

	for (int i = 1; i <= parts; i++) {
		table.push_back(std::pair<double, double> {(OX + std::cos(double(i) / parts * 3.14 * 2)*rad), OY + std::sin(double(i) / parts * 3.14 * 2)*rad});
	}

  tmt::move(table, angle);
	return table;
}

// Draw functions //
void DrawData()
{
  glColor3f(std::abs(R-0.2), std::abs(G-0.2), std::abs(B-0.2));

	glBegin(GL_LINES);
	for (int i = 0; i < nodes; i++) {
    int newpos = ((int)(i*times) % (int)nodes);
		glVertex2f(points[i].first, points[i].second);
		glVertex2f(points[newpos].first, points[newpos].second);
	}
	glEnd();
}

void DrawCircle(float rad)
{
	glPointSize(rad);
	glColor3f(1.0, 0.0, 0.0);

	glBegin(GL_POINTS);
	for (auto it = points.begin(); it != points.end(); it++) {
		glVertex2f(it->first, it->second);
	}
	glEnd();
}

void DrawNode(double rad, double x, double y)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(rad);
	glBegin(GL_POINT);
  glVertex2f(x, y);
	glEnd();
}

// GLUT callback handlers //
static void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  if (input) {
    tmt::DrawBitmapString(2, Height-15, GLUT_BITMAP_9_BY_15, cmd+val);
  }

  if (start) {
    glColor3f(0.0,0.0,0.0);
    tmt::DrawBitmapText(2, 15, GLUT_BITMAP_9_BY_15, "TIMES TABLE SIMULATOR\nCODED BY Grigori Dolgov ");
    if (wch == 0) {
      input = true;
      cmd = ">Start from multiplier of ";
    } else if (wch == 1) {
      input = true;
      cmd = ">Set step value to ";
    } else if (wch == 2) {
      input = true;
      wch = 2;
      cmd = ">Set nodes amount to ";
    } else if (wch == 3) {
      start = false;
      wch = 0;
    }
  } else {
		glColor3f(0.0, 0.0, 0.0);
		tmt::DrawBitmapString(2, 15, GLUT_BITMAP_9_BY_15, tmt::ToString(times) + "x times");
		tmt::DrawBitmapString(2, 30, GLUT_BITMAP_9_BY_15, "Nodes: " + tmt::ToString((int)nodes));
		DrawData();
		DrawCircle(2.0);
	}

	if (display_help) {
    glColor3i(0,0,0);
    int shift=130;
    tmt::DrawBitmapText(Width-shift, 15, GLUT_BITMAP_9_BY_15, "Playback keys:\nSPC stop/play\n/ round down\n* round up\n+ step forward\n- step back\n \nValues:\nj jump\ns step\nn nodes");
  }

  if (done) {
    tmt::DrawBitmapString(2, Height-15, GLUT_BITMAP_9_BY_15, "Press AGAIN to exit | OTHER to continue");
  }

	tmt::Render();
}


void key(unsigned char key, int x, int y)
{
    if(done)
    switch(key){
        case 27:
        case 'q':
            exit(0);
        default:
            done = false;
    }
    if(input){
        switch(key)
        {
        case 13:
            if(wch == 0){
                times = atof(val.c_str());
                if(start)
                    wch++;
            }
            else if(wch == 1){
                step = atof(val.c_str());
                if(start)
                    wch++;
            }
            else if(wch == 2){
                nodes = atof(val.c_str());
                //nodes = max_nodes;
                points = GenCircle(nodes);
                if(start)
                    wch++;
            }
        case 27:
            input = false;
            val = "";
            break;
        case 8:
            if(!val.empty())
                val.pop_back();
            break;
        default:
            val.push_back(key);
        }
    }
    else
    switch (key)
    {
    case '+':
        times += step;
        break;
    case '-':
        times -= step;
        break;
    case '/':
        times = std::floor(times);
        break;
    case '*':
        times = std::ceil(times);
        break;
    case 32:
        if(animate)
            animate = false;
        else
            animate = true;
        break;
    case 'j':
        wch = 0;
        cmd = ">Jump to ";
        input = true;
        break;
    case 's':
        wch = 1;
        cmd = ">Set step value to ";
        input = true;
        break;
    case 'n':
        wch = 2;
        cmd = ">Set nodes amount of ";
        input = true;
        break;
    case 'h':
        if(display_help)
            display_help = false;
        else
            display_help = true;
        break;
    case 27 :
    case 'q':
        done = true;
        break;

    }

    glutPostRedisplay();
}

void SpecialInput(int key, int x, int y)
{
switch(key)
{
  case GLUT_KEY_LEFT:
      angle -= nodes/100;
      points = GenCircle(nodes);
      break;
  case GLUT_KEY_RIGHT:
      angle += nodes/100;
      points = GenCircle(nodes);
      break;
  case GLUT_KEY_UP:
      angle += 3.14*nodes;
      points = GenCircle(nodes);
      break;
  case GLUT_KEY_DOWN:
      angle = 0;
      points = GenCircle(nodes);
      break;
}

glutPostRedisplay();
}

static void idle(void)
{
    if(!done){
      if (!input && animate) times += step;
      AddColor(0.0005);
    }
    glutPostRedisplay();
}

//first-time initialization
void animation_init()
{
    points = GenCircle(nodes);
}

// Program entry point //
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(Width, Height);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  glutCreateWindow("Times table simulator");

  glutDisplayFunc(display);
  glutKeyboardFunc(key);
  glutIdleFunc(idle);
  glutSpecialFunc(SpecialInput);

  glMatrixMode(GL_PROJECTION);
  glClearColor(1,1,1,1);
  glLoadIdentity();
	gluOrtho2D(0.0, Width, Height, 0.0);

  animation_init();
  glutMainLoop();

  return EXIT_SUCCESS;
}
