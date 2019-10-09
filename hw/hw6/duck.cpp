#include "duck.h"

#define RESPAWN_TIME 100


/*void Status () {
	if (-1+xpos > 5){
		flip = flip*(-1);
		tag = 0;
	}
	if (-1+xpos < -5){
		flip = flip*(-1);
		tag = 1;
	}
}*/

/*void timer (int dummy)
{
    if(die == 1) {
	    angle = angle - 45;
		if(angle == -90) {		
		    die = 0;
			temp = 1;
		}
	}	
	c = c + temp;
	if(c == 5) {
		angle = angle + 45;
		temp = 0;
		if(angle == 0)
			c = 0;
	}
	
	if (tag == 1) {
	    xpos = xpos+0.1;
		Status();
	}
	else {
		xpos = xpos-0.1;	
		Status();		
	}	


	glutTimerFunc (100, timer, 0);
	glutPostRedisplay();
}*/

void Duck::step()
{
	//drop, theta related
	if (hit) {
		theta += 6;
		if (theta >= 90) {
			theta = 90;
			hit = 0;
			respawn = 1;
			respawn_timer = RESPAWN_TIME;
		}
	}
	if (respawn) {
		--respawn_timer;
		if (respawn_timer < 0) {
			respawn = 0;
			theta = 0;
		}
	}

	move += flip*0.25;
	if (fabs(move) > 3) {
		flip *= -1;
	}
	glutPostRedisplay();

/*	move += flip*0.25;
	if(fabs(move) > 3)
	{
		flip *= -1;
	}
	glutPostRedisplay();*/
}
void Duck::render()
{
	int rendermode;
	glGetIntegerv (GL_RENDER_MODE, &rendermode);

	glPushMatrix();
	glTranslatef (pos[0]+move, pos[1],pos[2]);
	glRotatef (theta,-1,0,0);

	//render in both select & render modes
	glPushName(name);
	aim();
	glPopName();

	//only render in render mode
	if (rendermode == GL_RENDER) {

		glPushAttrib (GL_ALL_ATTRIB_BITS);

		glEnable (GL_ALPHA_TEST);
		glAlphaFunc (GL_GREATER, 0.0);

		glBegin (GL_QUADS);
		glMultiTexCoord2f (GL_TEXTURE0, 0,0);glMultiTexCoord2f (GL_TEXTURE1, 0,0); glVertex2i (-1,0);
		glMultiTexCoord2f (GL_TEXTURE0, flip*1,0);glMultiTexCoord2f (GL_TEXTURE1, 1,0); glVertex2i (1,0);
		glMultiTexCoord2f (GL_TEXTURE0, flip*1,1);glMultiTexCoord2f (GL_TEXTURE1, 1,1); glVertex2i (1,2);
		glMultiTexCoord2f (GL_TEXTURE0, 0,1);glMultiTexCoord2f (GL_TEXTURE1, 0,1); glVertex2i (-1,2);
		glEnd();

		glPopAttrib();
	}
	glPopMatrix();
}