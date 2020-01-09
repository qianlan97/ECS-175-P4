//
//  utility.cpp
//  ECS 175 P4
//
//  Created by Nan Chen on 11/26/19.
//  Copyright © 2019 Nan Chen. All rights reserved.
//

#include "utility.h"

using namespace std;

//rounding function
int rd(const float a) { return int(a + 0.5); }

//turn pixel(x,y) to (r, g, b) value
void MakePix(int x, int y, float r, float g, float b)
{
	//if it is inside the window
	if (x >= 0 && x < WinW && y >= 0 && y < WinH)
	{
		PixelBuffer[WinW * 3 * y + x * 3] = r;
		PixelBuffer[WinW * 3 * y + x * 3 + 1] = g;
		PixelBuffer[WinW * 3 * y + x * 3 + 2] = b;
	}
}

//draw the string at x, y
void drawstr(float x, float y, const char* format, size_t length)
{
	glRasterPos3f(x, y, 0);
	for (unsigned int i = 0; i < length; i++)
		glutBitmapCharacter(font_style, format[i]);
	glEnd();
}