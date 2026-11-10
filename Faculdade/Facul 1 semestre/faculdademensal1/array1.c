#include <stdio.h>

int main() {
    int n[10], i;

    for (i = 0; i < 10; i++){
        n[i] = i + 1;
    }

    printf("%s%14s\n", "indice", "Valor");

    for (i = 0; i < 10; i++)
        printf("%7d%13d\n", i, n[i]);

    return 0;

}