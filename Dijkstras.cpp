#include "Dijkstras.h"

void Container::compute()
{
	while (nodes.size() > 0)
	{
		Node* smallest = ExtractSmallest(nodes);
		vector<Node*>* adjacentNodes = AdjacentRemainingNodes(smallest);

		for (unsigned int i = 0; i < adjacentNodes->size(); ++i)
		{
			Node* adjacent = adjacentNodes->at(i);
			float distance = Distance(smallest, adjacent) + smallest->distanceFromStart;
			
			if (distance < adjacent->distanceFromStart)
			{
				adjacent->distanceFromStart = distance;
				adjacent->previous = smallest;
			}
		}
		delete adjacentNodes;
	}
}

Node* Container::ExtractSmallest(vector<Node*>& nodes)
{
	int size = nodes.size();
	if (size == 0) return NULL;
	int smallestPosition = 0;
	Node* smallest = nodes.at(0);
	for (int i = 1; i < size; ++i) {
		Node* current = nodes.at(i);
		if (current->distanceFromStart < smallest->distanceFromStart) {
			smallest = current;
			smallestPosition = i;
		}
	}
	nodes.erase(nodes.begin() + smallestPosition);
	return smallest;
}

vector<Node*>* Container::AdjacentRemainingNodes(Node* node)
{
	vector<Node*>* adjacentNodes = new vector<Node*>();
	for (unsigned int i = 0; i < edges.size(); ++i) {
		Edge* edge = edges.at(i);
		Node* adjacent = NULL;
		
		if (edge->node1 == node)
			adjacent = edge->node2;
		else if (edge->node2 == node)
			adjacent = edge->node1;
		
		if (adjacent && Contains(nodes, adjacent))
			adjacentNodes->push_back(adjacent);
	}
	return adjacentNodes;
}

float Container::Distance(Node* node1, Node* node2)
{
	const int size = edges.size();
	for(int i=0; i<size; ++i)
	{
		Edge* edge = edges.at(i);
		if (edge->Connects(node1, node2))
		{
			return edge->distance;
		}
	}
	return -1; // should never happen
}

bool Container::Contains(vector<Node*>& nodes, Node* node)
{
	const int size = nodes.size();
	for (int i = 0; i < size; ++i) {
		if (node == nodes.at(i)) {
			return true;
		}
	}
	return false;
}

vector<int> Container::findRoute(Node* start, Node* destination)
{
	start->distanceFromStart = 0;
	this->compute();
	vector<int> nodes;
	Node* previous = destination;
	while (previous) {
		nodes.push_back(previous->id);
		previous = previous->previous;
	}
	return nodes;
}




// these two not needed
/*
vector<Edge*>* AdjacentEdges(vector<Edge*>& Edges, Node* node);
void RemoveEdge(vector<Edge*>& Edges, Edge* edge);

vector<Edge*>* AdjacentEdges(vector<Edge*>& edges, Node* node)
{
	vector<Edge*>* adjacentEdges = new vector<Edge*>();

	const int size = edges.size();
	for(int i=0; i<size; ++i)
	{
		Edge* edge = edges.at(i);
		if (edge->node1 == node)
		{
			cout << "adjacent: " << edge->node2->id << endl;
			adjacentEdges->push_back(edge);
		}
		else if (edge->node2 == node)
		{
			cout << "adjacent: " << edge->node1->id << endl;
			adjacentEdges->push_back(edge);
		}
	}
	return adjacentEdges;
}

void RemoveEdge(vector<Edge*>& edges, Edge* edge)
{
	vector<Edge*>::iterator it;
	for (it=edges.begin(); it<edges.end(); ++it)
	{
		if (*it == edge)
		{
			edges.erase(it);
			return;
		}
	}
}
*/
