#pragma once
#define _USE_MATH_DEFINES

#include <utility>
#include <math.h>
#include <stack>
#include "Location.h"

using namespace std;

/*
 * ADA: Automated Devivery Android
 */
class Ada
{
// TODO: DEBUG public only, revert asap
public: //protected:
	static const float wheel_distance; // [meters]
	static const float node_achieved_distance; // [meters]

	Location location;
	pair<float, float> velocity; // [meters/second]
	double orientation; // [rad] where 0 means Ada is looking north, π/2 east, π south and 3π/2 west
	stack<Location> track;

	float getDistanceToNext();

public:
	Ada(void);

	void step(const float delta);

	void rotate();

	//void syncGpsLocation();
	~Ada(void);
};

