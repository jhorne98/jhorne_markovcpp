#include <iostream>
#include <map>
#include <set>
#include <random>
#include <vector>
#include <algorithm>
#include "include/tinyxml2.h"

#define DEBUG 0 

// the comparison struct: organize by descending
struct comp
{
	template<typename T>
	bool operator()(const T& l, const T& r) const {
		if(l.second != r.second) {
			return l.second > r.second;
		}

		return true;
	}
};

// the Node class: n transitions to other nodes
class Node 
{
	public:
		std::string name;
		std::map<Node*, double> transitions;	
		std::set<std::pair<Node*, double>, comp> organizedTransitions;

		Node(std::string nodeName) {
			name = nodeName;
		}

		// check if the probability transitions add to 1
		bool checkTransitions() {
			// can't tell you why this needs to be a float but it works
			float total = 0.0;

			std::map<Node*, double>::iterator it;

			for(it = transitions.begin(); it != transitions.end(); it++) {
				if (DEBUG) {std::cout << name << "->" << it->first->name << ":" << it->second << std::endl;}
				total += it->second;
			}

			if (DEBUG) {std::cout << total << std::endl;}
			return total == 1.0;
		}
};

int main()
{
	srand(time(NULL));

	tinyxml2::XMLDocument nodeDoc;
	nodeDoc.LoadFile("nodes.xml");
	tinyxml2::XMLElement* nodesXML = nodeDoc.FirstChildElement("nodes");
	
	std::vector<Node*> nodeMap;

	int moves = nodeDoc.FirstChildElement("moves")->Int64Text();
	std::cout << moves << std::endl;

	// load nodes into nodeMap from "nodes.xml"
	for (tinyxml2::XMLElement* e = nodesXML->FirstChildElement("node");
			e != NULL;
			e = e->NextSiblingElement("node"))
	{
		const char *name = e->FirstChildElement("name")->GetText();
		Node *temp = new Node(name);

		nodeMap.push_back(temp);	
	}
	
	// load transitions and chance for all nodes
	for (tinyxml2::XMLElement* node = nodesXML->FirstChildElement("node");
			node != NULL;
			node = node->NextSiblingElement("node"))
	{
		// get the parent node followed by list of transitions
		std::string parentName = node->FirstChildElement("name")->GetText();
		auto parentNode = std::find_if(nodeMap.begin(), nodeMap.end(), [&parentName](const Node* obj) {return obj->name == parentName;});
		
		for (tinyxml2::XMLElement* transition = node->FirstChild()->NextSiblingElement("transition");
				transition != NULL;
				transition = transition->NextSiblingElement("transition"))
		{
			// iterate over each transition node
			std::string toName = transition->FirstChildElement("to")->GetText();
			auto childNode = std::find_if(nodeMap.begin(), nodeMap.end(), [&toName](const Node* obj) {return obj->name == toName;});

			// add each specified node to transitions
			parentNode[0]->transitions.insert(
			{
				childNode[0],
				transition->FirstChildElement("chance")->DoubleText()
			});
		}
	}

	// start at random node
	Node *current = nodeMap[rand() % nodeMap.size()];

	// check transitions and sort
	for(int i = 0; i < nodeMap.size(); i++) { 
		if(nodeMap[i]->checkTransitions() != 1) {
			std::cout << "ERROR: node " << nodeMap[i]->name << "'s transitions do not sum to 1.0." << std::endl;
			return 1;
		}

		// sort transitions by value
		nodeMap[i]->organizedTransitions = std::set<std::pair<Node*, double>, comp>(nodeMap[i]->transitions.begin(), nodeMap[i]->transitions.end());
	}
	
	float currentPercent;

	// roll and move
	for(int step = 0; step < moves; step++) {
		currentPercent = 0.0;

		// I hate floating point
		double randomDouble = ((double)rand() / (double)RAND_MAX);
		std::cout << current->name << ", " << randomDouble << std::endl;

		// check if each node meets threshold, breaking at find
		for(auto const &pair: current->organizedTransitions) {
			currentPercent += pair.second;

			if(randomDouble <= currentPercent) {
				current = pair.first;
				break;
			} else {
				current = pair.first;
			}
		}
	}

	for(int i = 0; i < nodeMap.size(); i++) {
		delete nodeMap[i];
	}

	return 0;
}
