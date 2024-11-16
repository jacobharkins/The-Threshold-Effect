#include "graphs.h"
#include <Windows.h>
#include <ctime>
#include <fstream>



void setConsoleColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}


void resetConsoleColor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);
}


std::string Graph::get_color_hex(int color) const {
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
	std::cout << "(" << v1 << "," << v2 << ")";
	resetConsoleColor();
}


void Vertex::print() const {
	setConsoleColor(color);
	std::cout << id;
	resetConsoleColor();
}


void Graph::print() const {
	bool first = true;
	std::cout << "V(G): {";
	for (const Vertex& v : V) {
		if (!first) {
			std::cout << ", ";
		}
		v.print();
		first = false;
	}
	std::cout << "} \n";
	std::cout << "-------------------------------------------------------------------------------------------------------------- \n";
	first = true;
	std::cout << "E(G): {";
	for (const Edge& e : E) {
		if (!first) {
			std::cout << ", ";
		}
		e.print();
		first = false;
	}
	std::cout << "}\n";
	std::cout << "--------------------------------------------------------------------------------------------------------------" << std::endl;
}


void Graph::add_vertex(int id, int color = 15) {
	for (const auto& vertex : V) {
		if (vertex.id == id) {
			std::cout << "Vertex " << id << " already exists in the graph." << std::endl;
			return;
		}
	}
	V.emplace_back(id, color);
	size_t newSize = max(id + 1, adj.size());
	for (auto& row : adj) {
		row.resize(newSize, false); // Extend existing rows
	}
	adj.resize(newSize, std::vector<bool>(newSize, false)); // Add new rows
}


void Graph::add_edge(int u, int v, int color) {
	int maxVertex = max(u, v);
	if (maxVertex >= adj.size()) {
		adj.resize(maxVertex + 1, std::vector<bool>(maxVertex + 1, false));
	}
	adj[u][v] = true;
	adj[v][u] = true;
	E.emplace_back(u, v, color);
}


void Vertex::print_to_file(std::ofstream& outputFile, std::string color) const {
	outputFile << "<span style='color:" << color << "'>" << id << "</span>";
}


void Edge::print_to_file(std::ofstream& outputFile, std::string color) const {
	outputFile << "<span style='color:" << color << "'>(" << v1 << "," << v2 << ")</span>";
}


void Graph::print_to_file(const std::string& filename) const {
	std::ofstream outputFile(filename);
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file!" << std::endl;
		return;
	}

	outputFile << "<html><body> \n";

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
	outputFile << "}<br>\n";

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
	outputFile << "}\n";

	outputFile << "</body></html>" << std::endl;
	outputFile.close();
}


void Graph::gen_rand_graph(int n, float p) {
	adj = std::vector<std::vector<bool>>(n, std::vector<bool>(n, false));

	for (int i = 0; i < n; i++) {
		add_vertex(i);
		for (int j = i + 1; j < n; j++) {
			if (float(rand()) / RAND_MAX < p) {
				add_edge(i, j);
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


void Graph::get_degrees() {
	for (int i = 0; i < adj.size(); i++) {
		for (int j = 0; j < adj.size(); j++) {
			if (adj[i][j]) {
				V[i].degree++;
			}
		}
	}
}


bool Graph::has_k3() const {
	int n = V.size();
	for (int i = 0; i < n; i++) {										
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
				for (int l = k + 1; l < n; ++l) {																
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