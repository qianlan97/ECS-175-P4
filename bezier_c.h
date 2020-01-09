//
//  bezier_c.h
//  ECS 175 P4
//
//  Created by Nan Chen on 11/26/19.
//  Copyright © 2019 Nan Chen. All rights reserved.
//

#ifndef BEZIER_C_H
#define BEZIER_C_H

#include <vector>
#include "utility.h"
#include "glm/glm.hpp"

using namespace std;

class bezier_c
{
public:
	/* class members*/
	int n_points;  // number of control points
	vector<glm::vec2> controlP;  // control points of the curve

	/* constructos */
	bezier_c(); //default constructor

	/* member functions*/
	glm::vec2 deCasteljau(float t); // de Casteljau algorithm to compute a point given parameter t
	void addPoints(glm::vec2 P); // add a control points to the curve
	void deletePoints(int index); // delete the point given the index 
	void drawPoints(); // draw the controls points and connect them
	void movePoints(float x, float y, int i); // move the points position
	void drawCurve(int resolution); // draw the bezier curve
	
};

#endif
