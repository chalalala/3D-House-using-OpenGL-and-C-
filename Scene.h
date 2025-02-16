#pragma once

#include <GL/glut.h>
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <iostream>

using namespace std;

#include "Dog.h"
#include "Light.h"

// window vars
const int WINDOW_WIDTH = 1500;
const int WINDOW_HEIGHT = 900;
const int WINDOW_POS_X = 100;
const int WINDOW_POS_Y = 50;
const float WINDOW_RATIO = WINDOW_WIDTH / (float)WINDOW_HEIGHT;
static float aspect = WINDOW_RATIO;

// camera vars
static GLfloat camera_target[3] = { 0, 0, 0 };

// dog
static bool wag_tail = false;
static bool close_legs_hor = false;
static bool close_legs_vert = false;

// this class represent the whole scene
class Scene
{
private:
	Dog* dog;
	ObjectGL* horse;
	ObjectGL* tree[5];
	ObjectGL* sled;
	ObjectGL* snowman;
    Light* flashlight;
	static Scene* currentInstance; // trick that helps to add opengl callbacks in class

public:
	Scene(int argc, char** argv);
	void display(); // Function where the scene drawing occures
	void keyboard(unsigned char key, int x, int y); // Function for keyboard press
	void keyboardUp(unsigned char key, int x, int y); // // Function for keyboard after press
    void mySpecialFunc(int key, int x, int y);
	void reshape(GLint w, GLint h); // Function to handle reshape of the screen
	void SpecialInput(int key, int x, int y); // Function to handle arrows key
	void SpecialInputUp(int key, int x, int y); // Function to handle arrows key after press
	void timer(int v); // function that will be called every interval of time
};

