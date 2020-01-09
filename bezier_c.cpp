//
//  bezier_c.cpp
//  ECS 175 P4
//
//  Created by Nan Chen on 11/26/19.
//  Copyright © 2019 Nan Chen. All rights reserved.
//

#include "bezier_c.h"
#include <iostream>

using namespace std;

// the default constructor: n = 0
bezier_c::bezier_c() : n_points(0)
{
}

// using de Casteljau algorithm to compute the point given t
glm::vec2 bezier_c::deCasteljau(float t)
{
	//initializes the deCasteljau matrix
	glm::vec2** matrix = new glm::vec2*[n_points];
	for (int i = 0; i < n_points; i++)
		matrix[i] = new glm::vec2[n_points];
	
	// add control points
	for (int i = 0; i < n_points; i++)
		matrix[i][0] = controlP[i];

	// calculate next generations points
	for (int j = 1; j < n_points; j++)
		for (int i = 0; i < n_points - j; i++)
			matrix[i][j] = (1.0f - t) * matrix[i][j - 1] + t * matrix[i + 1][j - 1];

	glm::vec2 P = matrix[0][n_points - 1];

	for (int i = 0; i < n_points; i++)
		delete [] matrix[i];
	delete [] matrix;

	return P;
}

// add a control point to the curve
void bezier_c::addPoints(glm::vec2 P)
{
	controlP.push_back(P);
	n_points++;
}

// delete a control point 
void bezier_c::deletePoints(int index)
{
	if ((int) controlP.size() - 1 <= 0)
		cout << "Can't delete more points (n >= 0)" << endl;
	else
	{ 
		controlP.erase(controlP.begin() + index);
		n_points--;
	}
}

// connect all the control points and draw them
void bezier_c::drawPoints()
{
	for (int i = 0; i < n_points; i++)
	{
		float x = controlP[i][0] * 2.0f - 1.0f;
		float y = controlP[i][1] * 2.0f - 1.0f;
		glColor3f(0, 0, 1);
		glBegin(GL_QUADS);
		glVertex2f(x, y + 0.01f);
		glVertex2f(x + 0.01f, y);
		glVertex2f(x, y - 0.01f);
		glVertex2f(x - 0.01f, y);
		glEnd();
	}
}

// move a control point to position x, y
void bezier_c::movePoints(float x, float y, int i)
{
	controlP[i][0] = x;
	controlP[i][1] = y;
}

// draw the bezier curve
void bezier_c::drawCurve(int resolution)
{
	float t;
	glm::vec2 P;
	int x, y;
	for (int i = 0; i <= resolution; i++)
	{
		t = i / (resolution * 1.0f);
		P = deCasteljau(t);
		x = rd(P[0] * WinW);
		y = rd(P[1] * WinH);
		MakePix(x, y, 1, 0, 0);
	}
}
