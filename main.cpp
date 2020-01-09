//
//  main.cpp
//  ECS 175 P4
//
//  Created by Nan Chen on 11/26/19.
//  Copyright © 2019 Nan Chen. All rights reserved.
//

#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include "bezier_c.h"
#include "bspline_c.h"
#include <string>
#include "glm/glm.hpp"

using namespace std;

int getInput(); //get the int input and check if the input is valid
void display(); //display function
void KeyBoardFunc(unsigned char key, int x, int y);  //the keyboard function
void MouseFunc(int button, int state, int x, int y);  //the mouse function
bool checkBezier(float x_pos, float y_pos);
bool checkBspline(float x_pos, float y_pos);
void MotionFunc(int x, int y); //mouse motion function
void readFile(); // read the curve information
void writeFile(); // output the curve information 
void displayInfo(); //display the information of the curve

float* PixelBuffer; //the pixel buffer for normal shading
int WinW = 900;  //window width
int WinH = 900;  //window height
int select1 = -1; //the select points
int select2 = -1; //the select points
int delete1 = -1; //flag for deleting control points for bezier curve
int delete2 = -1; //flag for deleting control points for bspline curve
int bezier_index = -1; // the index for vector bezier curve
int bspline_index = -1; // the bspline index 
int addCurve = 0; // determin if we are adding a new curve
int slid = -1; //flag for slider
int bezier_resolution = 1000, bspline_resolution = 1000;

int MainWindow; 
GLvoid *font_style = GLUT_BITMAP_HELVETICA_12; //font style
vector<bezier_c> bezier;
vector<bspline_c> bspline;
vector<glm::vec2> points;

//get the int input and check if the input is valid
int getInput()
{
	int input;
	//if input is not a float
	while (!(cin >> input))
	{
		cout << "format is wrong" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please enter again: ";
	}
	return input;
}

void KeyBoardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
		//adding a bezier curve
		case '1':
		{
			if (addCurve == 0)
			{
				addCurve = 1;
				bspline_index = -1;
				bezier_index = -1;
			}
			break;
		}

		//adding a bspline curve
		case '2':
		{
			if (addCurve == 0)
			{
				addCurve = 2;
				bspline_index = -1;
				bezier_index = -1;
			}
			break;
		}

		//finish adding the curve
		case 'f':
		{
			if (addCurve == 1)
			{
				bezier_c curve = bezier_c();

				// add the points to the curve
				for (unsigned int i = 0; i < points.size(); i++)
					curve.addPoints(points[i]);

				bezier.push_back(curve);
			}

			else if (addCurve == 2)
			{
				//getting k value from the user
				cout << "Please enter the k value for the Bspline Curve: ";
				int k = getInput();

				while (k <= 0 || k > (int) points.size())
				{
					cout << k << " is not appropriate for this curve (1 <= k <= n + 1)" << endl;
					cout << "Please enter again: ";
					k = getInput();
				}

				bspline_c curve = bspline_c(k);
				
				// add the points to the curve
				for (unsigned int i = 0; i < points.size(); i++)
					curve.addPoints(points[i]);

				bspline.push_back(curve);
			}

			points.clear();
			addCurve = 0;
			glutPostRedisplay();
			break;
		}

		//delete the selected curve
		case 'd':
		{
			if (bezier_index != -1 && bezier.size() != 0)
			{
				bezier.erase(bezier.begin() + bezier_index);
				bezier_index = -1;
			}
			else if (bspline_index != -1 && bspline.size() != 0)
			{
				bspline.erase(bspline.begin() + bspline_index);
				bspline_index = -1;
			}
			glutPostRedisplay();
			break;
		}


		//set new resolution for bezier curve
		case 'a':
		{
			cout << "Enter a new resolution for Bezier Curves: ";
			bezier_resolution = getInput();
			glutPostRedisplay();
			break;
		}

		//set new resolution for bspline curve
		case 's':
		{
			cout << "Enter a new resolution for Bspline Curves: ";
			bspline_resolution = getInput();
			glutPostRedisplay();
			break;
		}

		//increase the selected bspline curve k value by 1
		case 'k':
		{
			if (bspline_index != -1)
				bspline[bspline_index].increaseK();
			glutPostRedisplay();
			break;
		}

		//decrease the selected bspline curve k value by 1
		case 'l':
		{
			if (bspline_index != -1)
				bspline[bspline_index].decreaseK();
			glutPostRedisplay();
			break;
		}

		//save and exit the program
		case 27:
		{
			glutDestroyWindow(MainWindow);

			//output file
			writeFile();

			exit(0);
		}
	}
}

