#include "graphs.h"
using namespace std;


int main() {
	srand(time(0));
	Graph graphs[1];
	for (Graph g : graphs) {
		g.gen_rand_graph(rand() % 100, float(rand()) / RAND_MAX);
		g.gen_rand_colors();
		g.print();
		g.print_to_file("graph.html");
	}
}
