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

#define $ printf("\t\t\t---ON LINE %d IN FUNCTION %s---\n", __LINE__, __func__);

static int list_resize (List *list);

static int size_cmp_resize (List *list);

static int list_verificate (List* list);

int list_ctor (List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    list->data = (value_t*) calloc (BASIC_SIZE_LIST, sizeof (value_t));
    CHECK_ERROR_(list->data, "NULL ADRESS (HAVENT MEMORY)", -1);
    
    list->next = (int*) calloc (BASIC_SIZE_LIST, sizeof (int));
    CHECK_ERROR_(list->next, "NULL ADRESS (HAVENT MEMORY)", -1);

    list->prev = (int*) calloc (BASIC_SIZE_LIST, sizeof (int));
    CHECK_ERROR_(list->prev, "NULL ADRESS (HAVENT MEMORY)", -1);

    memset ((void*)list->data, 0, BASIC_SIZE_LIST * sizeof (value_t));

    list->next[0] = 0;
    list->prev[0] = 0;

    memset ((void*)(list->next + 1), -1, (BASIC_SIZE_LIST - 1) * sizeof (int));
    memset ((void*)(list->prev + 1), -1, (BASIC_SIZE_LIST - 1) * sizeof (int));

    list->head = 1;
    list->tail = 1;
    list->free = 1;
    list->size = 0;
    list->capacity = BASIC_SIZE_LIST;

    for (int i = 1; i < BASIC_SIZE_LIST; i++)
    {
        list->next[i] = i + 1;
    }
    list->next[BASIC_SIZE_LIST - 1] = 0;

    return 0;
}

int list_dtor (List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    memset ((void*) list->data,        0, list->capacity * sizeof (value_t));
    memset ((void*) (list->next + 1), -1, list->capacity * sizeof (value_t));
    memset ((void*) (list->prev + 1), -1, list->capacity * sizeof (value_t));

    return 0;
}

int list_end_push (value_t value, List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);
    /*validator*/
    size_cmp_resize (list);

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

    list->size = list->size + 1;

    return 0;
    
}

int list_start_push (value_t value, List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    size_cmp_resize (list);

    list->data[list->free] = value;
    list->next[list->free] = list->head;
    list->prev[list->free] = 0;

    list->prev[list->head] = list->free;

    list->head = list->free;
    list->free = list->free + 1;

    list->size = list->size + 1;

    return 0;
}

int list_between_push (value_t value, int ptr, List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    size_cmp_resize (list);

    list->next[list->free] = list->next[ptr];
    list->prev[list->free] = ptr;
    list->data[list->free] = value;

    list->prev[list->next[ptr]] = list->free;

    list->next[ptr] = list->free;

    list->free = list->free + 1;

    list->size = list->size + 1;

    return 0;
}

int list_delete (int ptr, List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    if (ptr == list->tail)
    {
        list->next[list->prev[ptr]] = 0;
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

    list->next[ptr] = list->free;
    list->prev[ptr] = 0;

    list->data[ptr] = 0;

    list->free = ptr;

    list->size = list->size - 1;

    return 0;
}

static int list_resize (List *list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);
    
    list->capacity = list->capacity * 2;

    list->data = (value_t*) realloc ((void*)list->data, list->capacity * sizeof (value_t));
    CHECK_ERROR_(list->data, "NULL ADRESS (haven't memory) ", -1);

    list->next = (int*) realloc ((void*)list->next, list->capacity * sizeof (int));
    CHECK_ERROR_(list->next, "NULL ADRESS (haven't memory) ", -1);

    list->prev = (int*) realloc ((void*)list->prev, list->capacity * sizeof (int));
    CHECK_ERROR_(list->next, "NULL ADRESS (haven't memory) ", -1);

    for (int i = list->size + 1; i < list->capacity; i++)
    {
        list->next[i] = i + 1;
    }
    list->next[list->capacity - 1] = 0;

    memset ((void*)(list->data + list->size + 1), 0, (list->capacity - list->size - 1) * sizeof (value_t));

    memset ((void*)(list->prev + list->size + 1), 0, (list->capacity - list->size - 1) * sizeof (int));

    return 0;
}

static int size_cmp_resize (List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

     if (list->size >= list->capacity - 1)
    {
        list_resize(list);
    }
    return 0;
}

int list_dump (List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    FILE* output_file = fopen ("../list/list_dump.dot", "w");
    CHECK_ERROR_(output_file, "NOT CREATED", -1);

    fprintf (output_file, "\tdigraph dump_graph{\n"
                          "\trankdir=HR;\n");

    for (int i = 0; i < list->capacity; i++)
    {
        fprintf (output_file, "\t%d [shape = \"ellipse\", label = \" %d\\nvalue %d, next %d, prev %d\"];\n", i, i, list->data[i], list->next[i], list->prev[i]);
        if (i > 1)
        {
            fprintf(output_file, "\t%d -> %d[style=invis];\n", i - 1, i);
        }
    }

    fprintf (output_file, "\tINFO [shape = record, label = \" <hd> HEAD | <tl> TAIL | <fr> FREE\"];\n");
    fprintf (output_file, "\tINFO: <hd> -> %d [color= \"blue\"];\n", list->head);
    fprintf (output_file, "\tINFO: <tl> -> %d [color= \"blue\"];\n", list->tail);
    fprintf (output_file, "\tINFO: <fr> -> %d [color= \"red\"];\n", list->free);

    int ptr = list->head;

    for (int i = 1; (i < list->capacity) && (list->next[ptr] != 0); i++)
    {
        fprintf (output_file, "\t%d -> %d [color= \"blue\"];\n", ptr, list->next[ptr]);
        ptr = list->next[ptr];
    }

    ptr = list->free;
    
    for (int i = list->free; (i < list->capacity) && (list->next[ptr] != 0); i++)
    {
        fprintf (output_file, "\t%d -> %d [color= \"red\"];\n", ptr, list->next[ptr]);
        ptr = list->next[ptr];
    }

    fprintf(output_file, "}");

    if (ferror(output_file))
    {
        printf ("ERROR in function : %s \n"
                "writing file falled\n", __func__);
        return -1;
    }
    
    fclose (output_file);

    system ("\"C:/Program Files/Graphviz/bin/dot.exe\" -Tpng list_dump.dot -o graph.png");

    return 0;
}

static int list_verificate (List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);
}

#undef $


#undef CHECK_ERROR_