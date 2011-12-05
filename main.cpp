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
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Globals.
static bool paused = false;

static float player1color[3] = {1.0, 0.0, 0.0}; 
static float player2color[3] = {0.0, 0.0, 1.0};

Player *player1;
Player *player2;

//	Player texture information
static const string NYAN_TEXTURES[6] = { "frame00.png", "frame01.png", "frame02.png", "frame03.png", "frame04.png", "frame05.png" };
static int nyan_tex_width = 400, nyan_tex_height = 280;
static GLuint textureFrames[6];

//	Trail texture information
static const string RAINBOW_TRAIL_TEXTURE = "rainbow.png";
static int rainbow_tex_width = 398, rainbow_tex_height = 107;
static GLuint rainbowTexture;

//	the width and length of the playing area
static const int ARENA_WIDTH = 1000;
static const int ARENA_LENGTH = 1000;

//	the height of the walls
static const int ARENA_HEIGHT = 200;

//	Camera field of view values
static const GLdouble CAM_RIGHT = 50;
static const GLdouble CAM_TOP = 50;
static const GLdouble CAM_BACK = 300;
static const GLdouble CAM_FWD = 1000;

//	Viewport origin differences from each player's position
static const unsigned int CAM_BACK_DIST = 3;
static const unsigned int CAM_DIAG_LEFT_DIST = 2;
static const unsigned int CAM_DIAG_UP_DIST = 10;

static int isCollision = 0;

//	Time interval between frames.
static int animationPeriod = 10;

//	The size of the GLUT window.
static int width = 1000, height = 700;

//	Font used for writing text.
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
	for(float z=-ARENA_LENGTH; z<ARENA_LENGTH; z+=2.0)
	{
		glBegin(GL_QUADS);
		for(float x=-ARENA_WIDTH; x<ARENA_WIDTH; x+=2.0)
		{
			glVertex3f(x, 0, z);                               
			glVertex3f(x+2, 0, z);                               
			glVertex3f(x+2, 0, z+2);                               
			glVertex3f(x, 0, z+2);                               
		}
		glEnd();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPopMatrix();
}

void drawFrontWalls () {
	glPushMatrix();

	//	Enable transparency for these walls
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
        /*const int size = 1 * 1 * 4;
        GLubyte pixels[size];

        //looking at pixel, not infront
        glReadPixels(0, 0, x, z, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        for(int index = 0; index < size; index+=4)
        {
            cout << "red " << (unsigned)pixels[index+0] << endl;
            cout << "green " << (unsigned)pixels[index+1] << endl;
            cout << "blue " << (unsigned)pixels[index+2] << endl;
        }
        pixels[0] = '\0';*/
    }
	return 0;
}

