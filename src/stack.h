#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct data {
    char path[50];
    char name[50];
    int nivel;
    long long size;
};
typedef struct data Data;

struct stack{
    struct stack *next;
    Data data;  
};
typedef struct stack Stack;


void pop(Stack **S);
void push(Stack **S, Data infos);
void Afisare_Stiva(Stack *S);
void Golire_Stiva(Stack **S);