#include <stdio.h>

int main() {
    int valor[5];
    int maior, menor;

    printf("Digite 5 numeros: \n");
    for (int i = 0; i < 5; i++) {
        scanf("%d", &valor[i]);
    }
    maior = menor = valor[0];

    for(int i = 1; i < 5; i++ ) {
        if (valor[i] > maior) maior = valor[i];
        if (valor[i] < menor) menor = valor[i];
    }

    printf("Maior valor = %d\n", maior);
    printf("Menor valor = %d\n", menor);

    return 0;
}