#ifndef __GRAPHICS_UTILS_H__
#define __GRAPHICS_UTILS_H__

class GraphicsUtils
{
public:
	static void setClearColor(unsigned int color);
	static void setColor(unsigned int color);

	static void fillRectangle(float x, float y, float w, float h, float z);
	static void fillCircle(float x, float y, float r, int segments, float z);
	static void fillCone(float x, float y, float r, int segments, float angle, float range, float z);

	static void drawCircle(float x, float y, float r, int segments, float z);
	static void drawLine(float x, float y, float x2, float y2, float z);

private:
	GraphicsUtils() {};
	~GraphicsUtils() {};
};

#endif // __GRAPHICS_UTILS_H__