#include <stdio.h>
#include <stdlib.h>

int main() {
    int *vetor;
    int n, i;

    printf("Quantos numeros voce deseja armazenar? ");
    scanf("%d", &n);

    vetor = (int *)malloc(n * sizeof(int));

    if (vetor == NULL) {
        printf("Erro de alocacao de memoria!\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("Digite o numero %d: ", i + 1);
        scanf("%d", &vetor[i]);
    }
    printf("VOce digitou:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", vetor[1]);
    }
    printf("\n");

    printf("%d", sizeof(vetor));

    free(vetor);

    return 0;
}