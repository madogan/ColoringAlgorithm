// Muhammed Ali Doğan
// 150115035
// Project#3

#ifndef graph
#define graph

#include "base.c"
#include "queue.c"

typedef struct _Graph{
    int num_vertices;
    int *visited;
	char colors[32];
	struct Node *vertices;
    struct Node **adj_lists;
}_Graph;

typedef struct Node{
    int vertex;
	char data[64];
	int color;
	int colored;
    struct Node *next;
}Node;

_Graph* _graph = NULL;

_Graph* allocate_graph();
_Graph* build_graph(int num_vertices);

void print_graph();
void build_colors(Node *vertices);
void add_edge(Node *src, Node *dest);
void set_color(Node *node, int color);
void bubbleSort(Node **start);
void swap(Node *a, Node *b);

Node* allocate_node();
Node* BFS(int vertex);
Node* insert_node_end(Node **node, Node *src);
Node* insert_node_front(Node **node, Node *src);
Node* build_node(char data[64], int coordinate);
Node* retrieve_node_by_index(Node *vertices, Node *dest, int coordinate);
Node* retrieve_node_by_data(Node *vertices, Node *dest, char data[64]);

int coloration(_intList *list);
int is_exist_node(Node *a, Node *b);
int min_color_of_adjs(_intList *list);
int max_color_of_adjs(_intList *list);
int is_exist_edge(Node *src, Node *dest);
int is_exist_data(Node *a, char data[64]);
int is_exist_vertex(Node *a, int vertex);

_intList* find_coordinate_of_adjacencies(Node *vertices, _intList *dest);

/* ------------------------------------------------------------------------------ */

typedef struct Lesson{
	char name[64];
	struct Lesson *next_lesson;
}Lesson;

typedef struct Period{
    int num;
	struct Period *next_period;
	struct Lesson *lesson_list;
}Period;

Period *period_header = NULL;

int set_number_of_lessons = -1;

Lesson* insert_lesson_end(Lesson **node, char lesson_name[128]);
Lesson* allocate_lesson();

Period* insert_period_end(Period **node, int num, Lesson *lesson_list);
Period* allocate_period();

void build_lessons();
void print_slots();
void generate_edges(Period *period);

int calculate_number_of_set_of_lessons();
int is_exist_lesson_name(Lesson *a, char name[64]);
int find_max_color(Node *node);
int len_lesson_list(Lesson *list);

