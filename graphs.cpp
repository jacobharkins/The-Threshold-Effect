
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <cwchar>
#include <iomanip>
#include<iostream>
#include <ctime>

#include "graphs.h"



//Overloading Functions ------------------------------------------------------------------------------------------------------------------
// Overload for Vertex InStream
std::istream& operator>>(std::istream& is, Vertex& vertex) {
	char comma;
	is >> vertex.id >> comma >> vertex.degree >> comma >> vertex.color;
	return is;
}

// Overload for Vertex OutStream
std::ostream& operator<<(std::ostream& os, const Vertex& vertex) {
	os << vertex.id << "," << vertex.degree << "," << vertex.color;
	return os;
}

// Overload for Edge InStream
std::istream& operator>>(std::istream& is, Edge& edge) {
	char comma;
	is >> edge.v1 >> comma >> edge.v2 >> comma >> edge.color;
	return is;
}

// Overload for Edge OutStream
std::ostream& operator<<(std::ostream& os, const Edge& edge) {
	os << edge.v1 << "," << edge.v2 << "," << edge.color;
	return os;
}


// Overload for Graph InStream
std::istream& operator>>(std::istream& is, Graph& graph) {
	std::string line;

	// Clear the graph before importing
	graph.V.clear();
	graph.E.clear();

	// Vertices section
	if (std::getline(is, line) && line == "V") {
		while (std::getline(is, line) && !line.empty() && line != "E") {
			std::stringstream vertexStream(line);
			Vertex vertex;
			if (vertexStream >> vertex) {
				graph.V.push_back(vertex);
			}
			else {
				std::cerr << "Error parsing vertex: " << line << std::endl;
			}
		}
	}

	// Edge section
	if (line == "E") {
		while (std::getline(is, line) && !line.empty()) {
			std::stringstream edgeStream(line);
			Edge edge;
			if (edgeStream >> edge) {
				graph.E.push_back(edge);
			}
			else {
				std::cerr << "Error parsing edge: " << line << std::endl;
			}
		}
	}

	return is;
}

// Overload for Graph OutStream
std::ostream& operator<<(std::ostream& os, const Graph& graph) {
	// Output vertices
	os << "V" << std::endl;
	for (Vertex vertex : graph.V) {
		os << vertex << std::endl;
		std::cout << vertex << std::endl;
	}
	os << "E" << std::endl;
	// Output edges
	for (Edge edge : graph.E) {
		os << edge << std::endl;
		std::cout << edge << std::endl;
	}

	return os;
}


// Console Print Functions ---------------------------------------------------------------------------------------------------------------
// Print Vertex to the Console
void Vertex::cprint() const {
	setConsoleColor(color_to_console_color(hex_to_rgb(color)));
	std::cout << id;
	resetConsoleColor();
}

// Print Edge to the Console
void Edge::cprint() const {
	setConsoleColor(color_to_console_color(hex_to_rgb(color)));
	std::cout << "(" << v1 << "," << v2 << ")";
	resetConsoleColor();
}

// Print Graph to the Console
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


// Graph File Manipulation Functions -----------------------------------------------------------------------------------------------------
// Generate SVG
void Graph::generate_svg(const std::wstring& filename) const {
	std::ofstream outputFile(filename);
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file!" << std::endl;
		return;
	}
	int size = calculate_grid_size(V.size());
	int scale = 250;
	int offset = 25;
	outputFile << "<svg width=\"" + std::to_string(size * scale) + "\" height=\"" + std::to_string(size * scale) + "\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
	
	// Print edges
	outputFile << "<!-- Print Edges -->" << std::endl;
	for (const Edge& e : E) {
		std::pair<int, int> p1 = get_2d_coords(e.v1, size);
		std::pair<int, int> p2 = get_2d_coords(e.v2, size);
		std::cout << p1.first << " " << p1.second << ":" << p2.first << " " << p2.second << std::endl;
		outputFile << "<line x1=\"" + std::to_string((p1.first * scale) + offset) + "\" y1=\"" + std::to_string((p1.second * scale) + offset) + "\" x2=\"" + std::to_string((p2.first * scale) + offset) + "\" y2=\"" + std::to_string((p2.second * scale) + offset) + "\" stroke=\"" + e.color + "\" stroke-width=\"4\" />" << std::endl;
	}
	// Print vertices
		outputFile << "<!-- Print Vertices -->" << std::endl;
	for (const Vertex& v : V) {
		std::pair<int, int> p = get_2d_coords(v.id, size);
		std::cout << p.first << " " << p.second << std::endl;
		outputFile << "<circle cx=\"" + std::to_string((p.first * scale) + offset) + "\" cy=\"" + std::to_string((p.second * scale) + offset) + "\" r=\"5\" fill=\"" + v.color + "\" />" << std::endl;
	}

	outputFile << "</svg>" << std::endl;
	outputFile.close();
}

