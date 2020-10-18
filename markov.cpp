#include <iostream>
#include <map>
#include <set>
#include <random>

#define MOVES 100
#define DEBUG 0 

// the comparison struct: organize by descending
struct comp {
	template<typename T>
	bool operator()(const T& l, const T& r) const {
		if(l.second != r.second) {
			return l.second > r.second;
		}

		return true;
	}
};

// the Node class: n transitions to other nodes
class Node {
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


int main() {
	srand(time(NULL));

	// TODO XML map of Nodes to be read in during runtime
	Node *nodeA = new Node("A");
	Node *nodeB = new Node("B");
	Node *nodeC = new Node("C");

	Node *nodeMap[] = {nodeA, nodeB, nodeC};
	int nodeMapSize = sizeof(nodeMap)/sizeof(nodeMap[0]);

	// test data
	nodeA->transitions.insert({nodeA, 0.5});
	nodeA->transitions.insert({nodeB, 0.2});
	nodeA->transitions.insert({nodeC, 0.3});

	nodeB->transitions.insert({nodeA, 0.1});
	nodeB->transitions.insert({nodeB, 0.1});
	nodeB->transitions.insert({nodeC, 0.8});

	nodeC->transitions.insert({nodeA, 0.6});
	nodeC->transitions.insert({nodeB, 0.1});
	nodeC->transitions.insert({nodeC, 0.3});

	// start at first node in the map
	Node *current = nodeMap[1];

	// check transitions and sort
	for(int i = 0; i < nodeMapSize; i++) { 
		if(nodeMap[i]->checkTransitions() != 1) {
			std::cout << "ERROR: node " << nodeMap[i]->name << "'s transitions do not sum to 1.0." << std::endl;
			return 1;
		}

		// sort transitions by value
		nodeMap[i]->organizedTransitions = std::set<std::pair<Node*, double>, comp>(nodeMap[i]->transitions.begin(), nodeMap[i]->transitions.end());
	}

	// this doesn't work unless >=c++11 so naw
	/*
	// https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c
	double lowerBound = 0;
	double upperBound = 100;
	uniform_real_distribution<double> unif(lowerBound,upperBound);
	default_random_engine re;
	double randomDouble = unif(re);
	*/

	float currentPercent;

	// roll and move
	for(int step = 0; step < MOVES; step++) {
		currentPercent = 0.0;

		// I hate floating point
		double randomDouble = ((double)rand() / (double)RAND_MAX);
		std::cout << current->name << ", " << randomDouble << std::endl;

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

	for(int i = 0; i < nodeMapSize; i++) {
		delete nodeMap[i];
	}

	return 0;
}
