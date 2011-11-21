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

// Globals.
#ifndef M_PI
static const float M_PI = 3.14159265;
#endif

#ifndef M_PI_2
static const float M_PI_2 = 1.57079633;
#endif

static const bool enablePlayer2 = false;

static float player1color[3] = {1.0, 0.0, 0.0}; 
static float player2color[3] = {0.0, 0.0, 1.0};
static const long font = (long)GLUT_BITMAP_8_BY_13; // Font selection

Player *player1;
Player *player2;

static unsigned int cat; // Display lists base index.

static const int ARENA_HEIGHT = 500;
static const int ARENA_WIDTH = 100;
static const int ARENA_LENGTH = 100;

static const unsigned int TRAIL_HEIGHT = 10;

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
	glVertex3f(ARENA_WIDTH, ARENA_HEIGHT, -ARENA_LENGTH);                               
	glVertex3f(-ARENA_WIDTH, ARENA_HEIGHT, -ARENA_LENGTH);                          
	glVertex3f(-ARENA_WIDTH, -10, -ARENA_LENGTH); 
	glColor3f(0.0, 0.0, 0.4);             
	glVertex3f(ARENA_WIDTH, -10, -ARENA_LENGTH);
	glColor3f(0.0, 0.0, 0.8);       

	glVertex3f(ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);  
	glVertex3f(-ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);                          
	glVertex3f(-ARENA_WIDTH, -10, ARENA_LENGTH);   
	glColor3f(0.0, 0.0, 0.4);              
	glVertex3f(ARENA_WIDTH, -10, ARENA_LENGTH);  
	glColor3f(0.0, 0.0, 0.8);         

	glVertex3f(-ARENA_WIDTH, ARENA_HEIGHT, -ARENA_LENGTH);  
	glVertex3f(-ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);                         
	glVertex3f(-ARENA_WIDTH, -10, ARENA_LENGTH); 
	glColor3f(0.0, 0.0, 0.4);               
	glVertex3f(-ARENA_WIDTH, -10, -ARENA_LENGTH);  
	glColor3f(0.0, 0.0, 0.8);  

	glVertex3f(ARENA_WIDTH, ARENA_HEIGHT, -ARENA_LENGTH);  
	glVertex3f(ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);                         
	glVertex3f(ARENA_WIDTH, -10, ARENA_LENGTH);   
	glColor3f(0.0, 0.0, 0.4);             
	glVertex3f(ARENA_WIDTH, -10, -ARENA_LENGTH);   
	glColor3f(0.0, 0.0, 0.8);           
	glEnd(); 

	// tron lit floor
	glColor3f(0.3, 0.3, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for(float z=-ARENA_LENGTH; z<ARENA_LENGTH; z+=2.0)
	{
		glBegin(GL_QUADS);
		for(float x=-ARENA_WIDTH; x<ARENA_WIDTH; x+=2.0)
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
	for(float z=-ARENA_LENGTH; z<ARENA_LENGTH; z+=2.0)
	{
		glBegin(GL_QUADS);
		for(float y=-ARENA_HEIGHT; y<ARENA_HEIGHT; y+=2.0)
		{
			glVertex3f(ARENA_WIDTH, y, z);                               
			glVertex3f(ARENA_WIDTH, y+2, z);                               
			glVertex3f(ARENA_WIDTH, y+2, z+2);                               
			glVertex3f(ARENA_WIDTH, y, z+2);                               
		}
		glEnd();
	}*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int CatCollision(float x, float z, float a)
{
	// Check for collision with all walls
	if(x < (-ARENA_WIDTH+1.0) || x > (ARENA_WIDTH-1.0)){
		return 1;
	}

	if(z < (-ARENA_LENGTH+1.0) || z > (ARENA_LENGTH-1.0)){
		return 1;
	}

	return 0;
}

// Drawing routine.
void drawScene(void)
{
	// -- player 1 ----------------------------------------------------
	Point player1Pt = player1->positions[player1->positions.size() - 1];
	vector<Point> *player1Pts;
	player1Pts = &(player1->positions);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glViewport(0, height/2.0, width, height/2.0);

	// Locate the camera behind cat 
	gluLookAt(player1Pt.x + 10 * sin( player1->direction * M_PI_2), 
		-4.0, 
		player1Pt.z + 10 * cos( player1->direction * M_PI_2), 
		player1Pt.x,
		-4.0,
		player1Pt.z, 
		0.0, 
		1.0, 
		0.0);

	//	draw walls and floor
	glPushMatrix();
	drawWallFloors();
	glPopMatrix();

	//	draw cat
	glPushMatrix();
	glTranslatef(player1Pt.x, -10.0, player1Pt.z);
	glRotatef(player1->direction * 90, 0.0, 1.0, 0.0);
	glColor3fv(player1color);
	glCallList(cat);
	glPopMatrix();

	//	write player 1
	//glPushMatrix();
	//glTranslatef(0.0, 0.0, -30.0);
	//glColor3fv(player1color);
	//glRasterPos3f(-28.0, 25.0, 0.0);
	//writeBitmapString((void*)font, (char*)"PLAYER ONE"); 
	//glPopMatrix();

	if(isCollision){
		glPushMatrix();
		glTranslatef(0.0, 0.0, -50);
		glColor3fv(player1color);
		glRasterPos3f(-28.0, 25.0, 0.0);
		writeBitmapString((void*)font, (char*)"CRAHSED!!!!!"); 
		glPopMatrix();
	}

	//NyaWall(path1);
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	glColor3fv(player1color);
	glBegin(GL_QUADS); 

	for(int i=0; i < player1Pts->size() - 1; ++i) {
		player1Pt = (*player1Pts)[i];

		//cout << player1Pt.x << "  " << player1Pt.z << "  ";
		//	Top left, top right, bottom right, bottom left
		glVertex3f(player1Pt.x, TRAIL_HEIGHT, player1Pt.z);
		glVertex3f((*player1Pts)[i+1].x, TRAIL_HEIGHT, (*player1Pts)[i+1].z);
		glVertex3f((*player1Pts)[i+1].x, -10, (*player1Pts)[i+1].z);
		glVertex3f(player1Pt.x, -10, player1Pt.z);
	}

	glEnd();
	glPopMatrix();

	//cout << endl;

	//-- repeat for player 2 ---------------------------------------------
	if (enablePlayer2) {
		Point player2Pt = player2->positions[player2->positions.size() - 1];
		vector<Point> *player2Pts;
		player2Pts = &(player2->positions);

		glLoadIdentity();
		glViewport(0, 0, width, height/2.0);

		// Locate the camera behind cat 
		gluLookAt(player2Pt.x + 10 * sin( player2->direction * M_PI_2), 
			-4.0, 
			player2Pt.z + 10 * cos( player2->direction * M_PI_2), 
			player2Pt.x,
			-4.0,
			player2Pt.z, 
			0.0, 
			1.0, 
			0.0);

		//draw walls and floor
		glPushMatrix();
		drawWallFloors();
		glPopMatrix();

		//draw cat
		glPushMatrix();
		glTranslatef(player2Pt.x, -10.0, player2Pt.z);
		glRotatef(player2->direction * 90, 0.0, 1.0, 0.0);

		//write player 2
		glPushMatrix();
		glTranslatef(0.0, 0.0, -30.0);
		glColor3fv(player1color);
		glRasterPos3f(-28.0, 25.0, 0.0);
		writeBitmapString((void*)font, (char*)"PLAYER TWO"); 
	}

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
	Point player1Pt = player1->positions[player1->positions.size() - 1];

	isCollision = CatCollision(player1Pt.x, player1Pt.z, player1->direction * 90);
	if(!isCollision){
		if (isAnimate) 
		{
			player1->tick();
			player2->tick();
		}
//		if (animateLeft)
//		{
//			//dissable right while going left
//			isAnimate=0;
//			if(storeOrigPos)
//			{
//				goal = angle1+90.0;
//				if(goal>360.0){goal = goal-360.0; stillturn=1;}
//				turnGoal = goal;
//				//cout << "turning" << goal << endl;
//				storeOrigPos = 0;
//			}
//			else goal = turnGoal;
//			tempAngle = angle1;
//			if(angle1<goal||stillturn)
//			{	
//				tempAngle=tempAngle+10.0;
//				if (tempAngle > 360.0) tempAngle -= 360.0;
//				if (tempAngle < 0.0) tempAngle += 360.0;
//				angle1 = tempAngle;
//				//cout << goal << "   " << angle << endl;
//			}
//			else
//			{
//				animateLeft = 0;
//				storeOrigPos = 1;
//				isAnimate = 1;
//			}
//		
//		}
//
//		if (animateRight)
//		{
//			isAnimate=0;
//			if(storeOrigPos)
//			{
//				goal = angle1-90.0;
//				if(goal<0.0){goal = goal+360.0; stillturn=1;}
//				turnGoal = goal;
//				//cout << "turning" << goal << endl;
//				storeOrigPos = 0;
//
//			}
//			else goal = turnGoal;
//			tempAngle = angle1;
//			if(angle1>goal||stillturn)
//			{	
//				tempAngle=tempAngle-10.0;
//				if (tempAngle > 360.0) tempAngle -= 360.0;
//				if (tempAngle < 0.0) tempAngle += 360.0;
//				angle1 = tempAngle;
//				//cout << goal << "   " << angle << endl;
//			}
//			else
//			{
//				animateRight = 0;
//				storeOrigPos = 1;
//				isAnimate = 1;
//			}
//		
//		}
	}
	glutTimerFunc(animationPeriod, animate, 1);
	glutPostRedisplay();
}

// Initialization routine.
void setup(void) 
{
	player1 = new Player(30, 30, 0);
	player2 = new Player(80, 80, 1);

	glClearColor(1.0, 1.0, 1.0, 0.0);  
	cat = glGenLists(1);
	glNewList(cat, GL_COMPILE);

	glPushMatrix();
	player1->drawCat();
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
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	cout << key << " pressed." << endl;
	switch(key) {
	case 27:
		exit(0);
		break;         
	case 'a':
		animateLeft = 1;
		player2->turn(false);
		break;
	case 'd':
		//animateRight = 1;
		p2turn = 1;
		player2->turn(true);
	}

}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	Point player1Pt = player1->positions[player1->positions.size() - 1];

	//need left right for player 1, a and d for player 2?
	float tempxVal = player1Pt.x, tempzVal = player1Pt.z, tempAngle = player1->direction*90;

	p1turn = 0;
	p2turn = 0;
	// Compute next position.
	if (key == GLUT_KEY_LEFT){  
		animateLeft = 1;
		player1->turn(false);
	}
	if (key == GLUT_KEY_RIGHT){
 		animateRight = 1;
		player1->turn(true);
	}

	// Move bike to next position only if there will not be collision
	if (!CatCollision(tempxVal, tempzVal, tempAngle) )
	{
		isCollision = 0;
	}
	else
	{
		isCollision = 1;
	}
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

