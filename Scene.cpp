﻿#include "Scene.h"
#include "imageloader.h"

Scene* currentInstance;
float _angle = 0.0;
GLdouble zoom = 0.0f;
GLuint _textureBrick, _textureDoor, _textureGrass, _textureRoof, _textureWindow, _textureSky, _textureChimney, _textureSand, _textureSnow, _textureWood, _textureMoon;

void displaycallback()
{
	currentInstance->display();
}
void reshapecallback(GLint w, GLint h)
{
	currentInstance->reshape(w, h);
}
void keyboardcallback(unsigned char key, int x, int y)
{
	currentInstance->keyboard(key, x, y);
}
void keyboardupcallback(unsigned char key, int x, int y)
{
	currentInstance->keyboardUp(key, x, y);
}
void Specialcallback(int key, int x, int y)
{
	currentInstance->SpecialInput(key, x, y);
}
void Specialupcallback(int key, int x, int y)
{
	currentInstance->SpecialInputUp(key, x, y);
}
void timercallback(int v)
{
	currentInstance->timer(v);
}

GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
   GLdouble ortho = 250 + zoom;
   glOrtho(-ortho, ortho, -ortho, ortho, -250, 250);

//	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	Image* image = loadBMP("bricks.bmp");
	_textureBrick = loadTexture(image);
	image = loadBMP("door.bmp");
	_textureDoor = loadTexture(image);
	image = loadBMP("grass.bmp");
	_textureGrass = loadTexture(image);
	image = loadBMP("roof.bmp");
	_textureRoof = loadTexture(image);
	image = loadBMP("window.bmp");
	_textureWindow = loadTexture(image);
	image = loadBMP("sky.bmp");
	_textureSky = loadTexture(image);
   image = loadBMP("chimney.bmp");
	_textureChimney = loadTexture(image);
   image = loadBMP("sand.bmp");
	_textureSand = loadTexture(image);
   image = loadBMP("snow.bmp");
	_textureSnow = loadTexture(image);
   image = loadBMP("wood.bmp");
	_textureWood = loadTexture(image);
   image = loadBMP("moon.bmp");
	_textureMoon = loadTexture(image);
	delete image;

}

Scene::Scene(int argc, char** argv) {
	// init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("3D House");

	Initialize();

	// create drawing objects
	this->dog = new Dog("GermanShephardLowPoly.obj", 0, -1.5f, 1.5, 0.05f, glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0),30);
   this->horse = new ObjectGL("horse01.obj", -1.0, -1.5f, 2.0, 0.4f, glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), 180);
   for (int i=0; i<5; i++){
       this->tree[i] = new ObjectGL("tree.obj", -4.0+2.0*i, 2.0f, -3.0, 2.0f, glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0));
   }
   this->sled = new ObjectGL("sled.obj", -2.0, -1.2f, 3.0, 0.1f, glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), -90);
   this->snowman = new ObjectGL("snowman.obj", 2.0, -1.2f, 2.5, 4.0f, glm::vec3(0, 1, 0), glm::vec3(-1, 0, 0), -30);
   this->flashlight = new Light(GL_LIGHT0, 0, 8, 0, "Flashlight.obj", 0.2f);
	this->flashlight->towardVector = glm::vec3(0, 0, 1);

	// configure glut funcs
	::currentInstance = this;
	glutReshapeFunc(reshapecallback);
	glutDisplayFunc(displaycallback);
	glutTimerFunc(100, timercallback, 0);
	glutKeyboardFunc(keyboardcallback);
	glutKeyboardUpFunc(keyboardupcallback);
	glutSpecialFunc(Specialcallback);
	glutSpecialUpFunc(Specialupcallback);

	glutMainLoop(); // run the main loop
}

