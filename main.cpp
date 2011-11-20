//////////////////////////////////////////////////////////////////////////////////////////////         
// main.cpp
// 
// 
//
// Brendon Justin, Kimberly Tam
//////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

#include "Player.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#ifndef M_PI
static const float M_PI = 3.14159265;
static const float M_PI_2 = 1.57079633;
#endif

// Globals.
static float player1color[3] = {1.0, 0.0, 0.0}; 
static float player2color[3] = {0.0, 0.0, 1.0};
static const long font = (long)GLUT_BITMAP_8_BY_13; // Font selection

static float xVal1 = 0, zVal1 = 0; // Co-ordinates of the bike1
static float xVal2 = 0, zVal2 = 0; // Co-ordinates of the bike2
static float angle1 = 0.0; // Angle of the bike1
static float angle2 = 0.0; // Angle of the bike2
vector<int> path1; //mark start and then every spot turned
vector<int> path2;

static unsigned int cat; // Display lists base index.

static const int arenaheight = 50;
static const int arenawidth = 100;
static const int arenalength = 100;

static int isAnimate = 1; // Animated?   //change back to 0 once have the if commands
static int isCollision = 0;
static int animateRight = 0; //animation for turning right
static int animateLeft = 0; //animation for turning left
static int p1turn = 0;
static int p2turn = 0;
static int turnGoal = 0;

static int animationPeriod = 10; // Time interval between frames.
static int storeOrigPos = 1;

static const int width = 1000, height = 700;

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}  

