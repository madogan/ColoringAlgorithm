// Muhammed Ali Doğan
// 150115035
// Project#3

#ifndef base
#define base

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

typedef struct Text{
	char line[256];
	struct Text *next;
}Text;

typedef struct _intList{
	int key;
	struct _intList *next;
}_intList;

typedef struct _strList{
	char key[48];
	struct _strList *next;
}_strList;

Text *text_header = NULL; 

static int number_of_chars = 0, number_of_lines = 0;

FILE* open(char file_name[48], int open_type);

void close(FILE *f);
void build_text();
void build_text(char *file_name);
void build_informations();
int len_intList(_intList *root);
int len_strList(_strList *root);

int format(char key[64]);
int number_of_chars_in_file(char file_name[48]);
int number_of_lines_in_file(char file_name[48]);
int char_index(char *str_1, char *str_2, int ith);
int find_symbol_in_string(char *string, char symbols[4]);
int is_exist_str(_strList *list, char key[48]);
int is_exist_int(_intList *list, int key);

char* read_file(char *file_name, char text[number_of_chars]);
char* split(char *string, char dest[512], int begin, int end);

Text* allocate_text();
Text* insert_by_key_text(Text **node, char line[256]);

_intList* insert_by_key_intList(_intList **node, int key);
_intList* allocate_intList();


_strList* insert_by_key_strList(_strList **node, char key[48]);
_strList* allocate_strList();

#endif

