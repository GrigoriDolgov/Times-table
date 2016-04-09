#pragma once
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <string>
#include <algorithm>
#include <sstream>
#include <deque>

#ifndef LOGIC_H_INCLUDED
#define LOGIC_H_INCLUDED

namespace tmt {

template < typename T > std::string ToString( const T& n ) {
  std::ostringstream stm ;
  stm << n ;
  return stm.str() ;
}

void move(std::deque<std::pair<double, double> > &p, int u)
{
    if(u==0 || p.size()==1) return;
    if (u < 0) {
      for (int i = 0; i < -u; i++) {
        std::pair<double, double> temp = p.back();
        p.pop_back();
        p.push_front(temp);
      }
    } else {
      for (int i=0; i<u; i++) {
        std::pair<double, double> temp = p.front();
        p.pop_front();
        p.push_back(temp);
      }
    }
}

void Render(void)
{
	glutSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void DrawBitmapString(
	float x,
	float y,
	void *font,
	std::string string) {
	glRasterPos2f(x, y);
	for (unsigned int i = 0; i < string.size(); i++) {
		glutBitmapCharacter(font, string[i]);
	}
}

void DrawBitmapText(
	float x,
	float y,
	void *font,
	std::string string) {

	unsigned int to=string.find("\n");
	if(to == std::string::npos)
        to = string.size();
    else
        to++;

    glRasterPos2f(x, y);

	for (unsigned int i = 0; i < to-1; i++) {
		glutBitmapCharacter(font, string[i]);
	}

	if(to != string.size())
        DrawBitmapText(x, y+15, font, string.substr(to, string.size()-to));
}

} //tmt

#endif // LOGIC_H_INCLUDED
