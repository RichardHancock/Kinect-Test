#include "Vec3.h"
#include <math.h>

Vec3::Vec3() {}
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vec3::getLength()
{
	Vec3 temp;
	temp.x = pow(x, 2);
	temp.y = pow(y, 2);
	temp.z = pow(z, 2);
	float total = temp.x + temp.y + temp.z;

	return sqrt(total);
}

Vec3 Vec3::normalize()
{
	float length = getLength();
	return Vec3(x / length, y / length, z / length);
}