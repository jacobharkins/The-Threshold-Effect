#pragma once
#include <Windows.h>
#include "utilities.h"
#include <fstream>
#include <sstream>
#include <cwchar>
#include <iomanip>
#include<iostream>
#include <ctime>
#include <string>
#include<vector>

class Vertex {
public:
	int id;
	std::string color;
	int degree;

	Vertex() : id(-1), color("#000000"), degree(0) {}
	Vertex(int v) : id(v), color("#000000"), degree(0) {}
	Vertex(int v, std::string color) : id(v), color(color), degree(0) {}
	Vertex(int v, std::string color, int degree) : id(v), color(color), degree(degree) {}

	friend std::istream& operator>>(std::istream& is, Vertex& vertex);
	friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex);

	void cprint() const;
};


class Edge {
public:
	int v1;
	int v2;
	std::string color;

	Edge() : v1(-1), v2(-1), color("#FFFFFF") {}
	Edge(int vertex1, int vertex2) : v1(vertex1), v2(vertex2), color("#FFFFFF") {}
	Edge(int vertex1, int vertex2, std::string color) : v1(vertex1), v2(vertex2), color(color) {}

	friend std::istream& operator>>(std::istream& is, Edge& edge);
	friend std::ostream& operator<<(std::ostream& os, const Edge& edge);

	void cprint() const;
};



class Graph {
public:
	std::vector<Vertex> V;
	std::vector<Edge> E;
	std::vector<std::vector<bool>> adj;

	Graph() : V(), E() {}
	Graph(const std::vector<Vertex>& Vertices, const std::vector<Edge>& Edges) : V(Vertices), E(Edges) {}

	friend std::istream& operator>>(std::istream& is, Graph& graph);
	friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

	void cprint() const;													// Print the Graph to the console
	void generate_svg(const std::wstring& filename) const;					// Print the Graph to a file
	void export_graph(const std::wstring& filename) const;					// Export the Graph to a file
	void import_graph(const std::wstring& filename);						// Import the Graph from a file
	//void generate_svg(const std::string& filename) const;					// Generate SVG of graph
	void gen_rand_graph(int n, float p);									// Generate Random Graphs
	void gen_rand_colors();													// Randomly Color the Graph
	void add_vertex(std::string color = "#000000");							// Add a Vertex to Graph
	void add_edge(int u, int v, std::string color = "#000000");				// Add an Edge to Graph
	void get_degrees();														// Get all vertex degrees
	bool has_edge() const;													// Check if the Graph has an Edge
	bool has_k3() const;													// Check if the Graph has a k3 Subgraph
	bool has_k4() const;													// Check if the Graph has a k4 Subgraph
	bool is_hamiltonian() const;											// Check if the Graph has a Hamiltonian Cycle

private:
	bool is_hamiltonian_util(int pos, std::vector<int>& path, std::vector<bool>& visited) const;
};

