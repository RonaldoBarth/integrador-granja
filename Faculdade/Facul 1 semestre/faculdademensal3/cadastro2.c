#include <stdio.h>

int main() {
    int conta;
    char nome[40];
    float saldo;
    FILE *cfPtr

    if ((cfPtr = fopen("cliente.dat", "r")) == NULL) {
        printf("O arquivo nao pode ser aberto.\n");
    } else {
        printf("%-10s%-13s%s\n", "conta", "nome", "saldo");
    
        while(fscanf(cfPtr, "%d%s%f", &conta, nome, &saldo) == 3) {
            printf("%-10d%-13s%7.2f\n", conta, nome, saldo);
        }

        fclose(cfPtr);
    }

    return 0;
}