#include <stdio.h>

int main(){
    int nota;

    printf("insira a nota: \n");
    scanf("%d", &nota);

    nota >= 70 ? printf("Aprovado") : printf("Reprovado");

    return 0;
}