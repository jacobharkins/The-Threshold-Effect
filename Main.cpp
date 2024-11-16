#include "graphs.h"
#include <string>



int main() {
	srand(time(0));
	Graph graphs[10];
	for (int i = 0; i < std::size(graphs); i++ ) {
		graphs[i].gen_rand_graph(rand() % 50, float(rand()) / RAND_MAX);
		graphs[i].gen_rand_colors();
		graphs[i].print();
		graphs[i].print_to_file(std::string("graph") + std::to_string(i) + std::string(".html"));
	}
}