/*
	A function that build a linkled list 
	that represents a Text line by line
*/
void build_text(char *file_name)
{
	number_of_chars = number_of_chars_in_file(file_name);
	number_of_lines = number_of_lines_in_file(file_name);
	
	char text[number_of_chars], symbols[2] = ":,"; int i;
	read_file("file.txt", text); int first_index, second_index;
	
	char ret[512], dest[256];
	
	for(i = 0, first_index = 0; char_index(text, "\n", i) != -1 ; i++, first_index = second_index+1)
	{
		second_index = char_index(text, "\n", i);
		strcpy(ret, split(text, dest, first_index, second_index));
		insert_by_key_text(&text_header, ret);
	}
}
/*
	A function that finds length of _strList list.
*/
int len_strList(_strList *root)
{
	_strList *interim = root; int ret;
	for(ret = 0; interim != NULL; interim = interim->next){ ret++; }
	return ret;
}
/*
	A function that finds length of _intList list.
*/
int len_intList(_intList *root)
{
	_intList *interim = root; int ret;
	for(ret = 0; interim != NULL; interim = interim->next){ ret++; }
	return ret;
}
/*
	A function that inserts a str to list.
*/
_strList* insert_by_key_strList(_strList **node, char key[48])
{
    _strList *current, *interim, *temporary;
	
	//Create node to insert and assign values to its fields
    current = allocate_strList();
    strcpy(current->key, key);
	current->next = NULL;
	
    //If first node is null
    if(*node == NULL){
        *node = current;
    }	
	else //If list is not empty
	{	
		interim = *node;
		while (interim->next != NULL) 
		{
			interim = interim->next;
		};
		interim->next = current;
	}
	return current;
}
/*
	A function that inserts a int to list.
*/
_intList* insert_by_key_intList(_intList **node, int key)
{
    _intList *current, *interim, *temporary;
	
	//Create node to insert and assign values to its fields
    current = allocate_intList();
    current->key = key;
	current->next = NULL;
	
    //If first node is null
    if(*node == NULL){
        *node = current;
    }	
	else //If list is not empty
	{	
		interim = *node;
		while (interim->next != NULL && interim->next->key < current->key) 
		{
			interim = interim->next;
		};
		current->next = interim->next;
		interim->next = current;
	}
	return current;
}
/*
	A function that inserts a line to text.
*/
Text* insert_by_key_text(Text **node, char line[256])
{
    Text *current, *interim, *temporary;
	
	//Create node to insert and assign values to its fields
    current = allocate_text();
    strcpy(current->line, line);
	current->next = NULL;
	
    //If first node is null
    if(*node == NULL){
        *node = current;
    }	
	else //If list is not empty
	{	
		interim = *node;
		while (interim->next != NULL) 
		{
			interim = interim->next;
		};
		interim->next = current;
	}
	return current;
}
/*
	A function that checks there is a _strList for given key.
*/
int is_exist_str(_strList *list, char key[48])
{
	_strList *tmp = list;
	
	if(tmp == NULL){
		return(0);
	}
	while(tmp != NULL)
	{
		if(strcmp(key, tmp->key) == 0)
		{
			return(1);
		}
		tmp = tmp->next;
	}
	return(0);
}
/*
	A function that checks there is a _intList for given key.
*/
int is_exist_int(_intList *list, int key)
{
	_intList *tmp = list;
	
	if(tmp == NULL){
		return(0);
	}
	while(tmp != NULL)
	{
		if(key == tmp->key)
		{
			return(1);
		}
		tmp = tmp->next;
	}
	return(0);
}
/*
	A function that allocates 
	memory space for _strList Structure
*/
_strList* allocate_strList()
{
	_strList *node = (_strList *)malloc(sizeof(_strList));
	return node;
}
/*
	A function that allocates 
	memory space for _intList Structure
*/
_intList* allocate_intList()
{
	_intList *node = (_intList *)malloc(sizeof(_intList));
	return node;
}
/*
	A function that allocates 
	memory space for Text Structure
*/
Text* allocate_text()
{
	Text *node = (Text *)malloc(sizeof(Text));
	return node;
}
/*
	A function that split a char array between given numbers.
*/
char* split(char *string, char dest[512], int begin, int end)
{
	char ret[end-begin+1], *temp; int i;
	temp = string;
	for(i = 0; i < begin; i++)
	{
		temp++;
	}
	for(i = 0; i < (end-begin+1); i++)
	{
		ret[i] = temp[0];
		temp++;
	}
	for(i = 0; i < (end-begin+1); i++)
	{
		dest[i] = ret[i];
	}
	return dest;
}
/*
	A function that checks to existance of given symbols in given char array.
	It returns '1' as int, if symbols exist in the string.
*/
int find_symbol_in_string(char *string, char symbols[4])
{
	char *interim = string;
	char *temporary = symbols;
	int result = 0;

	for(int j = 0; interim[j] != '\0'; j++)
	{	
		for(int i = 0; temporary[i] != '\0'; i++)
		{	
			if(interim[j] == temporary[i])
			{
				result = 1;
			}
		}
		temporary = symbols;
	}
	return result;
}
/*
	A function that reads a file and write to a char array
*/
char* read_file(char *file_name, char text[number_of_chars]) 
{
	int i = 0, c, j = 0;
	
	FILE *file = open("file.txt", 1); // 'r'
	
	while(i < number_of_chars-1)
	{
		c = fgetc(file);
		text[i++] = (char) c;
	}
	
    close(file);
	
	return text;
}
/*
	A function that calculates 
	number of lines in a file.
*/
int number_of_lines_in_file(char file_name[48])
{
	FILE *file = open(file_name, 1); // "r"
	
	number_of_chars = number_of_chars_in_file("file.txt");
	
	char text[number_of_chars];  int ret, a;
	
	read_file("file.txt", text);
	
	for(ret = 0; char_index(text, "\n", ret) != -1 ; ret++)
	{
		a = char_index(text, "\n", ret);
	}
	
	close(file);

	return ret;
}
/*
	A function that finds index of first char
	into second char. First char must have one character
	also it does not look the other characters.
*/
int char_index(char *str_1, char *str_2, int ith)
{
	int ret = 0, counter = 0;
	
	while(str_1[0] != '\0')
	{
		//printf("\n - %d - \n", ret);
		if(str_1[0] == str_2[0])
		{
			if(counter == ith)
			{
				return ret;
			}
			counter++;
		}
		str_1++; ret++;
	}
	return -1;
}
/*
	A function that calculates the 
	number of characters in a file.
*/
int number_of_chars_in_file(char file_name[48])
{
	FILE *file = open(file_name, 1); // "r"
	
	int ret = 0, c;
	
	for(ret = 0; !feof(file); ret++)
	{
		c = fgetc(file);
	}
	
	close(file);
	
	return ret;
}
/* 
	A function that modifies open file function.
	1 for "r", 2 for "w", 3 for "a"
	11 for "r+", 22 for "w+", 33 for "a+" 
*/
FILE* open(char file_name[48], int open_type)
{
	FILE *f; 
	if(open_type == 1){
		f = fopen("file.txt", "r");
	}else if(open_type == 2){
		f = fopen("file.txt", "w");
	}else if(open_type == 3){
		f = fopen("file.txt", "a");
	}else if(open_type == 11){
		f = fopen("file.txt", "r+");
	}else if(open_type == 22){
		f = fopen("file.txt", "w+");
	}else if(open_type == 33){
		f = fopen("file.txt", "a+");
	}else{
		perror("Wrong input!\n \
		1 for 'r', 2 for 'w', 3 for 'a'\n \
		11 for 'r+', 22 for 'w+', 33 for 'a+'\n");
		exit(-1);
	}
	if(f == NULL) 
	{
		perror("Error in opening file");
		exit(-1);
    }
	return f;
}	
/*
	A function that modifies close file function.
*/
void close(FILE *f)
{
	fclose(f);
}
/*
	A function that is a simple preprocess for given word.
	It deletes symbols, whitespaces and makes the word lowercase.
*/
int format(char key[64])
{
	int i, j;
	for(i = 0; key[i]; i++)
	{
		while (!( (key[i] >= 'a' && key[i] <= 'z') || 
			      (key[i] >= 'A' && key[i] <= 'Z') || 
		          (key[i] == '\0') ||
				  (key[i] >= '0' && key[i] <= '9')) )
        {
            for(j = i; key[j] != '\0'; ++j)
            {
                key[j] = key[j+1];
            }
            key[j] = '\0';
        }
		key[i] = tolower(key[i]);
	}
}