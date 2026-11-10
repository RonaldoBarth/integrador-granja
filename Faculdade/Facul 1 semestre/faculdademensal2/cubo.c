#include <stdio.h>

int cuboPorValor(int);

int main() {
    int n = 5;
    int c;

    printf("O numero original do numero eh %d\n", n);
    c = cuboPorValor(n);

    printf("O novo valor do numero eh %d\n", n);
    printf("O valor de C? %d\n", c);

    return 0;
}

int cuboPorValor(int n) {
    n = 2;
    return n * n * n;
}