// Handles the display callback to show what we have drawn.
void Scene::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspect, 1, 100.0); // use Perspective projection

	// set view
    gluLookAt(0, 0, 1, camera_target[0], camera_target[1], camera_target[2], 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// enable opengl features
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	// add scene lights
//	flashlight->addlight();

    // Sky
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, _textureSky);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-10);
        glBegin(GL_QUADS);
            glTexCoord3f(0.0,1.0,0.1);  glVertex3f(-20,20,0);
            glTexCoord3f(1.0,1.0,0.1);  glVertex3f(20,20,0);
            glTexCoord3f(1.0,0.0,0.1);  glVertex3f(20,-20,0);
            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-20,-20,0);
        glEnd();
    glPopMatrix();

   // Moon
   glPushMatrix();
      glBindTexture(GL_TEXTURE_2D, _textureSnow);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTranslatef(0,0,-6);
      glRotatef(_angle, 0.0, 1.0, 1.0);

      glBegin(GL_TRIANGLE_FAN);
        glVertex3f(4.0, 3.0, -4.0); // Center
        for(int i = 0.0f; i <= 360; i++)
                glVertex3f(0.5*cos(3.1416 * i / 180.0) + 4.0, 0.5*sin(3.1416 * i / 180.0) + 3.0, -4.0);

        glEnd();
   glPopMatrix();

	// Snow
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, _textureSnow);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
            glTexCoord3f(0.0,70.0,1);  glVertex3f(-50,-1.5,50);
            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-50,-1.5,-50);
            glTexCoord3f(70.0,0.0,-1);  glVertex3f(50,-1.5,-50);
            glTexCoord3f(70.0,70.0,1);  glVertex3f(50,-1.5,50);
        glEnd();
    glPopMatrix();

    // Front side
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);  // Wall
            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-2,0.1,1);
            glTexCoord3f(4.0,2.0,0.1);  glVertex3f(2,0.1,1);
            glTexCoord3f(4.0,0.0,0.1);  glVertex3f(2,-1.5,1);
            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-2,-1.5,1);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureChimney);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Chimney
            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1.8,1.2,0.6);
            glTexCoord3f(4.0,2.0,0.1);  glVertex3f(-1.5,1.2,0.6);
            glTexCoord3f(4.0,0.0,0.1);  glVertex3f(-1.5,0.5,0.6);
            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1.8,0.5,0.6);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureSnow);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Roof
            glTexCoord3f(0.0,2.0,0); glVertex3f(-2.2,1.0001,0);
            glTexCoord3f(4.0,2.0,0);glVertex3f(2.2,1.0001,0);
            glTexCoord3f(4.0,0.0,1.25); glVertex3f(2.2,0.0,1.25);
            glTexCoord3f(0.0,0.0,1.25); glVertex3f(-2.2,0.0,1.25);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Roof Lower
            glTexCoord3f(0.0,2.0,0); glVertex3f(-2.2,1.0,0);
            glTexCoord3f(4.0,2.0,0);glVertex3f(2.2,1.0,0);
            glTexCoord3f(4.0,0.0,1.25); glVertex3f(2.2,0.0,1.25);
            glTexCoord3f(0.0,0.0,1.25); glVertex3f(-2.2,0.0,1.25);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureDoor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Door
            glTexCoord3f(0.0,1.0,1.0001); glVertex3f(0.75,-0.4,1.0001);
            glTexCoord3f(1.0,1.0,1.0001); glVertex3f(1.5,-0.4,1.0001);
            glTexCoord3f(1.0,0.0,1.0001); glVertex3f(1.5,-1.5,1.0001);
            glTexCoord3f(0.0,0.0,1.0001); glVertex3f(0.75,-1.5,1.0001);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureWindow);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Window Left
            glTexCoord3f(0.0,1.0,1.0001); glVertex3f(-1.5,-0.4,1.0001);
            glTexCoord3f(1.0,1.0,1.0001); glVertex3f(-0.75,-0.4,1.0001);
            glTexCoord3f(1.0,0.0,1.0001); glVertex3f(-0.75,-0.9,1.0001);
            glTexCoord3f(0.0,0.0,1.0001); glVertex3f(-1.5,-0.9,1.0001);
        glEnd();

        glBegin(GL_QUADS);  // Window Right
            glTexCoord3f(0.0,1.0,1.0001); glVertex3f(0.3,-0.4,1.0001);
            glTexCoord3f(1.0,1.0,1.0001); glVertex3f(-0.3,-0.4,1.0001);
            glTexCoord3f(1.0,0.0,1.0001); glVertex3f(-0.3,-0.9,1.0001);
            glTexCoord3f(0.0,0.0,1.0001); glVertex3f(0.3,-0.9,1.0001);
        glEnd();
    glPopMatrix();

    // Back side
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);  // Wall
            glTexCoord3f(0.0,2.0,-1);  glVertex3f(-2,0.1,-1);
            glTexCoord3f(4.0,2.0,-1);  glVertex3f(2,0.1,-1);
            glTexCoord3f(4.0,0.0,-1);  glVertex3f(2,-1.5,-1);
            glTexCoord3f(0.0,0.0,-1);  glVertex3f(-2,-1.5,-1);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureChimney);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Chimney
            glTexCoord3f(0.0,2.0,0.1);  glVertex3f(-1.8,1.2,0.3);
            glTexCoord3f(4.0,2.0,0.1);  glVertex3f(-1.5,1.2,0.3);
            glTexCoord3f(4.0,0.0,0.1);  glVertex3f(-1.5,0.8,0.3);
            glTexCoord3f(0.0,0.0,0.1);  glVertex3f(-1.8,0.8,0.3);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureSnow);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Roof
            glTexCoord3f(0.0,2.0,0); glVertex3f(-2.2,1.0001,0);
            glTexCoord3f(4.0,2.0,0);glVertex3f(2.2,1.0001,0);
            glTexCoord3f(4.0,0.0,-1.25); glVertex3f(2.2,0.0,-1.25);
            glTexCoord3f(0.0,0.0,-1.25); glVertex3f(-2.2,0.0,-1.25);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Roof Lower
            glTexCoord3f(0.0,2.0,0); glVertex3f(-2.2,1.0,0);
            glTexCoord3f(4.0,2.0,0);glVertex3f(2.2,1.0,0);
            glTexCoord3f(4.0,0.0,-1.25); glVertex3f(2.2,0.0,-1.25);
            glTexCoord3f(0.0,0.0,-1.25); glVertex3f(-2.2,0.0,-1.25);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureWindow);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Window Left
            glTexCoord3f(0.0,1.0,-1.0001); glVertex3f(-1.5,-0.3,-1.0001);
            glTexCoord3f(1.0,1.0,-1.0001); glVertex3f(-0.75,-0.3,-1.0001);
            glTexCoord3f(1.0,0.0,-1.0001); glVertex3f(-0.75,-0.8,-1.0001);
            glTexCoord3f(0.0,0.0,-1.0001); glVertex3f(-1.5,-0.8,-1.0001);
        glEnd();

        glBegin(GL_QUADS);  // Window Right
            glTexCoord3f(0.0,1.0,1.0001); glVertex3f(1.5,-0.3,-1.0001);
            glTexCoord3f(1.0,1.0,1.0001); glVertex3f(0.75,-0.3,-1.0001);
            glTexCoord3f(1.0,0.0,1.0001); glVertex3f(0.75,-0.8,-1.0001);
            glTexCoord3f(0.0,0.0,1.0001); glVertex3f(1.5,-0.8,-1.0001);
        glEnd();
    glPopMatrix();

    // Right side
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);  // Wall
            glTexCoord3f(0.0,2.0,1); glVertex3f(2,0.2,1);
            glTexCoord3f(2.0,2.0,-1); glVertex3f(2,0.2,-1);
            glTexCoord3f(2.0,0.0,-1); glVertex3f(2,-1.5,-1);
            glTexCoord3f(0.0,0.0,1); glVertex3f(2,-1.5,1);
        glEnd();

        glBegin(GL_TRIANGLES);  // Wall Upper
            glTexCoord3f(0.0,1.0,0); glVertex3f(2,1.0,0);
            glTexCoord3f(1.0,0.0,1); glVertex3f(2,0.2,1);
            glTexCoord3f(-1.0,0.0,-1); glVertex3f(2,0.2,-1);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureChimney);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Chimney
            glTexCoord3f(0.0,2.0,1); glVertex3f(-1.5,1.2,0.6);
            glTexCoord3f(2.0,2.0,-1); glVertex3f(-1.5,1.2,0.3);
            glTexCoord3f(2.0,0.0,-1); glVertex3f(-1.5,0.5,0.3);
            glTexCoord3f(0.0,0.0,1); glVertex3f(-1.5,0.5,0.6);
        glEnd();
    glPopMatrix();

    // Left side
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, _textureWood);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTranslatef(0,0,-6);
        glRotatef(_angle, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);  // Wall
            glTexCoord3f(0.0,2.0,1);    glVertex3f(-2,0.2,1);
            glTexCoord3f(2.0,2.0,-1);    glVertex3f(-2,0.2,-1);
            glTexCoord3f(2.0,0.0,-1);    glVertex3f(-2,-1.5,-1);
            glTexCoord3f(0.0,0.0,1);    glVertex3f(-2,-1.5,1);
        glEnd();

        glBegin(GL_TRIANGLES);  // Wall Upper
            glTexCoord3f(0.0,1.0,0);    glVertex3f(-2,1.0,0);
            glTexCoord3f(1.0,0.0,1);    glVertex3f(-2,0.2,1);
            glTexCoord3f(-1.0,0.0,-1);    glVertex3f(-2,0.2,-1);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureChimney);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Chimney
            glTexCoord3f(0.0,2.0,1); glVertex3f(-1.8,1.2,0.6);
            glTexCoord3f(2.0,2.0,-1); glVertex3f(-1.8,1.2,0.3);
            glTexCoord3f(2.0,0.0,-1); glVertex3f(-1.8,0.65,0.3);
            glTexCoord3f(0.0,0.0,1); glVertex3f(-1.8,0.5,0.6);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, _textureDoor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);  // Door
            glTexCoord3f(0.0,1.0,1.0001); glVertex3f(-2.001,-0.4,0.3);
            glTexCoord3f(1.0,1.0,1.0001); glVertex3f(-2.001,-0.4,-0.3);
            glTexCoord3f(1.0,0.0,1.0001); glVertex3f(-2.001,-1.5,-0.3);
            glTexCoord3f(0.0,0.0,1.0001); glVertex3f(-2.001,-1.5,0.3);
        glEnd();
    glPopMatrix();

   // Objects
	glEnable(GL_LIGHTING);
      glTranslatef(0,0,-6);
      glRotatef(_angle, 0.0, 1.0, 0.0);
      dog->draw();
      horse->draw();
      for (int i=0; i<5; i++){
         tree[i]->draw();
      }
      sled->draw();
      snowman->draw();
    glDisable(GL_LIGHTING);
