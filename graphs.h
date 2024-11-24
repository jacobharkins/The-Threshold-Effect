#include<vector>
#include<iostream>

#pragma once


class Vertex {
public:
	int id;
	int color;
	int degree;

	Vertex() : id(-1), color(15), degree(0) {}
	Vertex(int v) : id(v), color(15), degree(0) {}
	Vertex(int v, int color) : id(v), color(color), degree(0) {}
	Vertex(int v, int color, int degree) : id(v), color(color), degree(degree) {}

	friend std::wistream& operator>>(std::wistream& is, Vertex& vertex);
	friend std::wostream& operator<<(std::wostream& os, const Vertex& vertex);

	void cprint() const;
	void export_vertex(std::wofstream& outputFile) const;
	void print_to_file(std::ofstream& outputFile, std::string color) const;
};


class Edge {
public:
	int v1;
	int v2;
	int color;

	Edge() : v1(-1), v2(-1), color(15) {}
	Edge(int vertex1, int vertex2) : v1(vertex1), v2(vertex2), color(15) {}
	Edge(int vertex1, int vertex2, int color) : v1(vertex1), v2(vertex2), color(color) {}

	friend std::wistream& operator>>(std::wistream& is, Edge& edge);
	friend std::wostream& operator<<(std::wostream& os, const Edge& edge);

	void cprint() const;
	void export_edge(std::wofstream& outputFile) const;
	void print_to_file(std::ofstream& outputFile, std::string color) const;
};



class Graph {
public:
	std::vector<Vertex> V;
	std::vector<Edge> E;
	std::vector<std::vector<bool>> adj;

	Graph() : V(), E(){}
	Graph(const std::vector<Vertex>& Vertices, const std::vector<Edge>& Edges) : V(Vertices), E(Edges) {}

	friend std::wistream& operator>>(std::wistream& is, Graph& graph);
	friend std::wostream& operator<<(std::wostream& os, const Graph& graph);

	void cprint() const;										// Print the Graph to the console
	void print_to_file(const std::string& filename) const;		// Print the Graph to a file
	void export_graph(const std::wstring& filename) const;		// Export the Graph to a file
	void import_graph(const std::wstring& filename);			// Import the Graph from a file
	void generate_svg(const std::string& filename) const;		// Generate SVG of graph
	void gen_rand_graph(int n, float p);						// Generate Random Graphs
	void gen_rand_colors();										// Randomly Color the Graph
	void add_vertex(int id, int color = 15);					// Add a Vertex to Graph
	void add_edge(int u, int v, int color = 15);				// Add an Edge to Graph
	void get_degrees();											// Get all vertex degrees
	bool has_edge() const;										// Check if the Graph has an Edge
	bool has_k3() const;										// Check if the Graph has a k3 Subgraph
	bool has_k4() const;										// Check if the Graph has a k4 Subgraph


private:
	std::string get_color_hex(int color) const;					// Convert Hex code to color
};