void MouseFunc(int button, int state, int x, int y)
{
	int yp = WinH - y, xp = x;
	float x_pos = xp / (WinW * 1.0f);
	float y_pos = yp / (WinH * 1.0f);
	
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
	{
		//press down the mouse
		if (state == GLUT_DOWN)
		{
			// not adding a new curve
			if (addCurve == 0)
			{
				/* performing translation */
				//inside the cursor box of the control points for bezier curve
				for (unsigned int i = 0; i < bezier.size(); i++)
				{
					for (int j = 0; j < bezier[i].n_points; j++)
					{
						if (x_pos <= (bezier[i].controlP[j][0] + 0.005f) && x_pos >= (bezier[i].controlP[j][0] - 0.005f)
							&& y_pos <= (bezier[i].controlP[j][1] + 0.005f) && y_pos >= (bezier[i].controlP[j][1] - 0.005f))
						{
							select1 = j;
							bezier_index = i;
							bspline_index = -1;
							break;
						}
					}
				}

				//inside the cursor box of the control points for bspline curve
				for (unsigned int i = 0; i < bspline.size(); i++)
				{
					for (int j = 0; j < bspline[i].n_points; j++)
					{
						if (x_pos <= (bspline[i].controlP[j][0] + 0.005f) && x_pos >= (bspline[i].controlP[j][0] - 0.005f)
							&& y_pos <= (bspline[i].controlP[j][1] + 0.005f) && y_pos >= (bspline[i].controlP[j][1] - 0.005f))
						{
							select2 = j;
							bspline_index = i;
							bezier_index = -1;
							break;
						}
					}
				}

				/* performing adding control points */
				// not the same points as before for bezier curve
				if (!checkBezier(x_pos, y_pos))
				{
					// if currently selecting a bezier curve to add points 
					if (bezier_index != -1)
						bezier[bezier_index].addPoints(glm::vec2(x_pos, y_pos));
				}

				// not the same points as before for bspline curve
				if (!checkBspline(x_pos, y_pos))
				{
					// if currently selecting a bspline curve to add points 
					if (bspline_index != -1)
						bspline[bspline_index].addPoints(glm::vec2(x_pos, y_pos));
				}
			}

			//adding a bezier curve
			else if (addCurve == 1)
			{
				points.push_back(glm::vec2(x_pos, y_pos));
			}

			//adding a bspline curve
			else if (addCurve == 2)
			{
				points.push_back(glm::vec2(x_pos, y_pos));
			}
		}

		//release
		else if (state == GLUT_UP)
		{
			if (select1 != -1)
			{
				select1 = -1;
			}
			if (select2 != -1)
			{
				select2 = -1;
			}
		}

		break;
	}

	case GLUT_RIGHT_BUTTON:
	{
		//press down the mouse
		if (state == GLUT_DOWN)
		{
			//inside the cursor box of the control points for bezier curve
			for (unsigned int i = 0; i < bezier.size(); i++)
			{
				for (int j = 0; j < bezier[i].n_points; j++)
				{
					if (x_pos <= (bezier[i].controlP[j][0] + 0.005f) && x_pos >= (bezier[i].controlP[j][0] - 0.005f)
						&& y_pos <= (bezier[i].controlP[j][1] + 0.005f) && y_pos >= (bezier[i].controlP[j][1] - 0.005f))
					{
						bezier[i].deletePoints(j);
						delete1 = j;
						break;
					}
				}
				if (delete1 != -1)
					break;
			}

			//inside the cursor box of the control points for bspline curve
			for (unsigned int i = 0; i < bspline.size(); i++)
			{
				for (int j = 0; j < bspline[i].n_points; j++)
				{
					if (x_pos <= (bspline[i].controlP[j][0] + 0.005f) && x_pos >= (bspline[i].controlP[j][0] - 0.005f)
						&& y_pos <= (bspline[i].controlP[j][1] + 0.005f) && y_pos >= (bspline[i].controlP[j][1] - 0.005f))
					{
						bspline[i].deletePoints(j);
						delete2 = j;
						break;
					}
				}
				if (delete2 != -1)
					break;
			}
		}

		//release
		else if (state == GLUT_UP)
		{
			delete1 = -1;
			delete2 = -1;
		}

		break;
	}

	}
	glutPostRedisplay();
}

