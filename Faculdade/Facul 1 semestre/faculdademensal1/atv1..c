#include <stdio.h>

int main () {
    float num1, num2;
    int escolha1, escolha2, soma;
    

    do {

        printf("Escolha a opcao; \n");
        printf("1 - Soma :\n");
        printf("2 - Subtracao :\n");
        printf("3 - Multiplicacao :\n");
        printf("4 - Divisao :\n");
        printf("5 - Sair :\n");
        scanf("%d", &escolha1);

        printf("Escolha o primeiro numero: \n");
        scanf("%f", &num1);

        printf("Escolha o segundo numero: \n");
        scanf("%f", &num2);

        switch (escolha1)
        {
        case 1:
            printf("Resultado: %.2f\n", num1 + num2);
            break;
        
        case 2:
            printf("Resultado: %.2f\n", num1 - num2);
            break;

        case 3:
            printf("Resultado: %.2f\n", num1 * num2);
            break;

        case 4:
            if (num1 != 0)
                printf("Resultado: %.2f\n", num1 + num2);
            else 
                printf("Erro impossivel divisao por 0");
            break;

        case 5:
            printf("Retornando");
            break;

        default:
            printf("Opcao invalida, tente outra opcao. \n");
            break;
        }

    


    } 
    while ();
    
    

    return 0;
}