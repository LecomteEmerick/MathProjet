#pragma once
class Color
{
public:
	Color();
	Color(float, float, float);
	float red_get();
	float green_get();
	float blue_get();
	~Color();
private:
	float red_;
	float green_;
	float blue_;
};
