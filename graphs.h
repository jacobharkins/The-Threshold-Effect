#include<vector>
#include<iostream>

class Edge {
public:
	int v1;
	int v2;
	int color;

	Edge(int vertex1, int vertex2) : v1(vertex1), v2(vertex2), color(15){}
	Edge(int vertex1, int vertex2, int color = 15) : v1(vertex1), v2(vertex2), color(color){}
	void print() const;
	void print_to_file(std::ofstream& outputFile, std::string color) const;
};


class Vertex {
public:
	int id;
	int color;
	int degree;

	Vertex(int v) : id(v), color(15){}
	Vertex(int v, int color) : id(v), color(color) {}
	void print() const;
	void print_to_file(std::ofstream& outputFile, std::string color) const;
};


class Graph {
public:
	std::vector<Vertex> V;
	std::vector<Edge> E;
	std::vector<std::vector<bool>> adj;

	Graph() : V(), E(){}
	Graph(const std::vector<Vertex>& Vertices, const std::vector<Edge>& Edges) : V(Vertices), E(Edges) {}


	void print() const;										// Print the Graph to the console
	void print_to_file(const std::string& filename) const;		// Print the Graph to a file
	void gen_rand_graph(int n, float p);					// Generate Random Graphs
	void gen_rand_colors();									// Randomly Color the Graph
	void add_vertex(int id, int color);						// Add a vertex to Graph
	void add_edge(int u, int v, int color = 15);			// Add a vertex to Graph
	void get_degrees();										// Get all vertex degrees
	bool has_edge() const;									// Check if the Graph has an Edge
	bool has_k3() const;									// Check if the Graph has a k3 Subgraph
	bool has_k4() const;									// Check if the Graph has a k4 Subgraph


private:
	std::string get_color_hex(int color) const;					// Convert Hex code to color
};

