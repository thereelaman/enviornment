#include <stdlib.h>
#include <iostream> //for testing
#include <glut.h>

void renderScene();
void reshapeScene(int,int);

// this is where we process the Special Keyboard Button Presses eg: F1, F2, F3, Up, Down, Home etc.
void pressKey(int, int, int);
void releaseKey(int, int, int);

void processNormalKeys(unsigned char, int, int);

void pressMouseButton(int, int, int, int);
void processMouseMovement(int, int);

void computePos(float);

void drawSnowman();

int main(int argc, char** argv) {

	// init the GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// init the GLUT Window
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(1366, 728);
	glutCreateWindow("The Scene");

	
	glutDisplayFunc(renderScene);	// register the callback to the render start point
	glutReshapeFunc(reshapeScene);	// register the callback to the function used to change the perspective so that window resizing doesn't cause any issues
	glutIdleFunc(renderScene);		// register the callback to the function which is executed whenever a frame is idle

	glutIgnoreKeyRepeat(1); // so that repetetive key presses are ignored

	glutKeyboardFunc(processNormalKeys);	// register the callback to the Normal Keyboard Buttons
	
	glutSpecialFunc(pressKey);	// register the callback to the Special Keyboard Buttons
	glutSpecialUpFunc(releaseKey);

	glutMouseFunc(pressMouseButton);		// register the callback to mouse buttons
	glutMotionFunc(processMouseMovement);	// register the callback to mouse movement

	glEnable(GL_DEPTH_TEST);	//  does depth comparisons and updates the depth buffer

	// enter teh GLUT event processing cycle
	glutMainLoop();

	return 1;

}

float angle = 0.0;				// angle of rotation for the camera direction (on y axis/ xz plane)
float lx = 0.0f, lz = -1.0f;	// line of sight on xz plane
float x = 0.0f, z = 5.0f;		// xz position of the camera

float movementSpeed = 0.5f;		// defines the units at which the camera will move on a key press
float rotationSpeed = 2.0f;		// defines the units at which the camera will rotate on a key press

// these are the key states, they are the values to be used on 
float deltaAngle = 0.0f;
float deltaMove = 0.0f;

int xPos = -1; // variable to store the X position when the mouse is clicked

// this is where we start rendering our scene
void renderScene(void) {

	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// reset transformations
	glLoadIdentity();

	if (deltaMove)
		computePos(deltaMove);

	// set the camera
	gluLookAt( x, 1.0f, z,			// location of the camera
			  GLdouble(x) + GLdouble(lx), 1.0f, GLdouble(z) + GLdouble(lz), // relative look at point = line of sight (i.e. the real position/direction we want to look at) + camera position
			  0.0f, 1.0f, 0.0f);	// the up vector of the camera

	// Draw ground
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	// Draw 36 SnowMen
	for (int i = -3; i < 3; i++)
		for (int j = -3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i * 10.0, 0, j * 10.0);
			drawSnowman();
			glPopMatrix();
		}

	glutSwapBuffers();

}

// this is where we reshape the scene/ change the perspective
void reshapeScene(int w, int h) { // w, h represents width and height

	// we can't make a window of zero height, so we set the height to be 1
	// because when h=0, there can be a divide by zero error when calculating the ratio of the	
	if (h == 0)
		h = 1;

	float ratio = 1.0 * w / h;
	
	glMatrixMode(GL_PROJECTION); // use the Projection Matrix
	glLoadIdentity(); // reset transformations
	glViewport(0, 0, w, h); // set the viewport to be the entire window

	// Set the correct perspective.
	gluPerspective(90, ratio, 1, 1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

}

void pressKey(int key, int x, int y){

	switch (key) {
		case GLUT_KEY_LEFT	:	break;
		case GLUT_KEY_RIGHT	:	break;
		case GLUT_KEY_UP	:	deltaMove = 0.5f;		break;
		case GLUT_KEY_DOWN	:	deltaMove = -0.5f;		break;
	}

}

void releaseKey(int key, int x, int y){

	switch (key) {
		case GLUT_KEY_LEFT	:	
		case GLUT_KEY_RIGHT	:	break;
		case GLUT_KEY_UP	:	
		case GLUT_KEY_DOWN	:	deltaMove  = 0.0f;		break;
	}

}

// this is where we process the ASCII Keyboard Button Presses, eg: Escape, W, A, S, D, etc.
void processNormalKeys(unsigned char key, int x, int y) {

	int mod;
	switch (key) {

		case 27: exit(0); break;// 27 represents the Escape Key
		
	}
}

// this is where we process the mouse button changes
void pressMouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {
	
		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xPos = -1;
		}
		else {// state = GLUT_DOWN
			xPos = x;
		}
	}

}

void processMouseMovement(int x, int y) {

	// this will only be true when the left button is down
	//if (xPos >= 0) {

		// update deltaAngle
		deltaAngle = (x - xPos) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	//}

}

void computePos(float delta) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;

}

void drawSnowman() {

	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw Body
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	// Draw Nose
	glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidCone(0.08f, 0.5f, 10, 2);

}