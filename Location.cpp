#include "Location.h"

int lon2screen(float longitude)
{
	return ((longitude - 14.4) * 10000 - 180) * 10;
}

int lat2screen(float latitude)
{
	return ((latitude - 50.07) * -10000 + 90) * 10;
}

float meter2lat(float meter)
{
	return meter / LATITUDE_DEGREE;
}

float meter2lon(float meter)
{
	return meter / LONGITUDE_DEGREE;
}
