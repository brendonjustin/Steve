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

#include "png_loader.h"
#include "Player.h"

#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

using namespace std;

// Globals.
static bool paused = false;

static float player1color[3] = {1.0, 0.0, 0.0};
static float player2color[3] = {0.0, 0.0, 1.0};

Player *player1;
Player *player2;

// Player texture information
static const string NYAN_TEXTURES[6] = { "frame00.png", "frame01.png", "frame02.png", "frame03.png", "frame04.png", "frame05.png" };
static int nyan_tex_width = 400, nyan_tex_height = 280;
static GLuint textureFrames[6];

// Trail texture information
static const string RAINBOW_TRAIL_TEXTURE = "rainbow.png";
static int rainbow_tex_width = 398, rainbow_tex_height = 107;
static GLuint rainbowTexture;

// the width and length of the playing area
static const int ARENA_WIDTH = 500;
static const int ARENA_LENGTH = 500;

// the height of the walls
static const int ARENA_HEIGHT = 200;

static const int GRID_SPACING = 8;

static GLuint minimap_list_index;
static GLubyte minimap_list;

// the width and length of the map
static const int MINIMAP_WIDTH = 250;
static const int MINIMAP_HEIGHT = 250;

static const float MINIMAP_SCALE_X = 0.05;
static const float MINIMAP_SCALE_Z = 0.05;

static const int MINIMAP_OFFSET_X = 700;
static const int MINIMAP_OFFSET_Z = 700;

// Camera field of view values
static const GLdouble CAM_RIGHT = 50;
static const GLdouble CAM_TOP = 50;
static const GLdouble CAM_BACK = 300;
static const GLdouble CAM_FWD = 1000;

// Viewport origin differences from each player's position
static const unsigned int CAM_BACK_DIST = 3;
static const unsigned int CAM_DIAG_LEFT_DIST = 2;
static const unsigned int CAM_DIAG_UP_DIST = 10;

static int isCollision = 0;

// Time interval between frames.
static int updatePeriod = 20;

// The size of the GLUT window.
static int window_width = 1000, window_height = 700;

