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
    int size;
    int capacity;
};

#define DEF_CMD(num, name, ...) name = num,

enum push_types
{
    #include "push.def"
};

#undef DEF_CMD

const int BASIC_SIZE_LIST = 5;

int     list_ctor           (List* list);

int     list_dtor           (List* list);

int     list_end_push       (value_t value, List* list);

int     list_start_push     (value_t value, List* list);

int     list_between_push   (value_t value, int ptr, List* list);

int     list_delete         (int ptr, List* list);

int     list_dump           (List* list);

int     list_linearise      (List* list);

int list_push (int type, value_t value, int ptr, List* list);