#include "graphs.h"
#include <ctime>
using namespace std;


void Edge::print() const {
	cout << "(" << v1 << "," << v2 << ")";
}


void Vertex::print() const {
	cout << id;
}


void Graph::print() const {
	cout << "V(G): {";
	for (const Vertex& v : V) {
		v.print();
		cout << ", ";
	}
	cout << "}" << endl;
	cout << "--------------------------------------------------------------------------------------------------------------" << endl;

	cout << "E(G): {";
	for (const Edge& e : E) {
		e.print();
		cout << ",";
	}
	cout << "}" << endl;
	cout << "--------------------------------------------------------------------------------------------------------------" << endl;
	cout << "Has an edge? " << has_edge() << endl;
	cout << "Has a triangle subgraph? " << has_triangle() << endl;
	cout << endl << "##########################################################################" << endl;
}


void Graph::gen_rand_graph(int n, float p) {
	adj = vector<vector<bool>>(n, vector<bool>(n, false));

	for (int i = 0; i < n; i++) {
		V.push_back(Vertex(i));
		for (int j = i + 1; j < n; j++) {
			if (float(rand()) / RAND_MAX < p) {
				E.push_back(Edge(i, j));
				adj[i][j] = true;
				adj[j][i] = true;
			}
		}
	}
}


bool Graph::has_edge() const {
	return bool(size(E));
}

bool Graph::has_triangle() const {
	int n = V.size();
	for (int i = 0; i < n; i++) {									// Search for Triangles
		for (int j = i + 1; j < n; j++) {
			if (adj[i][j]) {
				for (int k = j + 1; k < n; k++) {
					if (adj[i][k] && adj[j][k]) {
						return true;
					}
				}
			}
		}
	}
	return false;
}
