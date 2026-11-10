#include <stdio.h>

int main(){
    float x, y, imc;

    printf("Coloque seu peso:");
    scanf("%f", &x);

    printf("Coloque sua altura:");
    scanf("%f", &y);

    imc = x / (y * y);

    if (imc >=36) {
        printf("Super Obesidade: %2.f\n", imc);
    }
    else if (imc >= 30 ) {
        printf("Obesidade: %2.f\n", imc);
    }
    else if (imc >= 24 ) {
        printf("Acima do peso: %2.f\n", imc);
    }
    else if (imc <= 24 ) {
        printf("Peso normal: %2.f\n", imc);
    }

    return 0;
} 