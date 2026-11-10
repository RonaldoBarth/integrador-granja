#include <stdio.h>

int main(){

    int x, y, z, p1, p2;

    printf("Insira o primeiro numero: ");
    scanf("%d", &x);

    printf("Insira o primeiro segundo: ");
    scanf("%d", &y);

    printf("Insira o primeiro terceiro: ");
    scanf("%d", &z);

    p1 = z - x * y;
    p2 = (z + y) * x;

    printf("Resultado p1: %d\n", p1);
    printf("Resultado p2: %d\n", p2);

    return 0;
}