bool checkBezier(float x_pos, float y_pos)
{
	//inside the cursor box of the control points for bezier curve
	for (unsigned int i = 0; i < bezier.size(); i++)
	{
		for (int j = 0; j < bezier[i].n_points; j++)
		{
			if (x_pos <= (bezier[i].controlP[j][0] + 0.005f) && x_pos >= (bezier[i].controlP[j][0] - 0.005f)
				&& y_pos <= (bezier[i].controlP[j][1] + 0.005f) && y_pos >= (bezier[i].controlP[j][1] - 0.005f))
				return true;
		}
	}
	return false;
}

bool checkBspline(float x_pos, float y_pos)
{
	//inside the cursor box of the control points for bspline curve
	for (unsigned int i = 0; i < bspline.size(); i++)
	{
		for (int j = 0; j < bspline[i].n_points; j++)
		{
			if (x_pos <= (bspline[i].controlP[j][0] + 0.005f) && x_pos >= (bspline[i].controlP[j][0] - 0.005f)
				&& y_pos <= (bspline[i].controlP[j][1] + 0.005f) && y_pos >= (bspline[i].controlP[j][1] - 0.005f))
				return true;
		}
	}
	return false;
}

void MotionFunc(int x, int y)
{
	int xp = x;
	int yp = WinH - y;

	// make it inside the window
	if (xp > WinW)
		xp = WinW;
	if (xp < 0)
		xp = 0;
	if (yp > WinH)
		yp = WinH;
	if (yp < 0)
		yp = 0;

	float x_pos = xp / (WinW * 1.0f);
	float y_pos = yp / (WinH * 1.0f);

	if (select1 != -1)
	{
		bezier[bezier_index].movePoints(x_pos, y_pos, select1);
		glutPostRedisplay();
	}

	if (select2 != -1)
	{
		bspline[bspline_index].movePoints(x_pos, y_pos, select2);
		glutPostRedisplay();
	}

	
}

//read the curve information from test.txt
void readFile()
{
	int n_bezier, n_bspline, n_points, k;
	float x, y;
	glm::vec2 P;

	//read in file 
	ifstream inf;
	inf.open("test.txt");

	// reading in bezier curves
	inf >> n_bezier;

	for (int i = 0; i < n_bezier; i++)
	{
		bezier_c curve = bezier_c();
		inf >> n_points;
		for (int j = 0; j < n_points; j++)
		{
			inf >> x >> y;
			P = glm::vec2(x, y);
			curve.addPoints(P);
		}
		bezier.push_back(curve);
	}

	/// reading in bspline curves
	inf >> n_bspline;
	for (int i = 0; i < n_bezier; i++)
	{
		inf >> k;
		inf >> n_points;

		bspline_c curve = bspline_c(k);

		for (int j = 0; j < n_points; j++)
		{
			inf >> x >> y;
			P = glm::vec2(x, y);
			curve.addPoints(P);
		}
		bspline.push_back(curve);
	}

	inf.close();
}

// write to test.txt the curve information
void writeFile()
{
	//output file
	ofstream outf;
	outf.open("test.txt", ios::out);

	outf << bezier.size() << endl;
	for (unsigned int i = 0; i < bezier.size(); i++)
	{
		outf << bezier[i].n_points << endl;
		for (int j = 0; j < bezier[i].n_points; j++)
			outf << bezier[i].controlP[j][0] << " " << bezier[i].controlP[j][1] << endl;
	}

	outf << bspline.size() << endl;
	for (unsigned int i = 0; i < bspline.size(); i++)
	{
		outf << bspline[i].k << endl;
		outf << bspline[i].n_points << endl;
		for (int j = 0; j < bspline[i].n_points; j++)
			outf << bspline[i].controlP[j][0] << " " << bspline[i].controlP[j][1] << endl;
	}

	outf.close();
}

