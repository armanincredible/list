#include "list.h"

#define CHECK_ERROR_(name, type_error, ret)                                             \
        do{                                                                             \
            if (name == NULL)                                                           \
            {                                                                           \
                printf ("\nERROR in function : %s \n"                                   \
                        "%s have %s\n", __func__,                                       \
                        #name, type_error);                                             \
                return ret;                                                             \
            }                                                                           \
        } while (0)


int list_ctor (List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    list->data = (value_t*) calloc (SIZE_LIST_DATA + SIZE_FREE, sizeof (value_t));
    CHECK_ERROR_(list->data, "NULL ADRESS (HAVENT MEMORY)", -1);
    
    list->next = (int*) calloc (SIZE_LIST_DATA + SIZE_FREE, sizeof (int));
    CHECK_ERROR_(list->next, "NULL ADRESS (HAVENT MEMORY)", -1);

    list->prev = (int*) calloc (SIZE_LIST_DATA + SIZE_FREE, sizeof (int));
    CHECK_ERROR_(list->prev, "NULL ADRESS (HAVENT MEMORY)", -1);

    memset (list->data, 0, SIZE_LIST_DATA + SIZE_FREE);

    list->next[0] = 0;
    list->prev[0] = 0;

    memset (list->next + 1, -1, SIZE_LIST_DATA + SIZE_FREE - 1);
    memset (list->prev + 1, -1, SIZE_LIST_DATA + SIZE_FREE - 1);

    list->head = 1;
    list->tail = 1;
    list->free = 1;

    return 0;

}

int list_end_push (value_t value, List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);
    /*validator*/
    if (list->tail == 1)
    {
        list->prev[list->head] = 0;
    }

    list->next[list->free] = 0;
    list->data[list->free] = value;

    list->next[list->tail] = list->free;
    list->prev[list->free] = list->tail;
    
    list->tail = list->free;
    list->free = list->free + 1;

    return 0;
    
}

int list_start_push (value_t value, List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    list->data[list->free] = value;
    list->next[list->free] = list->head;
    list->prev[list->free] = 0;

    list->prev[list->head] = list->free;

    list->head = list->free;
    list->free = list->free + 1;

    return 0;
}

int list_between_push (value_t value, int ptr, List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    list->next[list->free] = list->next[ptr];
    list->prev[list->free] = ptr;
    list->data[list->free] = value;

    list->prev[list->next[ptr]] = list->free;

    list->next[ptr] = list->free;

    list->free = list->free + 1;

    return 0;
}

int list_delete (int ptr, List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    if (ptr == list->tail)
    {
        list->next[list->prev[ptr]] == 0;
    }
    else if (ptr == list->head)
    {
        list->head = list->next[list->head];
    }
    else
    {
        list->next[list->prev[ptr]] = list->next[ptr];
        list->prev[list->next[ptr]] = list->prev[ptr];
    }

    list->data[ptr] = 0;
    list->next[ptr] = -1;
    list->prev[ptr] = -1;

    list->free = list->free - 1;

    return 0;
}






/*int list_dtor (List* list){}*/


#undef CHECK_ERROR_