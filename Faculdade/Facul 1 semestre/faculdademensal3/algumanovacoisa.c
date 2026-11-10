#include <stdio.h>

struct dadosClientes
{
    int numConta;
    char nome[20];
    char sobrenome[20];
    float saldo;
};

int main() {
    int i;
    struct dadosClientes clientesNulo = { 0 , "" , "", 0.0 };
    FILE *cfPtr;

    if ((cfPtr = fopen("credito.dat", "wb")) == NULL) {
        printf("O arquivo nao pode ser aberto\n");
    } else {
        for (i = 1; i <= 100; i++)
            fwrite(&clientesNulo, sizeof(struct dadosClientes), 1, cfPtr);

        fclose(cfPtr);
    }

    return 0;
}