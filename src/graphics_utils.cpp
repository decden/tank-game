#include "graphics_utils.h"

#include <cmath>
#include <GL/gl.h>

void GraphicsUtils::setClearColor(unsigned int hex)
{
	int r = (hex & 0xFF000000) >> 24;
	int g = (hex & 0x00FF0000) >> 16;
	int b = (hex & 0x0000FF00) >>  8;
	int a = (hex & 0x000000FF) >>  0;

	glClearColor(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

void GraphicsUtils::setColor(unsigned int hex)
{
	int r = (hex & 0xFF000000) >> 24;
	int g = (hex & 0x00FF0000) >> 16;
	int b = (hex & 0x0000FF00) >>  8;
	int a = (hex & 0x000000FF) >>  0;

	glColor4f(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

void GraphicsUtils::fillRectangle(float x, float y, float w, float h, float z)
{
	float x2 = x + w;
	float y2 = y + h;

	glBegin(GL_TRIANGLE_FAN);
	
		glVertex3f(x,  y,  z);
		glVertex3f(x,  y2, z);
		glVertex3f(x2, y2, z);
		glVertex3f(x2, y,  z);

	glEnd();
}

void GraphicsUtils::fillCircle(float x, float y, float r, int segments, float z)
{
	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < segments; ++i)
	{
		float vx = x + std::cos((float)i / segments * (2*M_PI)) * r;
		float vy = y + std::sin((float)i / segments * (2*M_PI)) * r;

		glVertex3f(vx, vy, z);
	}

	glEnd();
}

void GraphicsUtils::fillCone(float x, float y, float r, int segments, float angle, float range, float z)
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, z);

	for (int i = 0; i <= segments; ++i)
	{
		float vangle = (float)i / segments * range;
		vangle -= range / 2.0f + angle;

		float vx = x + std::cos(vangle) * r;
		float vy = y + std::sin(vangle) * r;

		glVertex3f(vx, vy, z);
	}

	glEnd();
}

void GraphicsUtils::drawCircle(float x, float y, float r, int segments, float z)
{
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < segments; ++i)
	{
		float vx = x + std::cos((float)i / segments * (2*M_PI)) * r;
		float vy = y + std::sin((float)i / segments * (2*M_PI)) * r;

		glVertex3f(vx, vy, z);
	}

	glEnd();
}

void GraphicsUtils::drawLine(float x, float y, float x2, float y2, float z)
{
	glBegin(GL_LINES);

	glVertex3f(x, y, z);
	glVertex3f(x2, y2, z);

	glEnd();
}