#ifndef _GE_MATH_H
#define _GE_MATH_H
#include <math.h>

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

		Vec3f euler;
		float sy = rotX_z/*rows[0][2]*/;
		if (sy < (1.0f - (float)CMP_EPSILON)) {
			if (sy > -(1.0f - (float)CMP_EPSILON)) {
				// is this a pure Y rotation?
				if (rotY_x/*rows[1][0]*/ == 0 && rotX_y/*rows[0][1]*/ == 0 && rotY_z/*rows[1][2]*/ == 0 && basisZ_y/*rows[2][1]*/ == 0 && rotY_y/*rows[1][1]*/ == 1) {
					// return the simplest form (human friendlier in editor and scripts)
					euler.x = 0;
					euler.y = atan2(basisX_z/*rows[0][2]*/, basisX_x/*rows[0][0]*/);
					euler.z = 0;
				}
				else {
					euler.x = atan2(-basisY_z/*rows[1][2]*/, basisZ_z/*rows[2][2]*/);
					euler.y = asin(sy);
					euler.z = atan2(-basisX_y/*rows[0][1]*/, basisX_x/*rows[0][0]*/);
				}
			}
			else {
				euler.x = atan2(basisZ_y/*rows[2][1]*/, basisY_y/*rows[1][1]*/);
				euler.y = -Math_PI / 2.0f;
				euler.z = 0.0f;
			}
		}
		else {
			euler.x = atan2(basisZ_y/*rows[2][1]*/, basisY_y/*rows[1][1]*/);
			euler.y = Math_PI / 2.0f;
			euler.z = 0.0f;
		}

		return euler;

		//float sy = get_scale_y();
		////float sy = sqrtf(rotX_x * rotX_x + rotY_x * rotY_x);

		//bool singular = sy < 1e-6; // If

		//float x, y, z;
		//if (!singular)
		//{
		//	x = atan2(rotZ_y, rotZ_z);
		//	y = atan2(-rotZ_x, sy);
		//	z = atan2(rotY_x, rotX_x);
		//}
		//else
		//{
		//	x = atan2(-rotY_z, rotY_y);
		//	y = atan2(-rotZ_x, sy);
		//	z = 0;
		//}
		//return Vec3f{ x, y, z };
	}
	/*Vec3f get_rotation()
	{
		float sy = sqrt(basisX_x * basisX_x + basisX_y * basisX_y);

		bool singular = sy < 1e-6; // If

		float x, y, z;

		if (!singular)
		{

			x = atan2(basisZ_y, basisZ_z);

			y = atan2(-basisZ_x, sy);

			z = atan2(basisY_x, basisX_x);

		}
		else
		{

			x = atan2(-basisY_z, basisY_y);

			y = atan2(-basisZ_x, sy);

			z = 0;

		}
		return Vec3f{ x, y, z };

	}*/


};

#endif //_GE_MATH_H