#ifndef _GE_MATH_H
#define _GE_MATH_H
#include <math.h>
#include <algorithm>

#define Math_PI 3.1415926535897932384626433833
#define CMP_EPSILON 0.00001

struct Vec3f
{
	float x, y, z;
};

struct BoneMatrix
{
	float basisX_x; //0,0
	float basisX_y; //0,1
	float basisX_z; //0,2

	float basisY_x; //1,0
	float basisY_y; //1,1
	float basisY_z; //1,2

	float basisZ_x; //2,0
	float basisZ_y; //2,1
	float basisZ_z; //2,2

	float translation_x;
	float translation_y;
	float translation_z;

	float get_scale_x()
	{
		return sqrtf(basisX_x * basisX_x + basisX_y * basisX_y + basisX_z * basisX_z);
	}
	float get_scale_y()
	{
		return sqrtf(basisY_x * basisY_x + basisY_y * basisY_y + basisY_z * basisY_z);
	}
	float get_scale_z()
	{
		return sqrtf(basisZ_x * basisZ_x + basisZ_y * basisZ_y + basisZ_z * basisZ_z);
	}


	Vec3f get_rotation()
	{
		float rotX_x = basisX_x / get_scale_x(); //0,0
		float rotX_y = basisX_y / get_scale_x(); //0,1
		float rotX_z = basisX_z / get_scale_x(); //0,2

		float rotY_x = basisY_x / get_scale_y(); //1,0
		float rotY_y = basisY_y / get_scale_y(); //1,1
		float rotY_z = basisY_z / get_scale_y(); //1,2

		float rotZ_x = basisZ_x / get_scale_z(); //2,0
		float rotZ_y = basisZ_y / get_scale_z(); //2,1
		float rotZ_z = basisZ_z / get_scale_z(); //2,2

		//setFromRotationMatrix(m, order = this._order, update = true) {
		// assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

		float x;
		float y;
		float z;

		float m11 = rotX_x, m12 = rotY_x, m13 = rotZ_x;
		float m21 = rotX_y, m22 = rotY_y, m23 = rotZ_y;
		float m31 = rotX_z, m32 = rotY_z, m33 = rotZ_z;

		//XYZ:
		y = asin(std::clamp(m13, -1.f, 1.f));

		if (abs(m13) < 0.9999999) {

			x = atan2(-m23, m33);
			z = atan2(-m12, m11);

		}
		else {

			x = atan2(m32, m22);
			z = 0;

		}
		return Vec3f{ x, y, z };
	}
	
};

#endif //_GE_MATH_H