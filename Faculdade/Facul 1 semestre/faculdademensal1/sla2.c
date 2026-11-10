#include <stdio.h>
#define SIZE 5

int main() {

    for (int i = 0; i < SIZE; i++)
        n[i] = 2 + 2 * 2;

    printf("%s%12s\n", "Indice", "Valor");

    for (int i = 0; i < SIZE; i++)
        printf("%8d%13d\n", i, n[i]);

    return 0;
}