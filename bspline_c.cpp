//
//  bspline_c.cpp
//  ECS 175 P4
//
//  Created by Nan Chen on 11/26/19.
//  Copyright © 2019 Nan Chen. All rights reserved.
//

#include "bspline_c.h"
#include <iostream>

using namespace std;

bspline_c::bspline_c(int k): k(k), n_points(0)
{
}

// using deBoor algorithm to compute the point given u
glm::vec2 bspline_c::deBoor(float u)
{
	//initializes the deBoor matrix
	glm::vec2** matrix = new glm::vec2*[n_points];
	for (int i = 0; i < n_points; i++)
		matrix[i] = new glm::vec2[k];

	// add control points
	for (int i = 0; i < n_points; i++)
		matrix[i][0] = controlP[i];

	// calculate next generations points
	float step = 1.0f / ((n_points + k - 1) * 1.0f);
	int I = findI(u);

	for (int j = 1; j < k; j++)
		for (int i = I - k + 1; i < I - j + 1; i++)
			matrix[i][j] = ((i + k) * step - u) / ((k - j) * step) * matrix[i][j - 1] + (u - (i + j) * step) / ((k - j) * step) * matrix[i + 1][j - 1];

	glm::vec2 P = matrix[I - k + 1][k - 1];

	for (int i = 0; i < n_points; i++)
		delete[] matrix[i];
	delete[] matrix;

	return P;
}

// get the domain of the curve
vector<float> bspline_c::cal_domain()
{
	domain.clear();
	float step = 1.0f / ((n_points + k - 1) * 1.0f);
	for (int i = k - 1; i <= n_points; i++)
	{
		domain.push_back(i * step);
	}
	return domain;
}

//find the I given u and domain
int bspline_c::findI(float u)
{
	int I = k - 1;
	for (int i = k - 1; i <= n_points - 1; i++)
	{
		if (u >= domain[i - k + 1] && u <+ domain[i - k + 2])
		{
			I = i;
			break;
		}
	}
	return I;
}


// add a control point to the curve
void bspline_c::addPoints(glm::vec2 P)
{
	controlP.push_back(P);
	n_points++;
}

// delete a control point 
void bspline_c::deletePoints(int index)
{
	if ((int) controlP.size() - 1 < k)
		cout << "Can't delete more points (K <= n + 1)" << endl;
	else
	{
		controlP.erase(controlP.begin() + index);
		n_points--;
	}
}

void bspline_c::decreaseK()
{
	if (k - 1 == 0)
		cout << "Can't delete K any more (K >= 1)" << endl;
	else
		k--;
}

void bspline_c::increaseK()
{
	if (k + 1 > n_points)
		cout << "Can't increase K any more (K <= n + 1)" << endl;
	else
		k++;
}

// connect all the control points and draw them
void bspline_c::drawPoints()
{
	for (int i = 0; i < n_points; i++)
	{
		float x = controlP[i][0] * 2.0f - 1.0f;
		float y = controlP[i][1] * 2.0f - 1.0f;
		glColor3f(1, 0, 1);
		glBegin(GL_QUADS);
		glVertex2f(x, y + 0.01f);
		glVertex2f(x + 0.01f, y);
		glVertex2f(x, y - 0.01f);
		glVertex2f(x - 0.01f, y);
		glEnd();
	}
}

// move a control point to position x, y
void bspline_c::movePoints(float x, float y, int i)
{
	controlP[i][0] = x;
	controlP[i][1] = y;
}

// draw the bspline curve
void bspline_c::drawCurve(int resolution)
{
	float u;
	glm::vec2 P;
	int x, y;
	// calculate the domain
	cal_domain();
	//calculate the step for resolution
	float step = (domain[n_points - k + 1] - domain[0]) / (resolution * 1.0f);
	for (int i = 0; i <= resolution; i++)
	{
		u = domain[0] + i * step;
		P = deBoor(u);
		x = rd(P[0] * WinW);
		y = rd(P[1] * WinH);
		MakePix(x, y, 0, 0.5, 0.5);
	}
}

