#include <glut.h>
#include "svl/svl.h"
#include <vector>
#include <deque>
#include <iostream>
#include "gl/glpng.h"
using namespace std;
#pragma comment(lib, "gifcapture.lib") 
extern void ProcessCapture(int,int=10);
GLuint view;
void night()
{
	
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, view);
	glEnable (GL_BLEND);
	glBegin (GL_QUADS);
	glTexCoord2f (0,0),glVertex2f (-10,-10);
	glTexCoord2f (0,1),glVertex2f (-10,10);
	glTexCoord2f (1,1),glVertex2f (10,10);
	glTexCoord2f (1,0),glVertex2f (10,-10);
	glEnd();
	glDisable (GL_TEXTURE_2D);
}

void special (int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		ProcessCapture(glutGetWindow ());
	}
}


typedef struct _particle {
	Vec2 pos, vel;
	bool b;//Save up the bom points
	bool b2;//Save up the second bom points
	int birth_time;  // birth_timestamp
	deque<Vec2> trail;
} PARTICLE;

vector<PARTICLE> particles; 
vector<PARTICLE> bom; 
Vec2 force(0,-1);  // gravity

//PARTICLE p;
int now;

float frand(float lo, float hi)
{
	int r = rand()%143;
	return lo + (hi-lo)*r/142.;
}

void display()
{
	glClear (GL_COLOR_BUFFER_BIT);
	
	glColor3f(1,1,1);
    night();//view
	
#define max(x,y) ((x)>(y)?(x):(y))
	for (int i = 0; i < particles.size(); i++) {
		PARTICLE p = particles[i];
		float life = (now - p.birth_time)/1000.;
		float intensity = max(0, (10 - life)/10);

		glColor3f (intensity,intensity,intensity);
		glBegin (GL_POINTS);
		glVertex2dv (p.pos.Ref());	
		glEnd();
		
#if 1   
		if(!p.b){
			glBegin (GL_POINTS);
			glVertex2dv (p.pos.Ref());	
			glEnd();
		}
		// trail display
		glBegin (GL_LINE_STRIP);
		for (int i = 0; i < p.trail.size(); i++) {
			glColor4f (intensity*1,intensity*1,0,1.0*i/p.trail.size());
			glVertex2dv (p.trail[i].Ref());
		}
		glEnd();
#endif
	}
		for (int a = 0; a < bom.size(); a++) {
		PARTICLE p = bom[a];
		float life = (now - p.birth_time)/1000.;
		float intensity = max(0, (10 - life)/10);
        glBegin (GL_POINTS);
		glVertex2dv (p.pos.Ref());	
		glEnd();

		// trail display
		glBegin (GL_LINE_STRIP);
		for (int i = 0; i < p.trail.size(); i++) {
			glColor4f (intensity*1,intensity*1,0,1.0*i/p.trail.size());
			glVertex2dv (p.trail[i].Ref());
		}
		glEnd();
		

	}

	glutSwapBuffers();
}

PARTICLE init_particle ()
{
	PARTICLE p;
	p.birth_time = glutGet (GLUT_ELAPSED_TIME);
	float start = frand(-3,-1);
	p.pos = Vec2 (start,0);
	start = frand (-.3,.3);
	p.vel = Vec2 (start,5);
	p.trail.push_back (p.pos);
	p.b = false;
	p.b2 = false;
	return p;
}

void init_particle2 ( PARTICLE copy )//second bom
{
	float angle = 0.0;
	float r = 5;
	copy.trail.clear();
	for(int i=0; i<36; i++){
		PARTICLE p = copy;
		p.vel = Vec2 (r*cos(angle*3.14/180),r*sin(angle*3.14/180));
		p.trail.push_back (p.pos);
		bom.push_back (p);
		angle += 10.0;
	}
}

float life (PARTICLE p) 
{
	return	(now - p.birth_time)/1000.;
}
#define TRAIL_SIZE 40

void timer(int dummy)
{
	double dt = 0.1;  // seconds
cout << particles.size() << endl;
	now = glutGet (GLUT_ELAPSED_TIME);

	for (int b = 0; b < particles.size(); b++) {
		PARTICLE p = particles[b];
		if (!p.b) 
			p.pos += p.vel * dt;
		if (p.vel[1] < 0.0)
			p.b = true;

		if(p.b && !p.b2){
			p.b2 = true;
			init_particle2 ( p );
		}

		p.trail.push_back (p.pos);
		if (p.trail.size() > TRAIL_SIZE)
			p.trail.pop_front();
		p.vel += force *dt;

		// restore pos/vel back to vector
		particles[b] = p;

		if (life(p) > 9.0) {
			particles.erase (particles.begin()+b); // remove from vector		
		}
	
	
	}
for (int i = 0; i < bom.size(); i++) {
		PARTICLE p = bom[i];

		p.pos += p.vel * dt;
		p.trail.push_back (p.pos);
		if (p.trail.size() > TRAIL_SIZE)
			p.trail.pop_front();
		p.vel += force *dt;

		// restore pos/vel back to vector
		bom[i] = p;

		if (life(p) > 9.0) {
			bom.erase (bom.begin()+i); // remove from vector		
		}
		
	}

	glutTimerFunc (100, timer, 0);
	glutPostRedisplay();
}

void init()
{
	// pipeline
    glViewport (0,0,400,400);
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluOrtho2D (-10,10,-4,16);
	glMatrixMode (GL_MODELVIEW);
	pngSetStandardOrientation(1);  // must use this !!
	view = pngBind("night.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_NEAREST, GL_NEAREST);


	// graphics setting
    glEnable (GL_POINT_SMOOTH);
    glPointSize (10);
	glLineWidth (5.0);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);
}

void keyboard (unsigned char key, int x, int y)
{
	if (key == ' ') {
		particles.push_back (init_particle ());
	}
	glutPostRedisplay();
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize (400,400);
	glutCreateWindow ("firework");
	
	glutDisplayFunc (display);
	glutTimerFunc (100, timer, 0);
	glutKeyboardFunc (keyboard);

	init();
	glutSpecialFunc (special);
	
	glutMainLoop();
}
