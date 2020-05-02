#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Renderer.h"

#include <glut.h>

//***************************************************************************//
//							   Object Declaration							 //
//***************************************************************************//

Camera  cam;
Keyboard kb;
Mouse mouse;
Renderer scene;

//***************************************************************************//
//							Regestering the Callbacks						 //
//***************************************************************************//

void renderScene(){
	scene.renderScene();
}
void reshapeScene(int width, int height){
	scene.reshapeScene(width, height);
}

// this is where we process the Special Keyboard Button Presses eg: F1, F2, F3, Up, Down, Home etc.
void pressKey(int key, int x, int y){}
void releaseKey(int key, int x, int y){}

void processNormalKeys(unsigned char key, int x, int y){}

void pressMouseButton(int button, int state, int x, int y){}
void processMouseMovement(int x, int y){}


void registerCallbacks() {

	glutDisplayFunc(renderScene);	// register the callback to the render start point
	glutReshapeFunc(reshapeScene);	// register the callback to the function used to change the perspective so that window resizing doesn't cause any issues
	glutIdleFunc(renderScene);		// register the callback to the function which is executed whenever a frame is idle

	glutIgnoreKeyRepeat(1); // so that repetetive key presses are ignored

	glutKeyboardFunc(processNormalKeys);	// register the callback to the Normal Keyboard Buttons

	glutSpecialFunc(pressKey);	// register the callback to the Special Keyboard Buttons
	glutSpecialUpFunc(releaseKey);

	glutMouseFunc(pressMouseButton);		// register the callback to mouse buttons
	glutMotionFunc(processMouseMovement);	// register the callback to mouse movement

}

int main(int argc, char** argv) {
	
	// init the GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// init the GLUT Window
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(1366, 728);
	glutCreateWindow("The Scene");

	registerCallbacks();

	glEnable(GL_DEPTH_TEST);	//  does depth comparisons and updates the depth buffer

	// enter teh GLUT event processing cycle
	glutMainLoop();

	return 1;

}