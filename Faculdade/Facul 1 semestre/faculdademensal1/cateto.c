#include <stdio.h>
#include <math.h>

int main() {

    float cateto1, cateto2;
    char op;

    printf("INsira o valor do cateto 1:");
    scanf("%f", &cateto1);

    printf("INsira o valor do cateto 2:");
    scanf("%f", &cateto2);

    do {
        printf("h -> para ver a hipotenusa:\n");
        printf("p -> para ver o perimetro\n");
        printf("a -> para ver a area\n");
        printf("f -> para finalizar\n");
        scanf("%c", &op);

        switch (op) {
        
        case 'h' :
            printf("A hipotenusa e: %.2f", sqrt (pow(cateto1, 2) + pow(cateto2, 2)));
            break;

        case 'p' :
            printf("O perimetro e: %.2f", cateto1 + cateto2 + sqrt(pow(cateto1, 2) + pow(cateto2, 2)));
            break;
        
        case 'a' :
            printf("A area e: %.2f", (cateto1 * cateto2) / 2);
            break;
            
        case 'f' :
            printf("Programa finalizado\n");
            break;

        default:
            printf("Opcao invalida! Escolha outra opcao\n");
            break;
        }

        }while (op != 'f');
    
   return 0;  
} 