#pragma once

/* 
This class was thought to allow merge engine and scripting, register the math functions in lua environment and allow to use it in Engine instead of MathGeoLib,
but lua has its own and very complete math library and MathGeoLib does all we need.
*/
class Mathf
{
public:
	Mathf();
	~Mathf();

	float Abs(const float &val);

	float Acos(const float &val);
	float Asin(const float &val);
	float Atan(const float &val);
	float Atan2(const float &y, const float &x);

public:
	const long double PI = 3.141592653589793238L;
	const long double Deg2Rad = (PI * 2.f) / 360.f;
	const long double Rad2Deg = 360.f / (PI * 2.f);
};

