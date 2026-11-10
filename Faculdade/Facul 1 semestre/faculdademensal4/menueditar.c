#include <stdio.h>
#include <stdlib.h>

struct listNode {
    char data;
    struct listNode *prox;
};

typedef struct listNode LISTNODE;
typedef LISTNODE *LISTNODEPTR;

void insert(LISTNODEPTR *, char);
char delete(LISTNODEPTR *, char);
int isEmpty(LISTNODEPTR);
void printList(LISTNODEPTR);
void showMenu(void);

int main() {
    LISTNODEPTR startPtr = NULL;
    int op;
    char item;

    showMenu();
    printf("Insira uma opcao: ");
    scanf("%d", &op);

    while(op != 3) {
        
    }

}