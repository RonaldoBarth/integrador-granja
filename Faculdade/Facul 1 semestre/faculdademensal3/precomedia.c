#include <stdio.h>

struct Produto {
    char nome[50];
    float preco;
    int quantidade;

};

int main() {
    struct Produto p[5];
    float soma = 0;
    int maior = 0;

    for (int i = 0; i < 5; i++) {
        printf("\nProduto %d\n", i + 1);
        scanf(" %[^\n]", p[i].nome);
        scanf("%f", &p[i].preco);
        scanf("%d", &p[i].quantidade);

        soma += p[i].preco;

        if (p[i].preco > p[maior].preco)
            maior = i;
    } 

    printf("\nProduto mais caro: %s (%.2f)\n", p[maior].nome, p[maior].preco);

    printf("Media dos preco: %.2f\n", soma / 5);

    return 0;
}