// Graph Export Function
void Graph::export_graph(const std::wstring& filename) const {
	std::ofstream outputFile(filename, std::wfstream::trunc);
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file!" << std::endl;
		return;
	}
	outputFile << std::dec;
	outputFile << *this;
	outputFile.close();
}


// Graph Import Function
void Graph::import_graph(const std::wstring& filename) {
	std::ifstream inputFile(filename);
	if (!inputFile.is_open()) {
		std::cerr << "Error opening file!" << std::endl;
		return;
	}
	inputFile >> std::dec;
	inputFile >> *this;
	inputFile.close();
}



// Graph Generation Functions ------------------------------------------------------------------------------------------------------------
// Generate Random Graph
void Graph::gen_rand_graph(int n, float p) {
	V.clear();
	E.clear();
	adj = std::vector<std::vector<bool>>(n, std::vector<bool>(n, false));

	for (int i = 0; i < n; i++) {
		add_vertex();
		for (int j = 0; j < i; ++j) {
			if (float(rand()) / RAND_MAX < p) {
				add_edge(i, j);
			}
		}
	}
}

// Randomly Color Graph
void Graph::gen_rand_colors() {
	for (Vertex& vertex : V) {
		int r = rand() % 256;  
		int g = rand() % 256;
		int b = rand() % 256;

		std::stringstream ss;
		ss << "#" << std::hex
			<< std::setw(2) << std::setfill('0') << r
			<< std::setw(2) << std::setfill('0') << g
			<< std::setw(2) << std::setfill('0') << b;

		vertex.color = ss.str();
	}
	std::cout << std::endl;
	for (Edge& edge : E) {
		int r = rand() % 256;  
		int g = rand() % 256;
		int b = rand() % 256;

		std::stringstream ss;
		ss << "#" << std::hex
			<< std::setw(2) << std::setfill('0') << r
			<< std::setw(2) << std::setfill('0') << g
			<< std::setw(2) << std::setfill('0') << b;

		edge.color = ss.str();
	}
}


// Graph Manipulation Functions ----------------------------------------------------------------------------------------------------------
// Add Vertex to Graph
void Graph::add_vertex(std::string color) {
	int id = V.size();
	V.emplace_back(id, color);
	size_t newSize = max(id + 1, adj.size());
	for (auto& row : adj) {
		row.resize(newSize, false); 
	}
	adj.resize(newSize, std::vector<bool>(newSize, false)); 
}

// Add Edge to Graph
void Graph::add_edge(int u, int v, std::string color) {
	int maxVertex = max(u, v);
	if (maxVertex >= adj.size()) {
		adj.resize(maxVertex + 1, std::vector<bool>(maxVertex + 1, false));
	}
	adj[u][v] = true;
	adj[v][u] = true;
	E.emplace_back(u, v, color);
}

// Get all Degrees
void Graph::get_degrees() {
	for (int i = 0; i < adj.size(); i++) {
		for (int j = 0; j < adj.size(); j++) {
			if (adj[i][j]) {
				V[i].degree++;
			}
		}
	}
}


// Graph Logic Functions -----------------------------------------------------------------------------------------------------------------
// Check if Graph Contains an Edge
bool Graph::has_edge() const {
	return bool(size(E));
}

// Check if Graph Contains a k3
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

// Check if Graph ontains a k4
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

// Recursive Hamiltonian Helper Function
bool Graph::is_hamiltonian_util(int pos, std::vector<int>& path, std::vector<bool>& visited) const {
	if (pos == V.size()) {
		// Check if the last vertex connects to the first to form a cycle
		return adj[path[pos - 1]][path[0]];
	}

	for (int vertex = 0; vertex < V.size(); ++vertex) {
		if (!visited[vertex] && adj[path[pos - 1]][vertex]) {
			path[pos] = vertex;
			visited[vertex] = true;

			if (is_hamiltonian_util(pos + 1, path, visited)) {
				return true;
			}

			// Backtrack
			visited[vertex] = false;
		}
	}

	return false;
}

// Check if Graph contains a hamiltonian cycle
bool Graph::is_hamiltonian() const {
	if (V.empty() || E.empty()) {
		return false;
	}

	std::vector<int> path(V.size(), -1);    
	std::vector<bool> visited(V.size(), false);

	path[0] = 0;                           
	visited[0] = true;

	return is_hamiltonian_util(1, path, visited);
}
