#include <stdio.h>

int main() {
    int num[5];
    int soma = 0;

    printf("Digite 5 numeros inteiros:  \n");

    for (int i = 0; i < 5; i++) {
        scanf("%d", &num[i]);
        soma += num[i];
    }

    printf("Soma dos elementos = %d\n", soma);

    return 0;
}