// Font used for writing text.
static const long font = (long)GLUT_BITMAP_8_BY_13;

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void drawBackWallsAndFloors ()
{
	glPushMatrix();

	// Draw walls of arena
	glColor3f(0.0, 0.0, 0.8);
	glBegin(GL_QUADS);
	glVertex3f(ARENA_WIDTH, ARENA_HEIGHT, -ARENA_LENGTH);
	glVertex3f(-ARENA_WIDTH, ARENA_HEIGHT, -ARENA_LENGTH);
	glVertex3f(-ARENA_WIDTH, 0, -ARENA_LENGTH);
	glColor3f(0.0, 0.0, 0.4);
	glVertex3f(ARENA_WIDTH, 0, -ARENA_LENGTH);
	glColor3f(0.0, 0.0, 0.8);

	glVertex3f(ARENA_WIDTH, ARENA_HEIGHT, -ARENA_LENGTH);
	glVertex3f(ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);
	glVertex3f(ARENA_WIDTH, 0, ARENA_LENGTH);
	glColor3f(0.0, 0.0, 0.4);
	glVertex3f(ARENA_WIDTH, 0, -ARENA_LENGTH);

	glEnd();

	// tron lit floor
	glColor3f(0.3, 0.3, 0.8);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for(float z =- ARENA_LENGTH; z < ARENA_LENGTH; z += GRID_SPACING)
	{
		glBegin(GL_QUADS);
		for(float x =- ARENA_WIDTH; x < ARENA_WIDTH; x += GRID_SPACING)
		{
			glVertex3f(x, 0, z);
			glVertex3f(x + GRID_SPACING, 0, z);
			glVertex3f(x + GRID_SPACING, 0, z + GRID_SPACING);
			glVertex3f(x, 0, z + GRID_SPACING);
		}
		glEnd();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPopMatrix();
}

void drawFrontWalls () {
	glPushMatrix();

	// Enable transparency for these walls
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw walls of arena
	glColor4f(0.0, 0.0, 0.8, 0.4);
	glBegin(GL_QUADS);
	glVertex3f(-ARENA_WIDTH, ARENA_HEIGHT, -ARENA_LENGTH);
	glVertex3f(-ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);
	glVertex3f(-ARENA_WIDTH, 0, ARENA_LENGTH);
	glColor4f(0.0, 0.0, 0.4, 0.4);
	glVertex3f(-ARENA_WIDTH, 0, -ARENA_LENGTH);
	glColor4f(0.0, 0.0, 0.8, 0.4);

	glVertex3f(ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);
	glVertex3f(-ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);
	glVertex3f(-ARENA_WIDTH, 0, ARENA_LENGTH);
	glColor4f(0.0, 0.0, 0.4, 0.4);
	glVertex3f(ARENA_WIDTH, 0, ARENA_LENGTH);

	glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();
}

int CatCollision(float x, float z, float a, int d)
{
	if( d == 3){ //3D
		// Check for collision with outer walls
		if(x < (-ARENA_WIDTH+1.0) || x > (ARENA_WIDTH-1.0)){
			return 1;
		}

		if(z < (-ARENA_LENGTH+1.0) || z > (ARENA_LENGTH-1.0)){
			return 1;
		}
	}
	//check player walls
	if( d == 2) { //2D
		////TEST
		//glColor3f(1.0, 0.0, 0.0);
		////glRectf(-window_width*4.7, -window_height*5.4, -window_width*4.6, -window_height*5.0); //READING PIXEL HERE ...

		////looking at pixel look for collision, not infront
		//glReadPixels(window_width/4+tempx/4, window_height/2+tempy/4, 1, 1, GL_RGB , GL_FLOAT , pixel);
		//int k = glGetError();
		//if(glGetError() != GL_NO_ERROR)
		// printf("opengl error: ");
		//cout << glGetError() << endl;
		////cout<< sizeof(GLfloat) << " " << sizeof(float) << endl;
		//printf("red %f\n", pixel[0]);
		//printf("green %f\n", pixel[2]);
		//printf("blue %f\n", pixel[3]);
		//memset(pixel, 0, 3*sizeof(GLfloat));
	}
	return 0;
}

// Drawing routine.
void drawScene(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);

	glClear(GL_DEPTH_BUFFER_BIT);

	// -- player 1 ----------------------------------------------------
	Point player1Pt = player1->positions[player1->positions.size() - 1];
	Point player1Pt2;
	vector<Point> *player1Pts;
	player1Pts = &(player1->positions);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glViewport(0, window_height/2.0, window_width, window_height/2.0);
	glLoadIdentity();
	//glOrtho(-CAM_RIGHT, CAM_RIGHT, -CAM_TOP, CAM_TOP, -CAM_BACK, CAM_FWD);

	// Locate the camera behind, and off to the side, of cat initially.
	// Don't rotate the camera.
	gluLookAt(player1Pt.x - CAM_DIAG_LEFT_DIST,
		CAM_DIAG_UP_DIST,
		player1Pt.z + CAM_BACK_DIST,
		player1Pt.x,
		CAM_DIAG_UP_DIST - 8,
		player1Pt.z,
		0.0,
		1.0,
		0.0);

	// draw walls and floor
	drawBackWallsAndFloors();

	// draw players' cats and trails
	player1->draw();
	player2->draw();

	drawFrontWalls();

	if(isCollision){
		glPushMatrix();
		glTranslatef(0.0, 0.0, -50);
		glColor3fv(player1color);
		glRasterPos3f(-28.0, 25.0, 0.0);
		writeBitmapString((void*)font, (char*)"CRAHSED!!!!!");
		glPopMatrix();
	}

	glPopMatrix();


	//-- repeat for player 2 ---------------------------------------------
	Point player2Pt = player2->positions[player2->positions.size() - 1];
	vector<Point> *player2Pts;
	player2Pts = &(player2->positions);

	glPushMatrix();

	//Drawing 3D
	glViewport(0, 0, window_width, window_height/2.0);
	glLoadIdentity();

	// Locate the camera behind, and off to the side, of cat initially.
	// Don't rotate the camera.
	gluLookAt(player2Pt.x - CAM_DIAG_LEFT_DIST,
		CAM_DIAG_UP_DIST,
		player2Pt.z + CAM_BACK_DIST,
		player2Pt.x,
		CAM_DIAG_UP_DIST - 8,
		player2Pt.z,
		0.0,
		1.0,
		0.0);

	drawBackWallsAndFloors();

	// draw players' cats and trails
	player1->draw();
	player2->draw();

	drawFrontWalls();

	if(isCollision){
		glPushMatrix();
		glTranslatef(0.0, 0.0, -60);
		glColor3fv(player1color);
		glRasterPos3f(-28.0, 25.0, 0.0);
		writeBitmapString((void*)font, (char*)"You Win!!!!!");
		glPopMatrix();
	}

	glPopMatrix();
	glDisable(GL_DEPTH_TEST);

	// Draw the minimap
	glCallList(minimap_list_index);

	glutSwapBuffers();
}