void displayInfo()
{
	string str = "Resolution for Bezier Curves: " + to_string(bezier_resolution);
	//draw the label
	glColor3f(0, 0, 0);
	drawstr(0.2f, -0.8f, str.c_str(), str.length());
	str = "Press A to set new resolution for Bezier Curves";
	drawstr(0.2f, -0.83f, str.c_str(), str.length());

	str = "Press D to delete a selected Curve";
	drawstr(0.2f, -0.71f, str.c_str(), str.length());

	str = "Press 1 to add a new Bezier Curve";
	drawstr(0.2f, -0.74f, str.c_str(), str.length());

	str = "Press 2 to add a new Bezier Curve";
	drawstr(0.2f, -0.77f, str.c_str(), str.length());
	
	str = "Resolution for Bspline Curves: " + to_string(bspline_resolution);
	drawstr(0.2f, -0.86f, str.c_str(), str.length());
	str = "Press S to set new resolution for Bspline Curves";
	drawstr(0.2f, -0.89f, str.c_str(), str.length());

	if (bspline_index != -1)
	{
		str = "Current selected Bspline Curve K value: " + to_string(bspline[bspline_index].k);
		drawstr(0.2f, -0.92f, str.c_str(), str.length());
		str = "Press k to increase K value by 1, L to decrease k value by 1";
		drawstr(0.2f, -0.95f, str.c_str(), str.length());
	}
	str = "Press esc to save and exit (test.txt)";
	drawstr(0.2f, -0.98f, str.c_str(), str.length());

}

//main program: read files, set default parameters for phong and project objects 
int main(int argc, char *argv[])
{
	//allocate buffer
	PixelBuffer = new float[WinW * WinH * 3];

	//initialize to white
	for (int i = 0; i < WinW * WinH * 3; i++)
		PixelBuffer[i] = 1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	//set window size to 500*500
	glutInitWindowSize(WinW, WinH);
	//set window position
	glutInitWindowPosition(100, 100);
	//create and set main window title
	MainWindow = glutCreateWindow("Project 4!");

	readFile(); // read file for curve information

	glClearColor(1, 1, 1, 0); //clears the buffer of OpenGL

	glutDisplayFunc(display); //display function
	
	glutKeyboardFunc(KeyBoardFunc); //keyboard function

	glutMouseFunc(MouseFunc); //mouse function

	glutMotionFunc(MotionFunc); //motion function

	glutMainLoop();//main display loop, will display until terminate

	return 0;
}

//display function
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draws pixel on screen
	glViewport(0, 0, WinW, WinH);
	glRasterPos3f(-1.0f, -1.0f, 0.0f);

	//initialize to 0
	for (int i = 0; i < WinW * WinH * 3; i++)
		PixelBuffer[i] = 1;

	//draw out bezier curves
	for (unsigned int i = 0; i < bezier.size(); i++)
		bezier[i].drawCurve(bezier_resolution);

	//draw out bspline curves
	for (unsigned int i = 0; i < bspline.size(); i++)
		bspline[i].drawCurve(bspline_resolution);

	glDrawPixels(WinW, WinH, GL_RGB, GL_FLOAT, PixelBuffer);

	//draw out bezier curves points
	for (unsigned int i = 0; i < bezier.size(); i++)
		bezier[i].drawPoints();

	//draw out bspline curves points 
	for (unsigned int i = 0; i < bspline.size(); i++)
		bspline[i].drawPoints();

	displayInfo();

	//draw out the new points for a new curve
	if (addCurve == 1)
	{
		float x_pos, y_pos;
		for (unsigned int i = 0; i < points.size(); i++)
		{
			x_pos = points[i][0] * 2.0f - 1.0f;
			y_pos = points[i][1] * 2.0f - 1.0f;
			glColor3f(0, 0, 1);
			glBegin(GL_QUADS);
			glVertex2f(x_pos, y_pos + 0.01f);
			glVertex2f(x_pos + 0.01f, y_pos);
			glVertex2f(x_pos, y_pos - 0.01f);
			glVertex2f(x_pos - 0.01f, y_pos);
			glEnd();
		}
	}

	else if (addCurve == 2)
	{
		float x_pos, y_pos;
		for (unsigned int i = 0; i < points.size(); i++)
		{
			x_pos = points[i][0] * 2.0f - 1.0f;
			y_pos = points[i][1] * 2.0f - 1.0f;
			glColor3f(1, 0, 1);
			glBegin(GL_QUADS);
			glVertex2f(x_pos, y_pos + 0.01f);
			glVertex2f(x_pos + 0.01f, y_pos);
			glVertex2f(x_pos, y_pos - 0.01f);
			glVertex2f(x_pos - 0.01f, y_pos);
			glEnd();
		}
	}
	//window refresh
	glFlush();
}

