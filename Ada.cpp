#include "Ada.h"

const float Ada::wheel_distance = 0.450;

Ada::Ada(void)
{
	velocity.first = velocity.second = 1;
	orientation = M_PI / 2; // read from onboard compass
	location.latitude = 0; // read from GPS
	location.longitude = 0;
}


void Ada::step(unsigned const int delta)
{
	float smaller = velocity.first > velocity.second ? velocity.first : velocity.second;
	float larger = velocity.first > velocity.second ? velocity.second : velocity.first;

	orientation += atan((larger - smaller) / wheel_distance);

	location.latitude -= meter2lat(smaller * sin(orientation));
	location.longitude -= meter2lon(smaller * cos(orientation));
}

Ada::~Ada(void)
{
}
