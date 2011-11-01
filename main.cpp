//////////////////////////////////////////////////////////////////////////////////////////////         
// tron.cpp
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

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals.
static float player1color[3] = {1.0, 0.0, 0.0}; 
static float player2color[3] = {0.0, 0.0, 1.0};
static float partSelectColor[3] = {1.0, 0.0, 0.0}; // Selection indicate color.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static ofstream outFile; // File to write configurations data.
static float xVal1 = 0, zVal1 = 0; // Co-ordinates of the bike1
static float xVal2 = 0, zVal2 = 0; // Co-ordinates of the bike2
static float angle = 0.0; // Angle of the bike
static unsigned int bike; // Display lists base index.
static int arenaheight = 20;
static int isCollision = 0;
static int arenawidth = 100;
static int arenalength = 100;

static int isAnimate = 1; // Animated?   //change back to 0 once have the if commands
static int animateRight = 0; //animation for turning right
static int animateLeft = 0; //animation for turning left
static int animationPeriod = 10; // Time interval between frames.
static int turnGoal = 0;
static int storeOrigPos = 1;

static int width = 1000, height = 700;

// Camera class.
class Camera
{
public:
	Camera();
	void incrementViewDirection();
	void decrementViewDirection();
	void incrementZoomDistance() { zoomDistance += 1.0; }
	void decrementZoomDistance() { zoomDistance -= 1.0; }

	float getViewDirection() const { return viewDirection; }
	float getZoomDistance() const { return zoomDistance; }

private:
	float viewDirection;
	float zoomDistance;
};

// Global camera.
Camera onecam;
Camera twocam;

// Camera constructor.
Camera::Camera()
{
	viewDirection = 0.0;
	zoomDistance = 30.0;
}

// Player class
class Player
{
public:
	Player();
	void draw();
	void drawCamera();

private:

};

// record path
vector<Player> player1; //dont actually need vector, just keep 
// translating or rotating bike and do exact same thing to following camera
vector<Player> player2;

// Global iterators to traverse 
vector<Player>::iterator path1; //vector of paths, x1, z1, x2, z2 .....
vector<Player>::iterator path2;

// Man constructor.
Player::Player()
{

}

void Player::drawCamera()
{

}

// Function to draw man.
void Player::draw()
{
 
}

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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int BikeCollision(float x, float z, float a)
{
	int i,j;

	// Check for collision with all walls

	return 0;
}

// Drawing routine.
void drawScene(void)
{
	// -- player 1 ----------------------------------------------------
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// Local iterator to traverse player1.
	vector<Player>::iterator P1Itr;
	glViewport(0, height/2.0, width, height/2.0);

	//write player 1
	glPushMatrix();
	glTranslatef(0.0, 0.0, -30.0);
	glColor3fv(player1color);
	glRasterPos3f(-28.0, 25.0, 0.0);
	writeBitmapString((void*)font, (char*)"PLAYER ONE"); 
	glPopMatrix();


	// Locate the camera at the tip of the opject and pointing in the direction of the object
	//want behind object!!!!
	//cout << angle << endl;
	gluLookAt(xVal1 , 
		0.0, 
		zVal1, 
		xVal1- 12 * sin( (PI/180.0) * angle),
		0.0,
		zVal1- 12 * cos( (PI/180.0) * angle), 
		0.0, 
		1.0, 
		0.0);

	//draw walls and floor
	glPushMatrix();
	drawWallFloors();
	glPopMatrix();

	//draw bike
	glPushMatrix();
	glTranslatef(xVal1 - 12 * sin( (PI/180.0) * angle), -10.0, zVal1- 12 * cos( (PI/180.0) * angle));
	glRotatef(angle, 0.0, 1.0, 0.0);
	glColor3fv(player1color);
	glCallList(bike);
	glPopMatrix();
	
	//-- repeat for player 2 ---------------------------------------------
	glLoadIdentity();
	glViewport(0, 0, width, height/2.0);
	vector<Player>::iterator P2Itr;

	//write player 2
	glPushMatrix();
	glTranslatef(0.0, 0.0, -30.0);
	glColor3fv(player1color);
	glRasterPos3f(-28.0, 25.0, 0.0);
	writeBitmapString((void*)font, (char*)"PLAYER TWO"); 

	// Place camera.
	gluLookAt(twocam.getZoomDistance() * sin(twocam.getViewDirection()*PI/180.0), 0.0, 
		twocam.getZoomDistance() * cos(twocam.getViewDirection()*PI/180.0), 0.0, 
		0.0, 0.0, 0.0, 1.0, 0.0); 

	//draw walls and floor
	drawWallFloors();

	glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
	float goal, tempAngle, stillturn=0;
	if (isAnimate) 
	{
		xVal1 = xVal1 - sin(angle * PI/180.0); 
		zVal1 = zVal1 - cos(angle * PI/180.0);

		xVal2 = xVal2 - sin(angle * PI/180.0); 
		zVal2 = zVal2 - cos(angle * PI/180.0);
	}
	if (animateLeft)
	{
		//dissable right while going left
		isAnimate=0;
		if(storeOrigPos)
		{
			goal = angle+90.0;
			if(goal>360.0){goal = goal-360.0; stillturn=1;}
			turnGoal = goal;
			//cout << "turning" << goal << endl;
			storeOrigPos = 0;
		}
		else goal = turnGoal;
		tempAngle = angle;
		if(angle<goal||stillturn)
		{	
			tempAngle=tempAngle+10.0;
			if (tempAngle > 360.0) tempAngle -= 360.0;
			if (tempAngle < 0.0) tempAngle += 360.0;
			angle = tempAngle;
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
			goal = angle-90.0;
			if(goal<0.0){goal = goal+360.0; stillturn=1;}
			turnGoal = goal;
			//cout << "turning" << goal << endl;
			storeOrigPos = 0;
		}
		else goal = turnGoal;
		tempAngle = angle;
		if(angle>goal||stillturn)
		{	
			tempAngle=tempAngle-10.0;
			if (tempAngle > 360.0) tempAngle -= 360.0;
			if (tempAngle < 0.0) tempAngle += 360.0;
			angle = tempAngle;
			//cout << goal << "   " << angle << endl;
		}
		else
		{
			animateRight = 0;
			storeOrigPos = 1;
			isAnimate = 1;
		}
		
	}

	glutTimerFunc(animationPeriod, animate, 1);
	glutPostRedisplay();
}

