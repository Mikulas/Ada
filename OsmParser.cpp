#pragma once
#include "OsmParser.h"

Location getNodeLocation(XMLElement* e)
{
	Location loc;
	loc.longitude = e->FindVariableZ("lon", true)->GetValueFloat(); // x
	loc.latitude = e->FindVariableZ("lat", true)->GetValueFloat(); // y
	return loc;
}

Location getNodeLocation(const int id, XML* a)
{
	char tag[100] = {0};
	
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
	res.latitude = l1.latitude - l2.latitude;
	res.longitude = l1.longitude - l2.longitude;
	return sqrt(res.latitude * res.latitude + res.longitude * res.longitude);
}

void DrawMap(const char* file, HDC hdc, Ada *ada)
{
	XML* a = new XML(file);
	a->CompressMemory();

	Graphics graphics(hdc);
	Pen penBlue(Color(255, 0, 0, 255));
	Pen penBlack(Color(255, 0, 0, 0));
	Pen penRed(Color(255, 255, 0, 0));
	
	REAL longitude = 0, latitude = 0;
	int num = a->GetRootElement()->GetChildrenNum();
	char tag[100] = {0};
	char child[100] = {0};
	int id = 0;
	bool isPath = false;

	Container map;

	// draw nodes
	int start = 523637130, finish = 418956013; // TODO: these are not to be hardcoded
	Node *nStart, *nFinish;

	for (unsigned int i = 0; i < a->GetRootElement()->GetChildrenNum(); ++i) {
		XMLElement* e = a->GetRootElement()->GetChildren()[i];
		e->GetElementName(tag);
		if (strcmp(tag, "node") != 0) continue;

		Location loc = getNodeLocation(e);
		//graphics.DrawEllipse(&penBlue, (int) loc.longitude, (int) loc.latitude, 3, 3);
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
		last.latitude = -1;

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
			if (last.latitude != -1) {
				graphics.DrawLine(&penBlack, lon2screen(last.longitude), lat2screen(last.latitude), lon2screen(current.longitude), lat2screen(current.latitude));
				map.newEdge(id, last_id, getDistance(last, current));
			}
			last = current;
			last_id = id;
		}
	}

	vector<int> moves = map.findRoute(nStart, nFinish);

	// draw path
	Location last, current;
	last.latitude = -1;
	for (vector<int>::iterator i = moves.begin(); i != moves.end(); ++i) {
		current = getNodeLocation(*i, a);
		if (last.latitude != -1) {
			graphics.DrawLine(&penRed, lon2screen(last.longitude), lat2screen(last.latitude), lon2screen(current.longitude), lat2screen(current.latitude));
		}
		last = current;
	}

	// draw start and finish nodes
	{
		Location lStart = getNodeLocation(start, a);
		ada->location.latitude = lStart.latitude;
		ada->location.longitude = lStart.longitude;
		Location lFinish = getNodeLocation(finish, a);
		int radius = 8;
		graphics.DrawEllipse(&penRed, lon2screen(lStart.longitude) - radius / 2, (int) lat2screen(lStart.latitude) - radius / 2, radius, radius);
		graphics.DrawEllipse(&penRed, lon2screen(lFinish.longitude) - radius / 2, (int) lat2screen(lFinish.latitude) - radius / 2, radius, radius);
	}
};
