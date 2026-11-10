#include <stdio.h>

int main() {
    int conta;
    char nome[40];
    float saldo;
    FILE *cfPtr;

    if ((cfPtr = fopen("clientes.dat", "w")) == NULL) {
        printf("O arquivo nao pode ser aberto\n");
        return 1;
    }

    printf("Digite a conta, o nome e o saldo.\n");
    printf("Use Ctrl+D (Linux/Mac) ou Ctrl+Z(Windows) para encerrar.");

    while (scanf("%d %39s %f", &conta, nome, &saldo) == 3) {
        fprintf(cfPtr, "%d %s %.2f\n", conta, nome, saldo);
        printf(" ?");
    }
    
    fclose(cfPtr);
    printf("Dados gravados com sucesso em 'clientes.dat'.");


    return 0;
}