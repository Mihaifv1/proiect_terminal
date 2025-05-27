#include <stdio.h>
#include <stdlib.h>

struct argument{
    char tip;    // 'f' - flag, 'a' - argument, 'o' - opțiune
    void* valoare;  // pointer la valoare
    // pentru 'a' și 'o' este pointer un șir de caractere
    // pentru 'f' este pointer la o valoare numerică, 0 sau 1
	char format;
    char *val_opt;
};
typedef struct argument Argument;

struct lista {
	Argument data;
	struct lista *urm;
};

typedef struct lista Lista;

void adaugare_element_final(Lista **head, Argument arg);
void adaugare_element_inceput(Lista **cap, Argument arg);
void sterge_element_inceput(Lista **cap);
void sterge_element_final(Lista **cap);
int eGol(Lista **cap);
void sterge_lista(Lista **cap);
