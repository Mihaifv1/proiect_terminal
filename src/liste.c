#include "liste.h"

void adaugare_element_inceput(Lista **cap, Argument arg)
{
    Lista *nou;
    nou = (Lista *)malloc(sizeof(Lista));
    nou->urm = *cap;
    nou->data = arg;
    (*cap) = nou;
    
}

void adaugare_element_final(Lista **head, Argument arg)
{   
    // daca lista este vida, se modifica adresa de inceput
    if (*head == NULL) 
    {
        adaugare_element_inceput(head, arg);
        return;
    }
    
    Lista *aux = *head;
    Lista *newNode = (Lista*)malloc(sizeof(Lista));
    newNode->data = arg; 
    
    while (aux->urm != NULL) 
        aux = aux->urm;
    
    aux->urm = newNode;
    newNode->urm = NULL; 
    
}


int eGol(Lista **cap)
{
    if((*cap) == NULL)
        return 1;
    return 0;
}

void sterge_lista(Lista **cap)
{
    while( *cap != NULL)
    {
        Lista *del;
        del = *cap;
        (*cap) = (*cap) -> urm;
        free(del->data.valoare);
        free(del->data.val_opt);
        free(del);
    }
    *cap = NULL;
}

void sterge_element_inceput(Lista **cap)
{
    Lista *del = *cap;
    *cap = (*cap)->urm;
    free(del->data.valoare);
    free(del->data.val_opt);    
    free(del);
}
void sterge_element_final(Lista **cap)
{
    Lista *p;
    p = *cap;
    while(p->urm->urm != NULL)
        p = p->urm;
    Lista *ult;
    ult = p->urm;
    p->urm = NULL;
    free(ult->data.valoare);
    free(ult->data.val_opt);
    free(ult);
}