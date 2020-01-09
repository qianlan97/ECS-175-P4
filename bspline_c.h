//
//  bspline_c.h
//  ECS 175 P4
//
//  Created by Nan Chen on 11/26/19.
//  Copyright © 2019 Nan Chen. All rights reserved.
//

#ifndef BSPLINE_C_H
#define BSPLINE_C_H

#include <vector>
#include "utility.h"
#include "glm/glm.hpp"

using namespace std;

class bspline_c
{
public:
	/* class members*/
	int k;
	int n_points;  // number of control points
	vector<glm::vec2> controlP;  // control points of the curve
	vector<float> domain; // the domain of the curve

	/* constructos */
	bspline_c(int k);

	/* member functions*/
	glm::vec2 deBoor(float u); // deBoor algorithm to compute a point given parameter u
	vector<float> cal_domain(); // get the domain of the curve
	int findI(float u); //find the I given u
	void addPoints(glm::vec2 P); // add a control points to the curve
	void deletePoints(int index); // delete a control point given the index
	void decreaseK(); //decrease the k value by 1
	void increaseK(); //increase the k value by 1
	void drawPoints(); // draw the controls points and connect them
	void movePoints(float x, float y, int i); // move the points position
	void drawCurve(int resolution); // draw the bspline curve
};
#endif 

