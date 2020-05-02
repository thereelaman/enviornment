#include <stdlib.h>
#include <iostream> //for testing
#include <glut.h>

void renderScene();
void reshapeScene(int,int);

// this is where we process the Special Keyboard Button Presses eg: F1, F2, F3, Up, Down, Home etc.
void pressSpecialKey(int, int, int);
void releaseSpecialKey(int, int, int);

void pressNormalKeys(unsigned char, int, int);
void releaseNormalKeys(unsigned char, int, int);

void processMouseMovement(int, int);

void computePos(float, float);

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

	glutKeyboardFunc(pressNormalKeys);	// register the callback to the Normal Keyboard Buttons
	glutKeyboardUpFunc(releaseNormalKeys);
	
	glutSpecialFunc(pressSpecialKey);	// register the callback to the Special Keyboard Buttons
	glutSpecialUpFunc(releaseSpecialKey);

	glutPassiveMotionFunc(processMouseMovement);	// register the callback to mouse movement

	glEnable(GL_DEPTH_TEST);	//  does depth comparisons and updates the depth buffer

	// enter teh GLUT event processing cycle
	glutMainLoop();

	return 1;

}

float lx = 0.0f,				// line of sight on xz plane
	  ly = 0.0f,
	  lz = -1.0f;
float x = 0.0f, y = 1.0f, z = 5.0f;		// xz position of the camera

float strafeX = 0.0f,
	  strafeZ = 0.0f;

float movementSpeed = 0.5f;		// defines the units at which the camera will move on a key press
float rotationSpeed = 2.0f;		// defines the units at which the camera will rotate on a key press

float angleXY = 0.0f;			// angle of rotation for the camera direction (on z axis/ xy plane)
float angleXZ = 0.0f;			// angle of rotation for the camera direction (on y axis/ xz plane)

// these are the key states, they are the values to be worked on 
float deltaAngleXY = 0.0f;
float deltaAngleXZ = 0.0f;

float deltaMoveZ = 0.0f;
float deltaMoveX = 0.0f;

// variable to store the X and Y positions when the mouse is clicked
int xPos = -1; 
int yPos = -1;

// this is where we start rendering our scene
void renderScene(void) {

	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// reset transformations
	glLoadIdentity();

	if (deltaMoveX || deltaMoveZ)
		computePos(deltaMoveX, deltaMoveZ);

	// set the camera
	gluLookAt( x, y, z,				// location of the camera
			  // relative look at point = line of sight (i.e. the real position/direction we want to look at) + camera position
			  GLdouble(x) + GLdouble(lx), GLdouble(y) + GLdouble(ly), GLdouble(z) + GLdouble(lz),
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

void pressSpecialKey(int key, int x, int y){

	switch (key) {
		case GLUT_KEY_LEFT	:	deltaMoveX -= 0.5f;		break;
		case GLUT_KEY_RIGHT	:	deltaMoveX += 0.5f;		break;
		case GLUT_KEY_UP	:	deltaMoveZ += 0.5f;		break;
		case GLUT_KEY_DOWN	:	deltaMoveZ -= 0.5f;		break;
	}

}

void releaseSpecialKey(int key, int x, int y){

	switch (key) {
		case GLUT_KEY_LEFT	:	deltaMoveX += 0.5f;		break;
		case GLUT_KEY_RIGHT	:	deltaMoveX -= 0.5f;		break;
		case GLUT_KEY_UP	:	deltaMoveZ -= 0.5f;		break;
		case GLUT_KEY_DOWN	:	deltaMoveZ += 0.5f;		break;
	}

}

// this is where we process the ASCII Keyboard Button Presses, eg: Escape, W, A, S, D, etc.
void pressNormalKeys(unsigned char key, int x, int y) {

	int mod;
	switch (key) {

		case 27: exit(0); break;// 27 represents the Escape Key
		case 'a': deltaMoveX -= 0.5f;	break;
		case 'd': deltaMoveX += 0.5f;	break;
		case 'w': deltaMoveZ += 0.5f;	break;
		case 's': deltaMoveZ -= 0.5f;	break;
		
	}
}

void releaseNormalKeys(unsigned char key, int x, int y) {

	switch (key) {
		case 'a':   deltaMoveX += 0.5f;		break;
		case 'd':	deltaMoveX -= 0.5f;		break;
		case 'w':	deltaMoveZ -= 0.5f;		break;
		case 's':	deltaMoveZ += 0.5f;		break;
	}

}

void processMouseMovement(int x, int y) {

	// this will only be true when the left button is down
	if (xPos >= 0) {

		// update deltaAngle
		deltaAngleXY = (x - xPos) * 0.001f;

		// update camera's direction
		lx = sin(angleXZ + deltaAngleXZ);
		//ly = 
		lz = -cos(angleXZ + deltaAngleXZ);
	}

	if (yPos >= 0) {

	}

}

void computePos(float deltaX, float deltaZ) {

	x += deltaZ * lx * 0.1f;
	z += deltaZ * lz * 0.1f;

	// calculate cross product of up and forward vector
	strafeX = -lz;
	strafeZ = lx;

	x += deltaX * strafeX * 0.1f;
	z += deltaX * strafeZ * 0.1f;

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