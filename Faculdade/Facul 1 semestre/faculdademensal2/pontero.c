#include <stdio.h>

void cuboPorReferencia(int *);

int main() {
    int n= 5;

    printf("O valor original do numero eh %d\n", n);

    cuboPorReferencia(&n);
    printf("O novo valor do numero eh %d\n", n);

    return 0;
}

void cuboPorReferencia(int *nPtr) {
    *nPtr = *nPtr * *nPtr * *nPtr;
}