#endif
/*
	A function that calculates number of vertices uniquely
*/
int calculate_number_of_set_of_lessons()
{
	int ret = 0;
	Period *tmp_period_header = period_header;
	Lesson *lesson_set = NULL;
	while(tmp_period_header != NULL)
	{
		Lesson *tmp_lesson_list = tmp_period_header->lesson_list;
		while(tmp_lesson_list != NULL)
		{
			if(is_exist_lesson_name(lesson_set, tmp_lesson_list->name) == 0)
			{
				insert_lesson_end(&lesson_set, tmp_lesson_list->name);
				ret++;
			}
			tmp_lesson_list = tmp_lesson_list->next_lesson;
		}
		tmp_period_header = tmp_period_header->next_period;
	}
	
	
	return ret;
}
/*
	A function that finds max color in given node list
*/
int find_max_color(Node *node)
{
	int ret = 0;
	Node *tmp_node = node;
	while(tmp_node != NULL)
	{
		if(tmp_node->color > ret)
		{
			ret = tmp_node->color;
		}
		tmp_node = tmp_node->next;
	}
	return ret;
}
/*
	A function that prints lesson slots
	according to colors
*/
void print_slots()
{
	Node *tmp_vertices = _graph->vertices;
	
	int i, max_color = find_max_color(tmp_vertices)-65;

	Node *node_next;
	for(i = 0; i <= max_color;i++)
	{
		printf("\n Final Exam Period %d: ", i+1);
		while(tmp_vertices != NULL)
		{
			if(tmp_vertices->color == i+65)
			{
				printf("%s, ", tmp_vertices->data);
			}
			tmp_vertices = tmp_vertices->next;
		}
		tmp_vertices = _graph->vertices;
	}
}
/*
	A function that add edges as combination of 
	lessons which are taken by a student
*/
void generate_edges(Period *period)
{
	Period *interim_period = period;
	Lesson *tmp_lessons;
	Node *tmp_node_1, *tmp_node_2, *tmp;
	Node *full_lesson_list = NULL;
	int i = 0, j = 0, k = 0, t = 0;
	
	/* This loop builds set of vertices (sorted) */
	for(k = 0; interim_period != NULL; interim_period = interim_period->next_period)
	{
		for(tmp_lessons = interim_period->lesson_list; tmp_lessons != NULL; tmp_lessons = tmp_lessons->next_lesson)
		{
			if(is_exist_data(_graph->vertices, tmp_lessons->name) == 0)
			{
				tmp_node_1 = build_node(tmp_lessons->name, k++);
				insert_node_end(&_graph->vertices, tmp_node_1);	
			}
			tmp_node_2 = build_node(tmp_lessons->name, t++);
			insert_node_end(&full_lesson_list, tmp_node_2);
			
		}
	}
		
	Node *tmp_vertices = _graph->vertices;
	Period *tmp_period = period;
	int num_lessons = 0;
	while(tmp_period != NULL)
	{
		Lesson *tmp_lesson_list = tmp_period->lesson_list;
		num_lessons = len_lesson_list(tmp_lesson_list);
		while(tmp_lesson_list->next_lesson != NULL)
		{	
			Node *node = retrieve_node_by_data(tmp_vertices, node, tmp_lesson_list->name);
			Lesson *tmp_lesson_next = tmp_lesson_list->next_lesson;
			
			for(j = i; tmp_lesson_next != NULL ;  tmp_lesson_next = tmp_lesson_next->next_lesson, j++)
			{
				Node *node_next = retrieve_node_by_data(tmp_vertices, node_next, tmp_lesson_next->name);
				add_edge(node, node_next);	
			}
			tmp_lesson_list = tmp_lesson_list->next_lesson;
		}
		i++;
		tmp_period = tmp_period->next_period;
	}
}
/*
	A function that calculates length of lesson list
*/
int len_lesson_list(Lesson *list)
{
	int ret = 0;
	Lesson *tmp_list = list;
	while(tmp_list != NULL)
	{
		ret++;
		tmp_list = tmp_list->next_lesson;
	}
	return ret;
}
/*
	A function that build a struct which
	store periods and its lessons as per file
*/
void build_lessons()
{
	build_text("file.txt");

	char str_lessons_line[512];
	char str_lesson[128] = "";

	int i, number_of_period = 0;
	
	int first_index = 0, second_index;
	
	Text *tmp_text_first_line = text_header;
	while(tmp_text_first_line != NULL)
	{
		Lesson *lesson_list = NULL;

		/* This statement deletes name of students */
		strcpy(str_lessons_line, split(tmp_text_first_line->line, str_lessons_line
		, (char_index(tmp_text_first_line->line, ":", 0)+1),  
		(strlen(tmp_text_first_line->line))));
		
		i = -1;
		do
		{
			i++;
			
			if(char_index(str_lessons_line, ",", i) == -1)
			{
				first_index = char_index(str_lessons_line, ",", i-1)+1;
				second_index = char_index(str_lessons_line, "\n", 0)-1;
				strcpy(str_lesson, split(str_lessons_line, str_lesson, first_index, second_index));
			}
			else
			{
				second_index = char_index(str_lessons_line, ",", i)-1;
				strcpy(str_lesson, split(str_lessons_line, str_lesson, first_index, second_index));
				first_index = second_index+2;
			}
			format(str_lesson);
			insert_lesson_end(&lesson_list, str_lesson);
		}while(char_index(str_lessons_line, ",", i) != -1);
		
		insert_period_end(&period_header, number_of_period++, lesson_list);
		first_index = 0;
		tmp_text_first_line = tmp_text_first_line->next;
	}
}
/*
	A function that inserts new lesson at end. 
*/
Lesson* insert_lesson_end(Lesson **node, char lesson_name[128])
{
    Lesson *current, *interim, *temporary;
	
	//Create node to insert and assign values to its fields
    current = allocate_lesson();
    strcpy(current->name, lesson_name);
	current->next_lesson = NULL;
	
    //If first node is null
    if(*node == NULL){
        *node = current;
    }	
	else //If list is not empty
	{	
		interim = *node;
		while (interim->next_lesson != NULL) 
		{
			interim = interim->next_lesson;
		};
		interim->next_lesson = current;
	}
	return current;
}
/*
	A function that inserts new period at end. 
*/
Period* insert_period_end(Period **node, int num, Lesson *lesson_list)
{
    Period *current, *interim, *temporary;
	
	//Create node to insert and assign values to its fields
    current = allocate_period();
    current->num = num;
	current->lesson_list = lesson_list;
	current->next_period = NULL;
	
    //If first node is null
    if(*node == NULL){
        *node = current;
    }	
	else //If list is not empty
	{	
		interim = *node;
		while (interim->next_period != NULL) 
		{
			interim = interim->next_period;
		};
		interim->next_period = current;
	}
	return current;
}
/*
	A function that allocates
	memory space for Lesson
*/
Lesson* allocate_lesson()
{
	Lesson *node = (Lesson *)malloc(sizeof(Lesson));
	return node;
}
/*
	A function that allocates
	memory space for Period
*/
Period* allocate_period()
{
	Period *node = (Period *)malloc(sizeof(Period));
	return node;
}
/*
	A function that implements coloring algorithm.
*/
void build_colors(Node *vertices)
{
	Node *tmp_vertices = vertices;             // A temporary variable which holds vertices of graph.
	_intList *adj_list_intList; 		       // A integer linked list which holds index of adjacency vertices.
	
	int index_vertex;                          // A variable which is used to visit all vertices.
	int color_num = 0; 			               // A variable which is used to give color.
	int num_max_color_of_adjs = 0; 			   // A variable which holds value of max colored adjacency.
	int check_all_vertices_not_colored = 0;    // A variable which is used if all adjacencies are not colored.
	
	/* This loop is used to visit all vertices */
	for(index_vertex = 0; tmp_vertices != NULL; tmp_vertices = tmp_vertices->next, index_vertex++, color_num = 0, num_max_color_of_adjs = 0)
	{
		/* -find_coordinate_of_adjacencies- function returns coordinates of adjacencies of the vertex */
		adj_list_intList = find_coordinate_of_adjacencies(tmp_vertices, adj_list_intList);
		
		/* This condition is used to select uncolored vertices */
		if(tmp_vertices->colored != 1)
		{
			/* This function sets color by number */
			set_color(tmp_vertices, color_num);
			
			/* This loop visits all adjacencies of the vertex */
			_intList *tmp_adj_list_intList = adj_list_intList;
			while(tmp_adj_list_intList != NULL)
			{
				/* -dest- is adjacency of vertex */
				Node *dest = retrieve_node_by_index(vertices, dest, tmp_adj_list_intList->key);
				
				/* 
					This -if/else- statement is used to determine -color_num-
					in order to find minimum and not colored number
				*/
				_intList *dest_adj_list = find_coordinate_of_adjacencies(dest, dest_adj_list);
				if(max_color_of_adjs(dest_adj_list) >= len_intList(dest_adj_list))
				{
					color_num = min_color_of_adjs(dest_adj_list);
				}
				else
				{
					color_num = max_color_of_adjs(dest_adj_list);
				}
				
				/* This condition is used to select uncolored adjacency vertex */
				if(dest->colored != 1)
				{
					/* This condition is used when all adjacencies are not colored */
					if(check_all_vertices_not_colored == 0)
					{
						color_num = num_max_color_of_adjs++;
					}
					set_color(dest, (color_num+1));
				}
				tmp_adj_list_intList = tmp_adj_list_intList->next;
			}
		}
	}
}
/*
	A function that changes color of the given number in given graph
*/
void set_color(Node *node, int color)
{
	Node *interim = _graph->vertices;
	
	node->color = _graph->colors[color];
	node->colored = 1;
	
	int i, j;
	for(i = 0; i < _graph->num_vertices; i++)
	{
		Node *temporary = _graph->adj_lists[i];
		while(temporary != NULL)
		{
			if(strcmp(temporary->data, node->data) == 1 && temporary->vertex == node->vertex)
			{
				temporary->color = _graph->colors[color];
				temporary->colored = 1;
			}
			temporary = temporary->next;
		}
	}
}
/*
	A function that finds adjacencies of given vertex.
*/
_intList* find_coordinate_of_adjacencies(Node *vertices, _intList *dest)
{
	_intList *int_list = NULL;
	
	Node *interim = vertices; 
	Node **temporary = _graph->adj_lists;
	
	int i = 0, color_num, j, counter = 0; 
	
	for(j = 0; j < _graph->num_vertices; j++)
	{
		if(is_exist_node(temporary[j], interim) == 1)
		{
			insert_by_key_intList(&int_list, j);
		}	
	}
	dest = int_list;
	return dest;
}
/*
	A function that give node by data
*/
Node* retrieve_node_by_data(Node *vertices, Node *dest, char data[64])
{
	Node *temp = vertices; int n;
	
	for(; temp!= NULL;temp = temp->next)
	{
		if(strcmp(temp->data, data) == 0)
		{
			dest = temp; n = 1;
		}
	}
	if(n != 1)
	{
		dest = NULL;
	}
	return dest;
}
/*
	A function that give node by vertex
*/
Node* retrieve_node_by_index(Node *vertices, Node *dest, int coordinate)
{
	Node *temp = vertices; int n;
	
	for(n = 0; n <= coordinate; n++, temp = temp->next)
	{
		dest = temp;
	}
	return dest;
}
/*
	A function that find the last number of color.
*/
int max_color_of_adjs(_intList *list)
{
	int ret = 65;
	Node *dest; Node *tmp_vertices = _graph->vertices; _intList *interim = list;

	while(interim != NULL)
	{
		dest = retrieve_node_by_index(tmp_vertices, dest, interim->key);
		if(dest->color >= ret)
		{
			ret = dest->color;
		}
		interim = interim->next;
	}

	return ret-65;
}
/*
	A function that find the last number of color.
*/
int min_color_of_adjs(_intList *list)
{
	int ret = 65;
	Node *dest; Node *tmp_vertices = _graph->vertices; _intList *interim = list;

	while(interim != NULL)
	{
		dest = retrieve_node_by_index(tmp_vertices, dest, interim->key);
		if(dest->color <= ret)
		{
			ret = dest->color;
		}
		interim = interim->next;
	}

	return ret-65;
}
/*
	A function that adds edge between two vertices.
*/
void add_edge(Node *src, Node *dest)
{ 
	// Add edge from dest to src
	Node* new_node_1 = allocate_node();
	new_node_1 = build_node(dest->data, dest->vertex);
    new_node_1->next = _graph->adj_lists[src->vertex];
    _graph->adj_lists[src->vertex] = new_node_1;
 
    // Add edge from src to dest
	Node* new_node_2 = allocate_node();
    new_node_2 = build_node(src->data, src->vertex);
    new_node_2->next = _graph->adj_lists[dest->vertex];
    _graph->adj_lists[dest->vertex] = new_node_2;
}
/*
	A function that checks there is an edge between two node.
*/
int is_exist_edge(Node *src, Node *dest)
{
	int ret = 0;
	
	if(is_exist_node(_graph->adj_lists[src->vertex], dest) == 1 &&
	   is_exist_node(_graph->adj_lists[dest->vertex], src) == 1 )
	{
		ret = 1;
    }
	
	return ret;
}
/*
	A function that checks there is a lesson for given data.
*/
int is_exist_lesson_name(Lesson *a, char name[64])
{
	Lesson *tmp = a;
	
	if(tmp == NULL){
		return(0);
	}
	while(tmp != NULL)
	{
		if(strcmp(tmp->name, name) == 0)
		{
			return(1);
		}
		tmp = tmp->next_lesson;
	}
	return(0);
}
/*
	A function that checks there is a node for given vertex
*/
int is_exist_vertex(Node *a, int vertex)
{
	Node *tmp = a;
	
	if(tmp == NULL){
		return(0);
	}
	while(tmp != NULL)
	{
		if(tmp->vertex == vertex)
		{
			return(1);
		}
		tmp = tmp->next;
	}
	return(0);
}
/*
	A function that checks there is a node for given data.
*/
int is_exist_data(Node *a, char data[64])
{
	Node *tmp = a;
	
	if(tmp == NULL){
		return(0);
	}
	while(tmp != NULL)
	{
		if(strcmp(tmp->data, data) == 0)
		{
			return(1);
		}
		tmp = tmp->next;
	}
	return(0);
}
/*
	A function that checks there is a node for given node.
*/
int is_exist_node(Node *a, Node *b)
{
	Node *tmp = a;
	
	if(tmp == NULL){
		return(0);
	}
	while(tmp != NULL)
	{
		if(strcmp(tmp->data, b->data) == 0 && tmp->vertex == b->vertex)
		{
			return(1);
		}
		tmp = tmp->next;
	}
	return(0);
}
/*
	A function that inserts new node at front. 
*/
Node* insert_node_front(Node **node, Node *src) 
{
    /* Create a new Linked List node */
    Node *current, *interim, *temporary;
	
	//Create node to insert and assign values to its fields
    current = allocate_node();
    strcpy(current->data, src->data);
	current->vertex = src->vertex;
	current->next = NULL;
  
    //If first node is null
    if(*node == NULL){
        *node = current;
    }	
	else //If list is not empty
	{	
		interim = *node;
		interim = current;
		interim->next = *node;
	}
	return current;
}
/*
	A function that inserts new node at end. 
*/
Node* insert_node_end(Node **node, Node *src)
{
    Node *current, *interim, *temporary;
	
	//Create node to insert and assign values to its fields
    current = allocate_node();
	current = build_node(src->data, src->vertex);
	
    //If first node is null
    if(*node == NULL){
        *node = current;
    }	
	else //If list is not empty
	{	
		interim = *node;
		while (interim->next != NULL && interim->next->vertex < current->vertex) 
		{
			interim = interim->next;
		};
		current->next = interim->next;
		interim->next = current;
	}
	return current;
}
/*
	A function that prints _graph.
*/
void print_graph() //Burada sıralama işlemi yapıyor, hataya sebep olabilir dikkat et.
{
	Node* interim = _graph->vertices; int v; 
    for (v = 0; v < _graph->num_vertices; v++, interim = interim->next)
    { 
        Node *temp = _graph->adj_lists[v];
		//bubbleSort(&temp);
        printf("\n Adjacency list of vertex %s(%d)\n ", interim->data,interim->vertex);
        while (temp)
        {
            printf("%s(%d) -> ", temp->data, temp->vertex);
            temp = temp->next;
        }
        printf("- \n");
    }
}
/*
	A function that generates a _graph.
*/
_Graph* build_graph(int num_vertices)
{
    _graph = allocate_graph();
    
	_graph->num_vertices = num_vertices;
 
    _graph->adj_lists = malloc(num_vertices * sizeof(Node*));
    
    _graph->visited = malloc(num_vertices * sizeof(int));
	
	_graph->vertices = NULL;
	
    int i, c; char tmp[0];
	for (i = 0; i < num_vertices; i++) {
        _graph->adj_lists[i] = NULL;
        _graph->visited[i] = 0; 
		_graph->colors[i] = (char)(i + 65);
    }
    return _graph;
}
/*
	A function that generates new node.
*/
Node* build_node(char data[64], int coordinate)
{
    Node *new_node = allocate_node();
    new_node->vertex = coordinate; // Coordinate
	strcpy(new_node->data, data); // Data
	new_node->colored = 0;
    new_node->next = NULL;
    return new_node;
}
/*
	A function that allocates
	memory space for _Graph
*/
_Graph* allocate_graph()
{
	_Graph *_graph = (_Graph *)malloc(sizeof(_Graph));
	return _graph;
}
/*
	A function that allocates
	memory space for Node
*/
Node* allocate_node()
{
	Node *node = (Node *)malloc(sizeof(Node));
	return node;
}
/* 
	Bubble sort the given linked list 
*/
void bubbleSort(Node **start)
{
    int swapped, i;
    struct Node *ptr1;
    struct Node *lptr = NULL;
 
    /* Checking for empty list */
    if (ptr1 == NULL)
        return;
 
    do
    {
        swapped = 0;
        ptr1 = *start;
 
        while (ptr1->next != lptr)
        {
            if (strcmp(ptr1->data, ptr1->next->data) == 1)
            { 
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}
 
/* 
	A function that swaps data of two nodes a and b
*/
void swap(Node *a, Node *b)
{
    char tmp[64];
	strcmp(tmp, a->data);
	strcmp(a->data, b->data);
	strcmp(b->data, tmp);
}
