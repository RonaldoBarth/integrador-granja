#include <stdio.h>

int main() {
    for (int k = 1; k < 10; k++);
    printf("%d", k);
    int x;
    
    
    printf("Coloque sua idade:");
    scanf("%d", &x);

    if (x >= 18){
        printf("Maior de idade");
    } else { printf("Menor de idade");

    }

    return 0;
}