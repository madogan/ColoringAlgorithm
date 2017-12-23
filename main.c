// Muhammed Ali Doğan
// 150115035
// Project#3

#include "graph.c"

void main()
{
	system("color 6");
	
	build_lessons();

	_graph = build_graph(calculate_number_of_set_of_lessons());
	
	generate_edges(period_header);
	
	build_colors(_graph->vertices);
	
    print_graph();
	
	print_slots();
}