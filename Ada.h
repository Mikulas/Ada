#pragma once
#define _USE_MATH_DEFINES

#include <utility>
#include <math.h>
#include "Location.h"

using namespace std;

/*
 * ADA: Automated Devivery Android
 */
class Ada
{
public: //protected: // TODO: DEBUG public only, revert asap
	static const float wheel_distance; // [meters]

	Location location;
	pair<float, float> velocity; // [meters/second]
	double orientation; // [rad] where 0 means Ada is looking north, π/2 east, π south and 3π/4 west

public:
	Ada(void);

	void step(unsigned const int delta);

	//void syncGpsLocation();
	~Ada(void);
};

