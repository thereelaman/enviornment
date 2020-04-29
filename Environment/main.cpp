#include<glut.h>

void renderScene();
void reshapeScene(int,int);

int main(int argc, char** argv) {

	//init the GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	//init the GLUT Window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1366, 728);
	glutCreateWindow("The Scene");

	//register the callback to the render start point
	glutDisplayFunc(renderScene);

	//register the function used to change the perspective so that window resizing doesn't cause any issues
	glutReshapeFunc(reshapeScene);

	//enter teh GLUT event processing cycle
	glutMainLoop();

	return 1;

}

//this is where we start rendering our scene
void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glEnd();

	glutSwapBuffers();
}

//this is where we reshape the scene/ change the perspective
void reshapeScene(int w, int h) { // w, h represents width and height


	// we can't make a window of zero height, so we set the height to be 1
	// because when h=0, there can be a divide by zero error when calculating the ratio of the	
	if (h == 0)
		h = 1;

	float ratio = 1.0 * w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, 1, 1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}