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

#define _VERIFICATE_                                                                    \
        do{                                                                             \
            int error = list_verificate (list);                                         \
                                                                                        \
            if (error == -1)                                                            \
            {                                                                           \
                return -1;                                                              \
            }                                                                           \
        } while (0)

static int  list_resize      (List *list);

static int  size_cmp_resize  (List *list);

static int  list_verificate  (List* list);

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

    list_verificate (list);

    return 0;
}

int list_dtor (List* list)
{
    CHECK_ERROR_(list,       "NULL ADRESS", -1);
    CHECK_ERROR_(list->data, "NULL ADRESS", -1);
    CHECK_ERROR_(list->next, "NULL ADRESS", -1);
    CHECK_ERROR_(list->prev, "NULL ADRESS", -1);

    _VERIFICATE_;

    memset ((void*) list->data,        0, list->capacity * sizeof (value_t));
    memset ((void*) (list->next + 1), -1, list->capacity * sizeof (value_t));
    memset ((void*) (list->prev + 1), -1, list->capacity * sizeof (value_t));

    free (list->data);
    free (list->next);
    free (list->prev);

    list->size     = -1;
    list->capacity = -1;
    list->free     = -1;
    list->head     = -1;
    list->tail     = -1;

    return 0;
}


int list_push (int type, value_t value, int ptr, List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    _VERIFICATE_;

    size_cmp_resize (list);

    int save_next_of_free = list->next[list->free];

    #define DEF_CMD(num, name, arg)     \
            if (type == name)           \
            {                           \
                arg                     \
            }                       

    #include "push.def"

    #undef DEF_CMD

    if (save_next_of_free != 0)
    {
        list->free = save_next_of_free;
    }
    list->size = list->size + 1;

    _VERIFICATE_;
    
    return 0;
}


int list_delete (int ptr, List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    _VERIFICATE_;

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

    _VERIFICATE_;

    return 0;
}

static int list_resize (List *list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

    _VERIFICATE_;
    
    list->capacity = list->capacity * 2;

    list->data = (value_t*) realloc ((void*)list->data, list->capacity * sizeof (value_t));
    CHECK_ERROR_(list->data, "NULL ADRESS (haven't memory) ", -1);

    list->next = (int*) realloc ((void*)list->next, list->capacity * sizeof (int));
    CHECK_ERROR_(list->next, "NULL ADRESS (haven't memory) ", -1);

    list->prev = (int*) realloc ((void*)list->prev, list->capacity * sizeof (int));
    CHECK_ERROR_(list->next, "NULL ADRESS (haven't memory) ", -1);

    list->free = list->size + 1;

    for (int i = list->size + 1; i < list->capacity; i++)
    {
        list->next[i] = i + 1;
    }
    list->next[list->capacity - 1] = 0;

    memset ((void*)(list->data + list->size + 1), 0, (list->capacity - list->size - 1) * sizeof (value_t));

    memset ((void*)(list->prev + list->size + 1), 0, (list->capacity - list->size - 1) * sizeof (int));

    _VERIFICATE_;

    return 0;
}

static int size_cmp_resize (List* list)
{
    CHECK_ERROR_(list, "NULL ADRESS", -1);

     if (list->size >= list->capacity - 2)
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

    for (int i = 1; (i <= list->size) && (list->next[ptr] != 0); i++)
    {
        fprintf (output_file, "\t%d -> %d [color= \"blue\"];\n", ptr, list->next[ptr]);
        ptr = list->next[ptr];
    }

    ptr = list->free;
    
    for (int i = list->free; (i < list->capacity - 1) && (list->next[ptr] != 0); i++)
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

int list_linearise (List* list)
{
    CHECK_ERROR_ (list, "NULL ADRESS", -1);

    if (list->size == 0)
    {
        printf ("TI KONCHENNIE");
        return -1;
    }
    _VERIFICATE_;

    value_t* save_ptr_data = list->data;
    /*int* save_ptr_prev = list->prev;*/
    int* save_ptr_next = list->next;

    list->data = (value_t*) calloc (list->capacity, sizeof (value_t));
    CHECK_ERROR_(list->data, "NULL ADRESS (HAVENT MEMORY)", -1);
    
    list->next = (int*) calloc (list->capacity, sizeof (int));
    CHECK_ERROR_(list->next, "NULL ADRESS (HAVENT MEMORY)", -1);

    /*list->prev = (int*) calloc (list->capacity, sizeof (int));
    CHECK_ERROR_(list->prev, "NULL ADRESS (HAVENT MEMORY)", -1);*/

    memset ((void*)list->data, 0, list->capacity * sizeof (value_t));
    memset ((void*)(list->next + 1), -1, (list->capacity - 1) * sizeof (int));

    int cur_operation = 1;

    list->next[0] = 0;
    list->prev[0] = 0;
    list->prev[1] = 0;

    list->tail = list->size;
    list->free = list->size + 1;

    for (int i = list->head; cur_operation != list->capacity; i = save_ptr_next[i])
    {
        list->data[cur_operation] = save_ptr_data[i];

        list->next[cur_operation] = cur_operation + 1;
        

        if ((cur_operation == list->size) || (cur_operation == list->capacity - 1))
        {
            list->next[cur_operation] = 0;
        }

        if ((cur_operation > 1) && (cur_operation <= list->size))
        {
            list->prev[cur_operation] = cur_operation - 1;
        }

        if (cur_operation > list->size)
        {
            list->prev[cur_operation] = -1;
        }

        cur_operation++;
    }

    list->head = 1;

    free(save_ptr_data);
    /*free(save_ptr_prev);*/
    free(save_ptr_next);

    _VERIFICATE_;

    return 0;
}

static int list_verificate (List* list)
{
    CHECK_ERROR_ (list,       "NULL ADRESS", -1);
    CHECK_ERROR_ (list->data, "NULL ADRESS", -1);
    CHECK_ERROR_ (list->prev, "NULL ADRESS", -1);
    CHECK_ERROR_ (list->next, "NULL ADRESS", -1);

    if ((list->head     <= 0) || (list->tail <= 0) || 
        (list->capacity <= 0) || (list->size <  0) || (list->free <= 0))
    {
        printf ("list have ERROR %d\n", __LINE__);
        return -1;
    }   


    int cur_oper = 0;

    for (int i = 1; cur_oper < list->size; i = list->next[i])
    {
        if ((list->next[i] == 0) && (i != list->tail))
        {
            printf ("list have ERROR %d\n", __LINE__);
            return -1;
        }

        if (list->next[i] == 0)
        {
            break;
        }


        if ((list->size > 0) && (i != list->prev[list->next[i]]))
        {
            printf ("list have ERROR %d\n", __LINE__);
            return -1;
        }

        if ((i > list->head) && (i != list->next[list->prev[i]]))
        {
            printf ("list have ERROR %d\n", __LINE__);
            return -1;
        }

        cur_oper++;
    }

    cur_oper = 0;
    bool have_end = 0;

    for (int i = list->free; cur_oper < list->capacity - list->size; i = list->next[i])
    {
        if (list->size != list->capacity - 1)
        {
            if (list->data[i] != 0)
            {
                printf ("list have ERROR %d\n", __LINE__);
                return -1;
            }
            if (list->next[i] == 0)
            {
                have_end = 1;
                break;
            }
            cur_oper++;
        }
        else 
        {
            have_end = 1;
            break;
        }
    }

    if (have_end == 0)
    {
        printf ("list have ERROR %d\n", __LINE__);
        return -1;
    }

    return 0;
}

#undef $

#undef CHECK_ERROR_

#undef _VERIFICATE_