// Initialization routine.
void setup(void) 
{
	glClearColor(1.0, 1.0, 1.0, 0.0);  
	bike = glGenLists(1);
	glNewList(bike, GL_COMPILE);
	glPushMatrix();
	glRotatef(180.0, 0.0, 1.0, 0.0); // make bike point down the z-axis initially
	//glColor3fv (player1color);  //if player1, one color, if player2 different color
	//if player 1, this color, if player 2....
	glScalef(0.5, 1.0, 1.0);
	//glutSolidCube(1.0);
	glBegin(GL_QUADS);
	glVertex3f(-1,-1,-1);
	glVertex3f( 1,-1,-1);
	glVertex3f( 1, 1,-1);
	glColor3f (0.0, 0.0, 0.0);
	glVertex3f(-1, 1,-1);
	glColor3fv (player1color);
	 
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,-1, 1);
	glVertex3f(-1, 1, 1);
	glColor3f (0.0, 0.0, 0.0);
	glVertex3f(-1, 1,-1);
	glColor3fv (player1color);
	 
	glVertex3f( 1,-1, 1);
	glVertex3f( 1,-1,-1);
	glVertex3f( 1, 1,-1);
	glColor3f (1.0, 0.5, 0.5);
	glVertex3f( 1, 1, 1);
	glColor3fv (player1color);
	 
	glVertex3f(-1,-1,-1);
	glVertex3f( 1,-1,-1);
	glVertex3f( 1,-1, 1);
	glColor3f (0.5, 0.0, 0.0);
	glVertex3f(-1,-1, 1);
	glColor3fv (player1color);
	 
	glVertex3f(-1,-1, 1);
	glVertex3f( 1,-1, 1);
	glVertex3f( 1, 1, 1);
	glColor3f (1.0, 0.5, 0.5);
	glVertex3f(-1, 1, 1);
	glColor3fv (player1color);
	glEnd();

	//glutSolidSphere(1.0, 10.0, 10.0);

	glPopMatrix();

	glEndList();

	// starting positions for bikes
	// Initialize global iterators for manVector.

	// Initialize camera.
	onecam = Camera();
	twocam = Camera();
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
	vector<Player>::iterator localManVectorIterator;
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
	float tempxVal = xVal1, tempzVal = zVal1, tempAngle = angle, newAngle;

	// Compute next position.
	if (key == GLUT_KEY_LEFT)  animateLeft = 1;
	if (key == GLUT_KEY_RIGHT) animateRight = 1;

	if( key == 'a')
	{
		tempxVal = xVal1 - sin(angle * PI/180.0); 
		tempzVal = zVal1 - cos(angle * PI/180.0);
	}
	if( key == 'd')
	{
		tempxVal = xVal1 + sin(angle * PI/180.0); 
		tempzVal = zVal1 + cos(angle * PI/180.0);
	}

	// Angle correction.
	if (tempAngle > 360.0) tempAngle -= 360.0;
	if (tempAngle < 0.0) tempAngle += 360.0;

	// Move bike to next position only if there will not be collision

	if (!BikeCollision(tempxVal, tempzVal, tempAngle) )
	{
		isCollision = 0;
		xVal1 = tempxVal;
		zVal1 = tempzVal;
		angle = tempAngle;
	}
	else isCollision = 1;

	//glutPostRedisplay();
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
	glutCreateWindow ("tron.cpp"); 
	setup(); 
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutTimerFunc(5, animate, 1);
	glutMainLoop(); 

	return 0;  
}

