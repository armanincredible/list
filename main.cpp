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


int main ()
{
    List list = {0, 0, 0, 0, 0, 0, 0, 0};
    list_ctor (&list);
    for (int i = 1; i < 10; i++)
    {
        list_end_push (i, &list);
    }


    list_start_push(20, &list);

    list_between_push (30, 3, &list);

    list_delete (5, &list);

    list_delete (8, &list);

    list_dump (&list);
    

    list_dtor (&list);

    return 0;
}

#undef CHECK_ERROR_
