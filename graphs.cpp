#include "graphs.h"
#include <Windows.h>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>

#pragma once

// Helper Functions
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


//Overloading Functions

// Overload for Vertex Input
std::istream& operator>>(std::istream& is, Vertex& vertex) {
	char comma;
	is >> vertex.id >> comma >> vertex.color >> comma >> vertex.degree;
	return is;
}

// Overload for Vertex Output
std::ostream& operator<<(std::ostream& os, const Vertex& vertex) {
	os << vertex.id << "," << vertex.color << "," << vertex.degree;
	return os;
}

// Overload for Edge Input
std::istream& operator>>(std::istream& is, Edge& edge) {
	char comma;
	is >> edge.v1 >> comma >> edge.v2 >> comma >> edge.color;
	return is;
}

// Overload for Edge Output
std::ostream& operator<<(std::ostream& os, const Edge& edge) {
	os << edge.v1 << "," << edge.v2 << "," << edge.color;
	return os;
}

// Overload for Graph Input
std::istream& operator>>(std::istream& is, Graph& graph) {
	std::string verticesLine, edgesLine;

	// Clear the graph before importing
	graph.V.clear();
	graph.E.clear();

	// Read vertices line
	if (getline(is, verticesLine)) {
		std::stringstream vertexStream(verticesLine);
		std::string vertexInfo;
		while (getline(vertexStream, vertexInfo, ':')) {
			std::stringstream vertexData(vertexInfo);
			int id, color, degree;
			char comma;

			if (vertexData >> id >> comma >> color >> comma >> degree) {
				graph.V.emplace_back(id); // Add vertex to graph
				graph.V.back().color = color;
				graph.V.back().degree = degree;
			}
		}
	}

	// Read edges line
	if (getline(is, edgesLine)) {
		std::stringstream edgeStream(edgesLine);
		std::string edgeInfo;
		while (getline(edgeStream, edgeInfo, ':')) {
			std::stringstream edgeData(edgeInfo);
			int v1, v2, color;
			char comma;

			if (edgeData >> v1 >> comma >> v2 >> comma >> color) {
				graph.E.emplace_back(v1, v2); // Add edge to graph
				graph.E.back().color = color;
			}
		}
	}

	return is;
}

// Overload for Graph Output
std::ostream& operator<<(std::ostream& os, const Graph& graph) {
	// Output vertices
	for (size_t i = 0; i < graph.V.size(); ++i) {
		os << graph.V[i];
		if (i != graph.V.size() - 1) os << ":";
	}
	os << std::endl;

	// Output edges
	for (size_t i = 0; i < graph.E.size(); ++i) {
		os << graph.E[i];
		if (i != graph.E.size() - 1) os << ":";
	}

	return os;
}


// Console Print Functions
void Vertex::cprint() const {
	setConsoleColor(color);
	std::cout << id;
	resetConsoleColor();
}


void Edge::cprint() const {
	setConsoleColor(color);
	std::cout << "(" << v1 << "," << v2 << ")";
	resetConsoleColor();
}


void Graph::cprint() const {
	bool first = true;
	std::cout << "V(G): {";
	for (const Vertex& v : V) {
		if (!first) {
			std::cout << ", ";
		}
		v.cprint();
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
		e.cprint();
		first = false;
	}
	std::cout << "}\n";
	std::cout << "--------------------------------------------------------------------------------------------------------------" << std::endl;
}


// Graph Export Functions
void Vertex::export_vertex(std::ofstream& outputFile) const {
	outputFile << id << "," << degree << "," << color;
}


void Edge::export_edge(std::ofstream& outputFile) const {
	outputFile << v1 << "," << v2 << "," << color;
}


void Graph::export_graph(const std::string& filename) const {
	std::ofstream outputFile(filename + ".g", std::fstream::trunc);
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file!" << std::endl;
		return;
	}

	outputFile << *this;
	outputFile.close();
}


// Graph Import Function
void Graph::import_graph(const std::string& filename) {
	std::ifstream inputFile(filename);
	if (!inputFile.is_open()) {
		std::cerr << "Error opening file!" << std::endl;
		return;
	}

	inputFile >> *this;
	inputFile.close();
}


// Graph File Writing Functions
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


void Graph::generate_svg(const std::string& filename) const {
	std::ofstream outputFile(filename);
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file!" << std::endl;
		return;
	}

	outputFile << "<svg\n";

	// Define Picture
	outputFile << "	width\"";
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


// Graph Generation Functions
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


// Graph Manipulation Functions
void Graph::add_vertex(int id, int color) {
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



//Graph Logic Functions
void Graph::get_degrees() {
	for (int i = 0; i < adj.size(); i++) {
		for (int j = 0; j < adj.size(); j++) {
			if (adj[i][j]) {
				V[i].degree++;
			}
		}
	}
}


bool Graph::has_edge() const {
	return bool(size(E));
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