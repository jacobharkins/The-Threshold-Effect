#include "graphs.h"
#include <Windows.h>
#include <ctime>
using namespace std;

void setConsoleColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}


void resetConsoleColor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15); // Default color (white)
}


void Edge::print() const {
	setConsoleColor(color);
	cout << "(" << v1 << "," << v2 << ")";
	resetConsoleColor();
}


void Vertex::print() const {
	setConsoleColor(color);
	cout << id;
	resetConsoleColor();
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


void Graph::gen_rand_colors() {
	for (int i = 0; i < size(E); i++) {
		E[i].color = rand() % 14 + 1;
	}
	for (int i = 0; i < size(V); i++) {
		V[i].color = rand() % 14 + 1;
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