void drawWallFloors ()
{
	// Draw walls of arena
	glColor3f(0.0, 0.0, 0.8);
	glBegin(GL_QUADS); 
	glVertex3f(arenawidth, arenaheight, -arenalength);                               
	glVertex3f(-arenawidth, arenaheight, -arenalength);                          
	glVertex3f(-arenawidth, -10, -arenalength); 
	glColor3f(0.0, 0.0, 0.4);             
	glVertex3f(arenawidth, -10, -arenalength);
	glColor3f(0.0, 0.0, 0.8);       

	glVertex3f(arenawidth, arenaheight, arenalength);  
	glVertex3f(-arenawidth, arenaheight, arenalength);                          
	glVertex3f(-arenawidth, -10, arenalength);   
	glColor3f(0.0, 0.0, 0.4);              
	glVertex3f(arenawidth, -10, arenalength);  
	glColor3f(0.0, 0.0, 0.8);         

	glVertex3f(-arenawidth, arenaheight, -arenalength);  
	glVertex3f(-arenawidth, arenaheight, arenalength);                         
	glVertex3f(-arenawidth, -10, arenalength); 
	glColor3f(0.0, 0.0, 0.4);               
	glVertex3f(-arenawidth, -10, -arenalength);  
	glColor3f(0.0, 0.0, 0.8);  

	glVertex3f(arenawidth, arenaheight, -arenalength);  
	glVertex3f(arenawidth, arenaheight, arenalength);                         
	glVertex3f(arenawidth, -10, arenalength);   
	glColor3f(0.0, 0.0, 0.4);             
	glVertex3f(arenawidth, -10, -arenalength);   
	glColor3f(0.0, 0.0, 0.8);           
	glEnd(); 

	// tron lit floor
	glColor3f(0.3, 0.3, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for(float z=-arenalength; z<arenalength; z+=2.0)
	{
		glBegin(GL_QUADS);
		for(float x=-arenawidth; x<arenawidth; x+=2.0)
		{
			glVertex3f(x, -10, z);                               
			glVertex3f(x+2, -10, z);                               
			glVertex3f(x+2, -10, z+2);                               
			glVertex3f(x, -10, z+2);                               
		}
		glEnd();
	}
	//make lines for walls too
	/*glColor3f(1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for(float z=-arenalength; z<arenalength; z+=2.0)
	{
		glBegin(GL_QUADS);
		for(float y=-arenaheight; y<arenaheight; y+=2.0)
		{
			glVertex3f(arenawidth, y, z);                               
			glVertex3f(arenawidth, y+2, z);                               
			glVertex3f(arenawidth, y+2, z+2);                               
			glVertex3f(arenawidth, y, z+2);                               
		}
		glEnd();
	}*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int CatCollision(float x, float z, float a)
{
	// Check for collision with all walls
	if(x < (-arenawidth+1.0) || x > (arenawidth-1.0)){
		return 1;
	}

	if(z < (-arenalength+1.0) || z > (arenalength-1.0)){
		return 1;
	}

	return 0;
}

// Drawing routine.
void drawScene(void)
{
	// -- player 1 ----------------------------------------------------
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glViewport(0, height/2.0, width, height/2.0);

	// Locate the camera behind cat 
	gluLookAt(xVal1 + 10 * sin( (M_PI/180.0) * angle1), 
		-4.0, 
		zVal1+ 10 * cos( (M_PI/180.0) * angle1), 
		xVal1,
		-4.0,
		zVal1, 
		0.0, 
		1.0, 
		0.0);

	//draw walls and floor
	glPushMatrix();
	drawWallFloors();
	glPopMatrix();

	//draw bike
	glPushMatrix();
	glTranslatef(xVal1,-10.0, zVal1);
	glRotatef(angle1, 0.0, 1.0, 0.0);
	glColor3fv(player1color);
	glCallList(cat);
	glPopMatrix();

	//write player 1
	glPushMatrix();
	glTranslatef(0.0, 0.0, -30.0);
	glColor3fv(player1color);
	glRasterPos3f(-28.0, 25.0, 0.0);
	writeBitmapString((void*)font, (char*)"PLAYER ONE"); 
	glPopMatrix();

	if(isCollision){
		glPushMatrix();
		glTranslatef(0.0, 0.0, -50);
		glColor3fv(player1color);
		glRasterPos3f(-28.0, 25.0, 0.0);
		writeBitmapString((void*)font, (char*)"CRAHSED!!!!!"); 
		glPopMatrix();
	}

	//NyaWall(path1);
	for(int i=0; i<path1.size(); i=i+3){
		cout << path1[i] << "  " << path1[i+1] << "  ";
		glColor3f(0.0, 0.0, 0.0);  
		glPushMatrix();
		glBegin(GL_QUADS); 
		glVertex3f(path1[i], arenaheight, -path1[i+1]);                               
		glVertex3f(-path1[i], arenaheight, -path1[i+1]);                          
		glVertex3f(-path1[i], -10, -path1[i+1]);            
		glVertex3f(path1[i], -10, -path1[i+1]);  
		glEnd();
		glPopMatrix();
	}
	cout << endl;
	
	//-- repeat for player 2 ---------------------------------------------
	glLoadIdentity();
	glViewport(0, 0, width, height/2.0);

	//write player 2
	glPushMatrix();
	glTranslatef(0.0, 0.0, -30.0);
	glColor3fv(player1color);
	glRasterPos3f(-28.0, 25.0, 0.0);
	writeBitmapString((void*)font, (char*)"PLAYER TWO"); 

	// Place camera.
	gluLookAt(xVal2 + 10 * sin( (M_PI/180.0) * angle2), 
		-4.0, 
		zVal2+ 10 * cos( (M_PI/180.0) * angle2), 
		xVal2,
		-4.0,
		zVal2, 
		0.0, 
		1.0, 
		0.0); 

	//draw walls and floor
	drawWallFloors();
	if(isCollision){
		glPushMatrix();
		glTranslatef(0.0, 0.0, -60);
		glColor3fv(player1color);
		glRasterPos3f(-28.0, 25.0, 0.0);
		writeBitmapString((void*)font, (char*)"You Win!!!!!"); 
		glPopMatrix();
	}

	glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
	float goal, tempAngle, stillturn=0;
	isCollision = CatCollision(xVal1, zVal1, angle1);
	if(!isCollision){
		if (isAnimate) 
		{
			xVal1 = xVal1 - sin(angle1 * M_PI/180.0); 
			zVal1 = zVal1 - cos(angle1 * M_PI/180.0);

			xVal2 = xVal2 - sin(angle1 * M_PI/180.0); 
			zVal2 = zVal2 - cos(angle1 * M_PI/180.0);
		}
		if (animateLeft)
		{
			//dissable right while going left
			isAnimate=0;
			if(storeOrigPos)
			{
				goal = angle1+90.0;
				if(goal>360.0){goal = goal-360.0; stillturn=1;}
				turnGoal = goal;
				//cout << "turning" << goal << endl;
				storeOrigPos = 0;
			}
			else goal = turnGoal;
			tempAngle = angle1;
			if(angle1<goal||stillturn)
			{	
				tempAngle=tempAngle+10.0;
				if (tempAngle > 360.0) tempAngle -= 360.0;
				if (tempAngle < 0.0) tempAngle += 360.0;
				angle1 = tempAngle;
				//cout << goal << "   " << angle << endl;
			}
			else
			{
				animateLeft = 0;
				storeOrigPos = 1;
				isAnimate = 1;
			}
		
		}

		if (animateRight)
		{
			isAnimate=0;
			if(storeOrigPos)
			{
				goal = angle1-90.0;
				if(goal<0.0){goal = goal+360.0; stillturn=1;}
				turnGoal = goal;
				//cout << "turning" << goal << endl;
				storeOrigPos = 0;

			}
			else goal = turnGoal;
			tempAngle = angle1;
			if(angle1>goal||stillturn)
			{	
				tempAngle=tempAngle-10.0;
				if (tempAngle > 360.0) tempAngle -= 360.0;
				if (tempAngle < 0.0) tempAngle += 360.0;
				angle1 = tempAngle;
				//cout << goal << "   " << angle << endl;
			}
			else
			{
				animateRight = 0;
				storeOrigPos = 1;
				isAnimate = 1;
			}
		
		}
	}
	glutTimerFunc(animationPeriod, animate, 1);
	glutPostRedisplay();
}

// Initialization routine.
void setup(void) 
{
	Player player1;
	glClearColor(1.0, 1.0, 1.0, 0.0);  
	cat = glGenLists(1);
	glNewList(cat, GL_COMPILE);

	glPushMatrix();
	player1.DrawCat();
	glPopMatrix();

	glEndList();

	// starting positions for bikes
	// Initialize global iterators for manVector.
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch(key) 
	{
	case 27:
		exit(0);
		break;         
	}
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	//need left right for player 1, a and d for player 2?
	float tempxVal = xVal1, tempzVal = zVal1, tempAngle = angle1, newAngle;

	p1turn = 0;
	p2turn = 0;
	// Compute next position.
	if (key == GLUT_KEY_LEFT){  
		animateLeft = 1;
		p1turn = 1;
		path1.push_back(xVal1);
		path1.push_back(zVal1);
	}
	if (key == GLUT_KEY_RIGHT){
 		animateRight = 1;
		p1turn = 1;
		path1.push_back(xVal1);
		path1.push_back(zVal1);
	}

	if( key == 'a')
	{
		animateLeft = 1;
		p2turn = 1;
		path2.push_back(xVal2);
		path2.push_back(zVal2);
	}
	if( key == 'd')
	{
		//animateRight = 1;
		p2turn = 1;
		path2.push_back(xVal2);
		path2.push_back(zVal2);
	}

	// Angle correction.
	if (tempAngle > 360.0) tempAngle -= 360.0;
	if (tempAngle < 0.0) tempAngle += 360.0;

	// Move bike to next position only if there will not be collision
	if (!CatCollision(tempxVal, tempzVal, tempAngle) )
	{
		isCollision = 0;
		xVal1 = tempxVal;
		zVal1 = tempzVal;
		angle1 = tempAngle;
	}
	else isCollision = 1;
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{

}

// Main routine.
int main(int argc, char **argv) 
{
	printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100); 
	glutCreateWindow ("steve"); 

	setup(); 

	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutTimerFunc(5, animate, 1);
	glutMainLoop(); 

	return 0;  
}

