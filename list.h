#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h> 

typedef int value_t;

struct List 
{
    value_t *data;
    int* prev;
    int* next;
    int free;
    int head;
    int tail;
};

const int SIZE_LIST_DATA = 10;
const int SIZE_FREE = 10;

int list_ctor (List* list);

int list_dtor (List* list);

int list_end_push (value_t value, List* list);

int list_start_push (value_t value, List* list);

int list_between_push (value_t value, int ptr, List* list);

int list_delete (int ptr, List* list);