//
    glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1

    //Add ambient light
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = {4.0f, 0.0f, 8.0f, 1.0f}; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = {0.5f, 0.2f, 0.2f, 1.0f}; //Color (0.5, 0.2, 0.2)
	//Coming from the direction (-1, 0.5, 0.5)
	GLfloat lightPos1[] = {-1.0f, 0.5f, 5.5f, 0.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

// Handles keyboard press
void Scene::keyboard(unsigned char key, int x, int y) {

	key = tolower(key);
	if (key == 'w') {
		dog->walk(0.05f);
	}
	else if (key == 's') {
		dog->walk(-0.05f);
	}
	else if (key == 'd') {
		dog->rotate(-5.0f);
	}
	else if (key == 'a') {
		dog->rotate(5.0f);
	}
	else if (key == 'q') {
		dog->rotateOrgan(2.0f, DOG_HEAD, false);
	}
	else if (key == 'e') {
		dog->rotateOrgan(-2.0f, DOG_HEAD, false);
	}
	else if (key == 'g') {
		dog->rotateOrgan(1.0f, DOG_HEAD, true);
	}
	else if (key == 't') {
		dog->rotateOrgan(-1.0f, DOG_HEAD, true);
	}
	glutPostRedisplay();
}

// Handles keyboard after press
void Scene::keyboardUp(unsigned char key, int x, int y) {

	key = tolower(key);
	if (key == 'w' || key == 's') {
		close_legs_vert = true;
	}
	if (key == 'a' || key == 'd') {
		close_legs_hor = true;
	}
}

void Scene::SpecialInput(int key, int x, int y) {
	// do the key action
//	switch (key)
//	{
//	case GLUT_KEY_UP:
//		dog->walk(0.05f);
//		break;
//	case GLUT_KEY_DOWN:
//		dog->walk(-0.05f);
//		break;
//	case GLUT_KEY_LEFT:
//		dog->rotate(5.0f);
//		break;
//	case GLUT_KEY_RIGHT:
//		dog->rotate(-5.0f);
//		break;
//	}
		switch (key) {
    case GLUT_KEY_RIGHT:
        _angle += 1;
        if (_angle > 360) _angle = 0.0;
		break;
    case GLUT_KEY_LEFT:
        _angle -= 1;
        if (_angle > 360) _angle = 0.0;
	    break;
	}
}

void Scene::SpecialInputUp(int key, int x, int y) {
	// make the legs move when walk or rotate
	if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN ) {
		close_legs_vert = true;
	}
	if (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT) {
		close_legs_hor = true;
	}
}

void Scene::timer(int v) {
	// wag tail if needed
	wag_tail = true;
	if (wag_tail) {
		dog->wagTail();
		glutPostRedisplay();
	}
	// move legs if needed
	if (close_legs_vert && abs(dog->organsAngles[DOG_LEFT_BACK_LEG][true]) >= 3.0f) {
		dog->moveLegs(3.0f, true);
		glutPostRedisplay();
	}
	else
	{
		close_legs_vert = false;
	}
	// move legs if needed
	if (close_legs_hor && abs(dog->organsAngles[DOG_LEFT_BACK_LEG][false]) >= 8.0f) {
		dog->moveLegs(8.0f, false);
		glutPostRedisplay();
	}
	else
	{
		close_legs_hor = false;
	}
	glutTimerFunc(1000 / 60, timercallback, v); // recursivly call to itslef for the next frame
}

// Handles the window reshape event
void Scene::reshape(GLint w, GLint h) {

	glViewport(0, 0, w, h);
	aspect = float(w) / float(h);
}
