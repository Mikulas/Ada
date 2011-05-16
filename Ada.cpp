#include "Ada.h"

const float Ada::wheel_distance = 0.450f;
const float Ada::node_achieved_distance = 0.5f;

Ada::Ada(void)
{
	velocity.first = velocity.second = 1;
	orientation = 0; // TODO: read from onboard compass
	location.latitude = 0; // TODO: read from GPS
	location.longitude = 0;
}


float Ada::getDistanceToNext()
{
	Location next = track.top();

	float dif_lat = lat2meter(next.latitude - location.latitude);
	float dif_lon = lon2meter(next.longitude - location.longitude);
	return sqrt(dif_lat * dif_lat + dif_lon * dif_lon);
}


void Ada::rotate()
{
	Location next = track.top();

	float dif_lat = lat2meter(next.latitude - location.latitude);
	float dif_lon = lon2meter(next.longitude - location.longitude);

	// WAIT for motor
	/*
	 * motor magic here,
	 * but for now this will do:
	 */
	// orientation += atan((larger - smaller) / wheel_distance);
	float new_orientation = atan(dif_lon / dif_lat);
	if (dif_lat < 0) new_orientation += (float) M_PI;

	// TOOD: make it constant somewhere
	float rounding = (float) M_PI / 180; // a degree
	do {
		float motor_step = (float) M_PI / 90; // this is to be computed
		if (orientation < new_orientation)
			orientation += motor_step;
		else
			orientation -= motor_step;
	} while (orientation < new_orientation - rounding || orientation > new_orientation + rounding);
}


void Ada::step(const float delta)
{
	float smaller = velocity.first > velocity.second ? velocity.first : velocity.second;
	float larger = velocity.first > velocity.second ? velocity.second : velocity.first;

	if (track.empty()) {
		velocity.first = velocity.second = 0;
		return;
	}

	float res = getDistanceToNext();
	if (getDistanceToNext() < node_achieved_distance) {
		track.pop();
		if (track.empty()) return;
	}

	rotate();

	location.latitude += meter2lat(delta * smaller * (float) cos(orientation));
	location.longitude += meter2lon(delta * smaller * (float) sin(orientation));
}

Ada::~Ada(void)
{
}
