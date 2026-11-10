#include <stdio.h>

int main() {

    int opcao, limpol, yami;

    do{
        printf("\n-----MENU PRINCIPAL-----");
        printf("1- Limpeza");
        printf("2- Alimentos");
        printf("3- Padaria");
        printf("4- Pagamento");
        printf("5- Sair");
        scanf("%f", &opcao);

        switch (opcao){
            
        case 1:
            printf("Produtos de limpeza.");
            printf("11 Detergente R$1.99");
            printf("12 Sabão em Pó (1kg) R$8.99");
            printf("13 Esponja R$1.50");
            printf("14 Amaciante(1Lt) R$15.00");
            printf("15. Voltar ao Menu Principal"); 
            break;
        
        default:  printf("Opcao invalida\n");
                    }
             } while(opcao != 0);
                break;
        
        case 2:
        do
        {
            printf("Alimentos.");
        } while (yami);

    } while(opcao != 0);

    return 0;
}
