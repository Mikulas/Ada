#include "Ada.h"

const float Ada::wheel_distance = 0.450;
const float Ada::node_achieved_distance = 1.5;

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

	orientation = M_PI + atan(dif_lon / dif_lat);
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

	location.latitude += meter2lat(delta * smaller * cos(orientation));
	location.longitude += meter2lon(delta * smaller * sin(orientation));
}

Ada::~Ada(void)
{
}
