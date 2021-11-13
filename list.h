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
    const char* error;
};

enum Errors
{
    NO_ERROR,
    TAIL_VAL_ERROR,
    HEAD_VAL_ERROR,
    SIZE_VAL_ERROR,
    FREE_VAL_ERROR,
    CAPACITY_VAL_ERROR,
    LIST_PTR_ERROR,
    PREV_PTR_ERROR,
    NEXT_PTR_ERROR,
    DATA_PTR_ERROR,
    NEXT_DATA_VAL_ERROR,
    NEXT_FREE_VAL_ERROR,
    PREV_VAL_ERROR,
    NEXT_PREV_VAL_ERROR
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

int     list_dump           (const List* list, const char* file, const int line, const char* funct);

int     list_linearise      (List* list);

int     list_push           (int type, value_t value, int ptr, List* list);

/*int     list_end_push       (value_t value, List* list);

int     list_start_push     (value_t value, List* list);

int     list_between_push   (value_t value, int ptr, List* list);*/