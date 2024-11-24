#include "graphs.h"

// Helper Functions



//Overloading Functions

// Overload for Vertex Input
std::wistream& operator>>(std::wistream& is, Vertex& vertex) {
	wchar_t comma;
	is >> vertex.id >> comma >> string_to_wchar(vertex.color) >> comma >> vertex.degree;
	return is;
}

// Overload for Vertex Output
std::wostream& operator<<(std::wostream& os, const Vertex& vertex) {
	os << vertex.id << L"," << string_to_wchar(vertex.color) << L"," << vertex.degree;
	return os;
}

// Overload for Edge Input
std::wistream& operator>>(std::wistream& is, Edge& edge) {
	wchar_t comma;
	is >> edge.v1 >> comma >> edge.v2 >> comma >> string_to_wchar(edge.color);
	return is;
}

// Overload for Edge Output
std::wostream& operator<<(std::wostream& os, const Edge& edge) {
	os << edge.v1 << L"," << edge.v2 << L"," << string_to_wchar(edge.color);
	return os;
}

// Overload for Graph Input
std::wistream& operator>>(std::wistream& is, Graph& graph) {
	std::wstring verticesLine, edgesLine;

	// Clear the graph before importing
	graph.V.clear();
	graph.E.clear();

	// Read vertices line
	if (getline(is, verticesLine)) {
		std::wstringstream vertexStream(verticesLine);
		std::wstring vertexInfo;
		while (getline(vertexStream, vertexInfo, L':')) {
			std::wstringstream vertexData(vertexInfo);
			int id, color, degree;
			wchar_t comma;

			if (vertexData >> id >> comma >> color >> comma >> degree) {
				graph.V.emplace_back(id); // Add vertex to graph
				graph.V.back().color = color;
				graph.V.back().degree = degree;
			}
		}
	}

	// Read edges line
	if (getline(is, edgesLine)) {
		std::wstringstream edgeStream(edgesLine);
		std::wstring edgeInfo;
		while (getline(edgeStream, edgeInfo, L':')) {
			std::wstringstream edgeData(edgeInfo);
			int v1, v2, color;
			wchar_t comma;

			if (edgeData >> v1 >> comma >> v2 >> comma >> color) {
				graph.E.emplace_back(v1, v2); // Add edge to graph
				graph.E.back().color = color;
			}
		}
	}

	return is;
}

// Overload for Graph Output
std::wostream& operator<<(std::wostream& os, const Graph& graph) {
	// Output vertices
	for (const auto& vertex : graph.V) {
		os << vertex.id << L"," << string_to_wchar(vertex.color) << L"," << vertex.degree << L":";
	}
	os << std::endl;

	// Output edges
	for (const auto& edge : graph.E) {
		os << edge.v1 << L"," << edge.v2 << L"," << string_to_wchar(edge.color) << L":";
	}

	return os;
}


// Console Print Functions
void Vertex::cprint() const {
	setConsoleColor(color_to_console_color(hex_to_rgb(color)));
	std::cout << id;
	resetConsoleColor();
}


void Edge::cprint() const {
	setConsoleColor(color_to_console_color(hex_to_rgb(color)));
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
void Vertex::export_vertex(std::wofstream& outputFile) const {
	outputFile << id << L"," << degree << L"," << string_to_wchar(color);
}


void Edge::export_edge(std::wofstream& outputFile) const {
	outputFile << v1 << L"," << v2 << L"," << string_to_wchar(color);
}


void Graph::export_graph(const std::wstring& filename) const {
	std::wofstream outputFile(filename + L".g", std::wfstream::trunc);
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file!" << std::endl;
		return;
	}

	outputFile << *this;
	outputFile.close();
}


// Graph Import Function
void Graph::import_graph(const std::wstring& filename) {
	std::wifstream inputFile(filename);
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
		v.print_to_file(outputFile, v.color);
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
		e.print_to_file(outputFile, e.color);
		firstEdge = false;
	}
	outputFile << "}\n";

	outputFile << "</body></html>" << std::endl;
	outputFile.close();
}


//void Graph::generate_svg(const std::string& filename) const {
//	std::ofstream outputFile(filename);
//	if (!outputFile.is_open()) {
//		std::cerr << "Error opening file!" << std::endl;
//		return;
//	}
//
//	outputFile << "<svg\n";
//
//	// Define Picture
//	outputFile << "	width\"";
//	bool firstVertex = true;
//	for (const Vertex& v : V) {
//		if (!firstVertex) {
//			outputFile << ", ";
//		}
//		v.print_to_file(outputFile, get_color_hex(v.color));
//		firstVertex = false;
//	}
//	outputFile << "}<br>\n";
//
//	// Print edges
//	outputFile << "<h3>Edges:</h3>{";
//	bool firstEdge = true;
//	for (const Edge& e : E) {
//		if (!firstEdge) {
//			outputFile << ", ";
//		}
//		e.print_to_file(outputFile, get_color_hex(e.color));
//		firstEdge = false;
//	}
//	outputFile << "}\n";
//
//	outputFile << "</body></html>" << std::endl;
//	outputFile.close();
//}


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
	for (Vertex vertex : V) {
		// Generate random RGB components
		int r = rand() % 256;  // Random number between 0 and 255
		int g = rand() % 256;
		int b = rand() % 256;

		// Convert RGB to a hex string
		std::stringstream ss;
		ss << "#" << std::setw(2) << std::setfill('0') << std::hex << r
			<< std::setw(2) << std::setfill('0') << g
			<< std::setw(2) << std::setfill('0') << b;

		vertex.color = ss.str();
	}
	for (Edge edge : E) {
		// Generate random RGB components
		int r = rand() % 256;  // Random number between 0 and 255
		int g = rand() % 256;
		int b = rand() % 256;

		// Convert RGB to a hex string
		std::stringstream ss;
		ss << "#" << std::setw(2) << std::setfill('0') << std::hex << r
			<< std::setw(2) << std::setfill('0') << g
			<< std::setw(2) << std::setfill('0') << b;

		edge.color = ss.str();
	}
}


// Graph Manipulation Functions
void Graph::add_vertex(int id, std::string color) {
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


void Graph::add_edge(int u, int v, std::string color) {
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