#include "math_utils.h"

float rad2deg(float rad)
{
	return rad / M_PI * 180.0f;
}

float deg2rad(float deg)
{
	return deg / 180.0f * M_PI;
}
