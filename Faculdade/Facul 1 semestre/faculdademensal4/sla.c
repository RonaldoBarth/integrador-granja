#include <stdio.h>

struct dadosClientes
{
    int numConta;
    char nome[20];
    char sobrenome[20];
    float saldo;
};

int main() {
    struct dadosClientes cliente;
    FILE *cfPtr;

    if ((cfPtr = fopen("credito.dat", "a+")) == NULL) {
        printf("O arquivo nao pode ser aberto.\n");
    } else {
        printf("Insira o numero da conta (1 a 100, 0 para encerrar)\n");
        scanf("%d", &cliente.numConta);

    
    while (cliente.numConta != 0) {
        printf("Insira o nome, sobrenome e saldo: ");
        scanf("%s %s %f", &cliente.nome, &cliente.sobrenome, &cliente.saldo);

        fseek(cfPtr, (cliente.numConta - 1) * sizeof(struct dadosClientes), SEEK_SET);
        fwrite(&cliente, sizeof(struct dadosClientes), 1, cfPtr);

        printf("Insira o numero da conta\n");
        scanf("%d", &cliente.numConta);
        }

    }

    fclose(cfPtr);

    return 0;
}