// Timer function.
void update(int value)
{
	float goal, tempAngle, stillturn=0;
	GLfloat pixel[3];

	// Only update if the game is not paused.
	if (!paused) {
		player1->tick();
		player2->tick();

		// Get some of the players' positions for use with the minimap
		Point player1Pt = player1->positions[player1->positions.size() - 1];
		Point player1Pt2;
		vector<Point> *player1Pts;
		player1Pts = &(player1->positions);

		Point player2Pt = player2->positions[player2->positions.size() - 1];
		Point player2Pt2;
		vector<Point> *player2Pts;
		player2Pts = &(player2->positions);

		static float magic_constant = 3.0;

		//	Check the player's position on the minimap.  If there is already something there,
		//	then they are colliding with it.
		// Bottom left corner of the map:
		//glReadPixels(window_width * 0.5 - MINIMAP_WIDTH / 4, window_height * 0.5 - MINIMAP_HEIGHT / 4, 1, 1, GL_RGB , GL_FLOAT , pixel);
		glReadPixels(window_width * 0.5 + player1Pt.z * MINIMAP_SCALE_Z * magic_constant, window_height * 0.5 + player1Pt.x * MINIMAP_SCALE_X * magic_constant, 1, 1, GL_RGB , GL_FLOAT , pixel);

		int k = glGetError();
		if(glGetError() != GL_NO_ERROR)
		{
			printf("opengl error: ");
			//cout << glGetError() << endl;
		}

		//cout << " R: " << pixel[0] << " G: " << pixel[1] << " B: " << pixel[2] << " x: " << player1Pt.x << " z: " << player1Pt.z << endl;

		//if( pixel[0]>0 && pixel[1]<0.5 && pixel[2]<0.5)
		//{
		//	//cout << "red only" << endl;
		//}
		//if( pixel[2]>0 && pixel[1]<0.5 && pixel[0]<0.5)
		//{
		//	//cout << "blue only" << endl;
		//}

		if ((pixel[0] > 0 && pixel[1] < 0.5 && pixel[2] < 0.5) || (pixel[2] > 0 && pixel[1] < 0.5 && pixel[0] < 0.5))
		{
			player1->collided = true;
		}
		memset(pixel, 0, 3*sizeof(GLfloat));
		//if(!isCollision){ }

		glReadPixels(window_width * 0.5 + player2Pt.z * MINIMAP_SCALE_Z * magic_constant, window_height * 0.5 + player2Pt.x * MINIMAP_SCALE_X * magic_constant, 1, 1, GL_RGB , GL_FLOAT , pixel);

		k = glGetError();
		if(glGetError() != GL_NO_ERROR)
		{
			printf("opengl error: ");
			//cout << glGetError() << endl;
		}

		if ((pixel[0] > 0 && pixel[1] < 0.5 && pixel[2] < 0.5) || (pixel[2] > 0 && pixel[1] < 0.5 && pixel[0] < 0.5))
		{
			player2->collided = true;
		}
		memset(pixel, 0, 3*sizeof(GLfloat));

		// Make sure the minimap display list is empty, then make it again.
		glDeleteLists(minimap_list_index, 1);
		glNewList(minimap_list_index, GL_COMPILE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//SET TO 2D
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glViewport(window_width / 2 - MINIMAP_WIDTH * 0.5, window_height / 2 - MINIMAP_HEIGHT * 0.5, MINIMAP_WIDTH, MINIMAP_HEIGHT);

		//MINI MAP
		glColor4f(0.0, 0.0, 0.8, 0.5);

		//area for minimap
		glRectf(-MINIMAP_WIDTH / 8, -MINIMAP_HEIGHT / 8, MINIMAP_WIDTH / 8, MINIMAP_HEIGHT / 8);

		// Note: Drawing current position is disabled
		//2D position of each player
		glPointSize(5.0f);

		// Player 1
		//glColor3fv(player1color);
		//glBegin(GL_POINTS);
		//glVertex3f(player1Pt.z * MINIMAP_SCALE_Z, player1Pt.x * MINIMAP_SCALE_X, 0.0);
		//glEnd();

		// Player 2
		//glColor3fv(player2color);
		//glBegin(GL_POINTS);
		//glVertex3f(player2Pt.z * MINIMAP_SCALE_Z, player2Pt.x * MINIMAP_SCALE_X, 0.0);
		//glEnd();

		//lines for player 1
		glLineWidth(2.0f);
		glColor3fv(player1color);
		glBegin(GL_LINES);
		for(int i=0; i < player1Pts->size() - 1; ++i) {
			player1Pt = (*player1Pts)[i];
			player1Pt2 = (*player1Pts)[i+1];

			glVertex3f(player1Pt2.z * MINIMAP_SCALE_Z, player1Pt2.x * MINIMAP_SCALE_X, 0);
			glVertex3f(player1Pt.z * MINIMAP_SCALE_Z, player1Pt.x * MINIMAP_SCALE_X, 0);
		}

		glEnd();

		//lines for player 2
		glLineWidth(2.0f);
		glColor3fv(player2color);
		glBegin(GL_LINES);
		for(int i=0; i < player2Pts->size() - 1; ++i) {
			player2Pt = (*player2Pts)[i];
			player2Pt2 = (*player2Pts)[i+1];

			glVertex3f(player2Pt2.z * MINIMAP_SCALE_Z, player2Pt2.x * MINIMAP_SCALE_X, 0);
			glVertex3f(player2Pt.z * MINIMAP_SCALE_Z, player2Pt.x * MINIMAP_SCALE_X, 0);
		}
		glEnd();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEndList();
		glFlush();

	}

	glutTimerFunc(updatePeriod, update, 1);
	glutPostRedisplay();
}

// Initialization routine.
void init(void)
{
	// Load the nyan cat frames!
	for (unsigned int i = 0; i < 6; ++i) {
		textureFrames[i] = loadTexture(NYAN_TEXTURES[i], nyan_tex_width, nyan_tex_height);

		// Handle errors
		if (textureFrames[i] == 0) {
			// TODO soon
			cout << "Texture " << i << " failed to load." << endl;
		}
	}

	rainbowTexture = loadTexture(RAINBOW_TRAIL_TEXTURE, rainbow_tex_width, rainbow_tex_height);

	minimap_list_index = glGenLists(1);
	minimap_list = 0;

	player1 = new Player(30, 30, 0, textureFrames, &rainbowTexture);
	player2 = new Player(80, 80, 1, textureFrames, &rainbowTexture);

	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-CAM_RIGHT, CAM_RIGHT, -CAM_TOP, CAM_TOP, -CAM_BACK, CAM_FWD);

	glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch(key) {
	case 27:
		// esc key
		exit(0);
		break;
	case 'a':
		player1->turn(false);
		break;
	case 'd':
		player1->turn(true);
		break;
	case 32:
		// space key
		paused = !paused;
	}
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
	// Compute next position.

	switch(key) {
	case GLUT_KEY_LEFT:
		player2->turn(false);
		break;
	case GLUT_KEY_RIGHT:
		player2->turn(true);
		break;
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
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow ("steve");

	init();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutTimerFunc(updatePeriod, update, 1);
	glutMainLoop();

	return 0;
}
