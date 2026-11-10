#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int data;          // campo para guardar o valor
    struct No *prox;          // campo para apontar pro próximo nó
} No;

No* inserirInicio(No *head, int val) {
    No *novo = malloc(sizeof(No));
    novo->data = val;
    novo->prox = head;
    return novo;
}

int contar(No *head) {
    int total = 0;
    No *p = head;           // p começa no primeiro nó

    while (p != NULL) {     // enquanto não chegou no fim
        total++;            // conta esse nó
        p = p->prox;        // pula pro próximo
    }

    return total;
}
 int main() {
    No no;

    inserirInicio;
    contar;

    return 0;
 }