// Drawing routine.
void drawScene(void)
{
        GLfloat pixel[3];
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

	glViewport(0, height/2.0, width, height/2.0);
	glLoadIdentity();
	//glOrtho(-CAM_RIGHT, CAM_RIGHT, -CAM_TOP, CAM_TOP, -CAM_BACK, CAM_FWD);

        float temp1 = player1Pt.z*30;
        float temp2 = player1Pt.x*30;

	//  Locate the camera behind, and off to the side, of cat initially.
	//  Don't rotate the camera.
	gluLookAt(player1Pt.x - CAM_DIAG_LEFT_DIST, 
		CAM_DIAG_UP_DIST, 
		player1Pt.z + CAM_BACK_DIST,
		player1Pt.x,
		CAM_DIAG_UP_DIST - 8,
		player1Pt.z, 
		0.0, 
		1.0, 
		0.0);

	//	draw walls and floor
	drawBackWallsAndFloors();

	//	draw players' cats and trails
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

        //SET TO 2D
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluOrtho2D(-width/2, 0, 0, height/2);
        glDisable(GL_DEPTH_TEST);

        //MINI MAP
        glColor3f(1.0, 1.0, 1.0);
        //semi transparent menu would be AWESOME
        //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

        //area for map
        glRectf(-width*12.4, -height*11.4, -width*12.4 + ARENA_WIDTH*62, -height*11.5 +ARENA_LENGTH*62);

        //lines for player
        glBegin(GL_LINES);
        glColor3fv(player1color);
        for(int i=0; i < player1Pts->size() - 1; ++i) {
                player1Pt = (*player1Pts)[i];
                player1Pt2 = (*player1Pts)[i+1];

                //int k = CatCollision(player1Pt2.z*30-width*9.4, player1Pt2.x*30-height*7.0,player1->direction,2);
                glVertex3f(player1Pt2.z*30-width*9.4, player1Pt2.x*30-height*7.0, 0);
                glVertex3f(player1Pt.z*30-width*9.4, player1Pt.x*30-height*7.0, 0);
                //cout << k << endl;
        }
        glEnd();

        //2D position
        int tempx, tempy;
        tempx = -325;  //works at 30 to -920 which is the width
        tempy = 370;  //works from 50-535 which is the height of the mini map
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        glVertex3f(temp1-width*9.4, temp2 -height*7.0, 0.0);
        //glVertex3f(-width*6.45, -height*12.2, 0.0);  // here when read (width/4, height/2)
        //glVertex3f((-width)*6.45, (-height+100)*12.2, 0.0);// here when read (width/4, height/2+100/4)
        glVertex3f((-width+tempx)*6.43, (-height+tempy)*12.1, 0.0);// here when read (width/4+tempx/4, height/2+tempy/4)
        glEnd();

        //TEST
        glColor3f(1.0, 0.0, 0.0);
        //glRectf(-width*4.7, -height*5.4, -width*4.6, -height*5.0); //READING PIXEL HERE ...

        //looking at pixel look for collision, not infront
        glFlush();
        glReadPixels(width/4+tempx/4, height/2+tempy/4, 1, 1, GL_RGB , GL_FLOAT , pixel);
        int k = glGetError();
        if(glGetError() != GL_NO_ERROR)
            printf("opengl error: ");
        cout << glGetError() << endl;
        //cout<< sizeof(GLfloat) << "  " << sizeof(float) << endl;
        printf("red %f\n", pixel[0]);
        printf("green %f\n", pixel[2]);
        printf("blue %f\n", pixel[3]);
        memset(pixel, 0, 3*sizeof(GLfloat));

	glPopMatrix();


	//-- repeat for player 2 ---------------------------------------------
	Point player2Pt = player2->positions[player2->positions.size() - 1];
	vector<Point> *player2Pts;
	player2Pts = &(player2->positions);

        //Drawing 3D
	glViewport(0, 0, width, height/2.0);
	glLoadIdentity();

	temp1 = player2Pt.z*30;
	temp2 = player2Pt.x*30;

	//  Locate the camera behind, and off to the side, of cat initially.
	//  Don't rotate the camera.
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

	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers();
}

// Timer function.
void update(int value)
{
	float goal, tempAngle, stillturn=0;
	Point player1Pt = player1->positions[player1->positions.size() - 1];

	//	Do nothing if the game is paused
	if (!paused) {
		player1->tick();
		player2->tick();

                isCollision = CatCollision(player1Pt.x, player1Pt.z, player1->direction * 90, 3);
		if(!isCollision){
		}
	}
	glutTimerFunc(animationPeriod, update, 1);
	glutPostRedisplay();
}

// Initialization routine.
void init(void) 
{
	//	Load the nyan cat frames!
	for (unsigned int i = 0; i < 6; ++i) {
		textureFrames[i] = loadTexture(NYAN_TEXTURES[i], nyan_tex_width, nyan_tex_height);

		//	Handle errors
		if (textureFrames[i] == 0) {
			// TODO soon
			cout << "Texture " << i << " failed to load." << endl;
		}
	}

	rainbowTexture = loadTexture(RAINBOW_TRAIL_TEXTURE, rainbow_tex_width, rainbow_tex_height);

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
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100); 
	glutCreateWindow ("steve"); 

	init(); 

	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutTimerFunc(5, update, 1);
	glutMainLoop(); 

	return 0;  
}

