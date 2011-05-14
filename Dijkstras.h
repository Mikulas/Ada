#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Node;
class Edge;
class Container;

/*
void Dijkstras();
vector<Node*>* AdjacentRemainingNodes(Node* node);
Node* ExtractSmallest(vector<Node*>& nodes);
int Distance(Node* node1, Node* node2);
bool Contains(vector<Node*>& nodes, Node* node);
void PrintShortestRouteTo(Node* destination);
*/

class Node
{
public:
	Node(unsigned int id) : id(id), previous(NULL), distanceFromStart(INT_MAX)
	{
		
	}

	unsigned int id;
	Node* previous;
	int distanceFromStart;
};


class Edge
{
public:
	Node* node1;
	Node* node2;
	float distance;

	Edge(Node* node1, Node* node2, float distance) : node1(node1), node2(node2), distance(distance)
	{
		
	}

	bool Connects(Node* node1, Node* node2)
	{
		return ((node1 == this->node1 && node2 == this->node2)
			 || (node1 == this->node2 && node2 == this->node1));
	}
};

class Container
{
protected:
	Node* ExtractSmallest(vector<Node*>& nodes);
	vector<Node*>* AdjacentRemainingNodes(Node* node);
	int Distance(Node* node1, Node* node2);
	bool Contains(vector<Node*>& nodes, Node* node);
	void compute();

public:
	Node* getNodeById(const unsigned int id) {
		for (vector<Node*>::iterator i = this->nodes.begin(); i != this->nodes.end(); ++i) {
			if ((*i)->id == id) {
				return *i;
			}
		}
	}
	
	vector<Node*> nodes;
	vector<Edge*> edges;
	void newNode(unsigned int id) {
		this->nodes.push_back(new Node(id));
	}
	void newEdge(unsigned int id1, unsigned int id2, int distance) {
		this->edges.push_back(new Edge(this->getNodeById(id1), this->getNodeById(id2), distance));
	}
	vector<int> findRoute(Node* start, Node* end);
};
