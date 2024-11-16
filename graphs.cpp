#include "graphs.h"
#include <Windows.h>
#include <ctime>
#include <fstream>
using namespace std;


void setConsoleColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}


void resetConsoleColor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);
}


string Graph::get_color_hex(int color) const {
	switch (color) {
		case 0: return "#000000";  // Black
		case 1: return "#0000AA";  // Blue
		case 2: return "#00AA00";  // Green
		case 3: return "#00AAAA";  // Aqua
		case 4: return "#AA0000";  // Red
		case 5: return "#AA00AA";  // Purple
		case 6: return "#AAAA00";  // Yellow
		case 7: return "#AAAAAA";  // White
		case 8: return "#555555";  // Gray
		case 9: return "#5555FF";  // Light Blue
		case 10: return "#55FF55"; // Light Green
		case 11: return "#55FFFF"; // Light Aqua
		case 12: return "#FF5555"; // Light Red
		case 13: return "#FF55FF"; // Light Purple
		case 14: return "#FFFF55"; // Light Yellow
		case 15: return "#FFFFFF"; // Bright White
		default: return "#000000"; // Default to black if color code is out of range
	}
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


void Graph::add_vertex(int id, int color = 15) {
	for (const auto& vertex : V) {
		if (vertex.id == id) {
			cout << "Vertex " << id << " already exists in the graph." << endl;
			return;
		}
	}
	V.emplace_back(id, color);
	size_t newSize = max(id + 1, adj.size());
	for (auto& row : adj) {
		row.resize(newSize, false); // Extend existing rows
	}
	adj.resize(newSize, vector<bool>(newSize, false)); // Add new rows
}


void Graph::add_edge(int u, int v, int color) {
	int maxVertex = max(u, v);
	if (maxVertex >= adj.size()) {
		adj.resize(maxVertex + 1, vector<bool>(maxVertex + 1, false));
	}
	adj[u][v] = true;
	adj[v][u] = true;
	E.emplace_back(u, v, color);
}


void Graph::print() const {
	bool first = true;
	cout << "V(G): {";
	for (const Vertex& v : V) {
		if (!first) {
			cout << ", ";
		}
		v.print();
		first = false;
	}
	cout << "}" << endl;
	cout << "--------------------------------------------------------------------------------------------------------------" << endl;
	first = true;
	cout << "E(G): {";
	for (const Edge& e : E) {
		if (!first) {
			cout << ", ";
		}
		e.print();
		first = false;
	}
	cout << "}" << endl;
	cout << "--------------------------------------------------------------------------------------------------------------" << endl;
}


void Vertex::print_to_file(ofstream& outputFile, string color) const {
	outputFile << "<span style='color:" << color << "'>" << id << "</span>";
}


void Edge::print_to_file(ofstream& outputFile, string color) const {
	outputFile << "<span style='color:" << color << "'>(" << v1 << "," << v2 << ")</span>";
}


void Graph::print_to_file(const string& filename) const {
	ofstream outputFile(filename);
	if (!outputFile.is_open()) {
		cerr << "Error opening file!" << endl;
		return;
	}

	outputFile << "<html><body>" << endl;

	// Print vertices
	outputFile << "<h3>Vertices:</h3>{";
	bool firstVertex = true;
	for (const Vertex& v : V) {
		if (!firstVertex) {
			outputFile << ", ";
		}
		v.print_to_file(outputFile, get_color_hex(v.color));
		firstVertex = false;
	}
	outputFile << "}<br>" << endl;

	// Print edges
	outputFile << "<h3>Edges:</h3>{";
	bool firstEdge = true;
	for (const Edge& e : E) {
		if (!firstEdge) {  
			outputFile << ", ";
		}
		e.print_to_file(outputFile, get_color_hex(e.color));
		firstEdge = false;
	}
	outputFile << "}" << endl;

	outputFile << "</body></html>" << endl;
	outputFile.close();
}


void Graph::gen_rand_graph(int n, float p) {
	adj = vector<vector<bool>>(n, vector<bool>(n, false));

	for (int i = 0; i < n; i++) {
		add_vertex(i);
		for (int j = i + 1; j < n; j++) {
			if (float(rand()) / RAND_MAX < p) {
				add_edge(i, j);
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
	for (int i = 0; i < n; i++) {										// Search for Triangles
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


bool Graph::has_k4() const {
	int n = V.size();
	for (int i = 0; i < n - 3; ++i) {
		for (int j = i + 1; j < n - 2; ++j) {
			for (int k = j + 1; k < n - 1; ++k) {
				for (int l = k + 1; l < n; ++l) {							// Search for K4									
					if (adj[i][j] && adj[i][k] && adj[i][l] &&
						adj[j][k] && adj[j][l] && adj[k][l]) {
						return true;  
					}
				}
			}
		}
	}
	return false;  
}