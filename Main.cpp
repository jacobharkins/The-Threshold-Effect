#include "graphs.h"
#include <string>


int main() {
	srand(time(0));
	Graph graphs[2];
	//for (int i = 0; i < std::size(graphs); i++) {
	//	graphs[i].gen_rand_graph(rand() % 20, float(rand()) / RAND_MAX);
	//	graphs[i].gen_rand_colors();
	//	graphs[i].export_graph("graph" + std::to_string(i));
	//}
	graphs[0].import_graph("graph0.g");
	graphs[1].import_graph("graph1.g");
	graphs[0].cprint();
	graphs[1].cprint();
}