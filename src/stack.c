#include "stack.h"

void push(Stack **S, Data infos)
{   
    if((*S) == NULL)
    {
        *S = (Stack*)malloc(sizeof(Stack));
        strcpy((*S)->data.name, infos.name);
        (*S)->data.size = infos.size;
        (*S)->data.nivel = infos.nivel;
        (*S)->next = NULL;
    }
    else
    {
        Stack *nou;
        nou = (Stack*)malloc(sizeof(Stack));
        strcpy(nou->data.name, infos.name);
        nou->data.size = infos.size;
        nou->data.nivel = infos.nivel;
        nou->next = *S;
        (*S) = nou;
    }
}

void pop(Stack **S)
{
    if((*S)->next == NULL || *S == NULL)
    {
        free(*S);
        *S = NULL;
    }
    else
    {
        Stack *del;
        del = *S;
        (*S) = (*S)->next;
        free(del);
    }
}

void Afisare_Stiva(Stack *S)
{
    Stack *p;
    p = S;
    int i;
    while(p != NULL)
    {
        for(i = 0; i < p->data.nivel; i++)
            printf("    ");
        printf("%s %d %lld \n", p->data.name, p->data.nivel, p->data.size);
        p = p->next;
    }
}

void Golire_Stiva(Stack **S)
{
    while(*S != NULL)
        pop(S);
    *S = NULL;
}