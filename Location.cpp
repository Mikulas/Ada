#include "Location.h"

int lon2screen(float longitude)
{
	return lon2meter(longitude * 100000 - 1441800) / 100000 * 1.8;
}

int lat2screen(float latitude)
{
	return lat2meter(latitude * -100000 + 5007900) / 100000 * 1.8;
}

float meter2lat(float meter)
{
	return meter / LATITUDE_DEGREE;
}

float meter2lon(float meter)
{
	return meter / LONGITUDE_DEGREE;
}

float lat2meter(float latitude)
{
	return LATITUDE_DEGREE * latitude;
}

float lon2meter(float longitude)
{
	return LONGITUDE_DEGREE * longitude;
}
