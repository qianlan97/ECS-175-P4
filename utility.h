//
//  utility.h
//  ECS 175 P4
//
//  Created by Nan Chen on 11/26/19.
//  Copyright © 2019 Nan Chen. All rights reserved.
//

#ifndef UTIL_H
#define UTIL_H
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

//global variable
extern float* PixelBuffer;
extern int WinW, WinH;
extern GLvoid *font_style; //font style

int rd(const float a); //function for rounding
void MakePix(int x, int y, float r, float g, float b); //makepixel function
void drawstr(float x, float y, const char* format, size_t length); 

#endif
