#include "graphs.h"
#include <string>
using namespace std;


int main() {
	srand(time(0));
	Graph graphs[10];
	for (int i = 0; i < size(graphs); i++ ) {
		graphs[i].gen_rand_graph(rand() % 50, float(rand()) / RAND_MAX);
		graphs[i].gen_rand_colors();
		graphs[i].print();
		graphs[i].print_to_file(string("graph") + to_string(i) + string(".html"));
	}
}
