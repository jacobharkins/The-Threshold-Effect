#include<vector>
#include<iostream>
using namespace std;

class Edge {
public:
	int v1;
	int v2;
	int color;

	Edge(int vertex1, int vertex2) : v1(vertex1), v2(vertex2), color(15){}
	Edge(int vertex1, int vertex2, int color = 15) : v1(vertex1), v2(vertex2), color(color){}
	void print() const;
	void print_to_file(ofstream& outputFile, string color) const;
};

class Vertex {
public:
	int id;
	int color;

	Vertex(int v) : id(v), color(15){}
	Vertex(int v, int color) : id(v), color(color) {}
	void print() const;
	void print_to_file(ofstream& outputFile, string color) const;
};


class Graph {
public:
	vector<Vertex> V;
	vector<Edge> E;
	vector<vector<bool>> adj;

	Graph() : V(), E(){}
	Graph(const vector<Vertex>& Vertices, const vector<Edge>& Edges) : V(Vertices), E(Edges) {}


	void print() const;									// Print the Graph to the console
	void print_to_file(const string& filename) const;							// Print the Graph to a file
	void gen_rand_graph(int n, float p);				// Generate Random Graphs
	void gen_rand_colors();								// Randomly Color the Graph
	void add_vertex(int id, int color);					// Add a vertex to Graph
	void add_edge(int u, int v, int color = 15);		// Add a vertex to Graph
	bool has_edge() const;								// Check if the Graph has an Edge
	bool has_triangle() const;							// Check if the Graph has a Triangle Subgraph
	bool has_k4() const;								// Check if the Graph has a k4 Subgraph


private:
	string get_color_hex(int color) const;
};

