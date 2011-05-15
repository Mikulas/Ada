#pragma once

#include "Constants.h"

typedef struct {
	float latitude, longitude;  // [degrees]
} Location;

int lon2screen(float);
int lat2screen(float);

float meter2lat(float);
float meter2lon(float);
float lat2meter(float);
float lon2meter(float);
