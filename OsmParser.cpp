#pragma once
#include "OsmParser.h"

typedef struct {
	float lat, lon;
} Location;

Location getNodeLocation(XMLElement* e)
{
	Location loc;
	loc.lon = e->FindVariableZ("lon", true)->GetValueFloat(); // x
	loc.lat = e->FindVariableZ("lat", true)->GetValueFloat(); // y
	loc.lon = ((loc.lon - 14.4) * 10000 - 180) * 10; // just so we can draw it on the screen
	loc.lat = ((loc.lat - 50.07) * -10000 + 90) * 10; 

	return loc;
}

Location getNodeLocation(const int id, XML* a)
{
	char tag[100] = {0};
	Location loc;
	
	for (unsigned int i = 0; i < a->GetRootElement()->GetChildrenNum(); ++i) {
		XMLElement* e = a->GetRootElement()->GetChildren()[i];
		e->GetElementName(tag);
		if (strcmp(tag, "node") != 0) continue;

		if (id == e->FindVariableZ("id", true)->GetValueInt()) {
			return getNodeLocation(e);
		}
	}

	throw new exception("Node searched for was not found.");
}

char* getNodeTagValue(XMLElement* e, const char* key)
{
	char attribute[100] = {0};
	e->FindVariableZ("k", true)->GetValue(attribute);
	if (strcmp(attribute, key) != 0)
		throw 1;
			
	e->FindVariableZ("v", true)->GetValue(attribute);
	return attribute;
}

float getDistance(Location l1, Location l2)
{
	Location res;
	res.lat = l1.lat - l2.lat;
	res.lon = l1.lon - l2.lon;
	return sqrt(res.lat * res.lat + res.lon * res.lon);
}

void DrawMap(const char* file, HDC hdc)
{
	
	XML* a = new XML(file);
	a->CompressMemory();

	Graphics graphics(hdc);
	Pen penBlue(Color(255, 0, 0, 255));
	Pen penBlack(Color(255, 0, 0, 0));
	Pen penRed(Color(255, 255, 0, 0));
	
	REAL lon = 0, lat = 0;
	int num = a->GetRootElement()->GetChildrenNum();
	char tag[100] = {0};
	char child[100] = {0};
	int id = 0;
	bool isPath = false;

	Container map;

	// draw nodes
	int start = 523637130, finish = 418956013;
	Node *nStart, *nFinish;

	for (unsigned int i = 0; i < a->GetRootElement()->GetChildrenNum(); ++i) {
		XMLElement* e = a->GetRootElement()->GetChildren()[i];
		e->GetElementName(tag);
		if (strcmp(tag, "node") != 0) continue;

		Location loc = getNodeLocation(e);
		//graphics.DrawEllipse(&penBlue, (int) loc.lon, (int) loc.lat, 3, 3);
		int id = e->FindVariableZ("id", true)->GetValueInt();
		map.newNode(id);
	}

	nStart = map.getNodeById(start);
	nFinish = map.getNodeById(finish);

	// draw ways	
	for (unsigned int i = 0; i < a->GetRootElement()->GetChildrenNum(); ++i) {
		isPath = false;
		XMLElement* e = a->GetRootElement()->GetChildren()[i];
		e->GetElementName(tag);
		if (strcmp(tag, "way") != 0) continue;

		int last_id;
		Location last, current;
		last.lat = -1;

		// skip buildings
		for (unsigned int n = 0; n < e->GetChildrenNum(); ++n) {
			XMLElement* node = e->GetChildren()[n];
			node->GetElementName(child);
			if (strcmp(child, "tag") != 0) continue;
			
			try {
				char* value = getNodeTagValue(node, "highway");
				isPath = strcmp(value, "") != 0;
			} catch (int e) {
				// tag not found
			}
		}
		if (!isPath) continue;

		for (unsigned int n = 0; n < e->GetChildrenNum(); ++n) {
			XMLElement* node = e->GetChildren()[n];
			node->GetElementName(child);
			if (strcmp(child, "nd") != 0) continue;

			id = node->FindVariableZ("ref", true)->GetValueInt();
			current = getNodeLocation(id, a);
			if (last.lat != -1) {
				graphics.DrawLine(&penBlack, last.lon, last.lat, current.lon, current.lat);
				map.newEdge(id, last_id, getDistance(last, current)); // TODO: compute distance
			}
			last = current;
			last_id = id;
		}
	}

	vector<int> moves = map.findRoute(nStart, nFinish);

	// draw start and finish nodes
	{
		Location lStart = getNodeLocation(start, a);
		Location lFinish = getNodeLocation(finish, a);
		int radius = 8;
		graphics.DrawEllipse(&penRed, (int) lStart.lon - radius / 2, (int) lStart.lat - radius / 2, radius, radius);
		graphics.DrawEllipse(&penRed, (int) lFinish.lon - radius / 2, (int) lFinish.lat - radius / 2, radius, radius);
	}

	// draw path
	Location last, current;
	last.lat = -1;
	for (vector<int>::iterator i = moves.begin(); i != moves.end(); ++i) {
		current = getNodeLocation(*i, a);
		if (last.lat != -1) {
			graphics.DrawLine(&penRed, last.lon, last.lat, current.lon, current.lat);
		}
		last = current;
	}
};
