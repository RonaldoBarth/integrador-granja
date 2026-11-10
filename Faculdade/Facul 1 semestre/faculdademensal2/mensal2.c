#include <stdio.h>

int main () {
    int opc, produto, cartao;
    int p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0, p6 = 0, p7 = 0; // Estoque padaria //
    int l1 = 50, l2 = 30, l3 = 0, l4 = 20, l5 = 100, l6 = 15, l7 = 60; // Estoque limpeza //
    int a1 = 10, a2 = 15, a3 = 10, a4 = 5, a5 = 20, a6 = 6, a7 = 15; // Estoque alimento //
    int limp = 0, comi = 0, pao = 0, pag;
    float vendas = 0, quantidade = 0, dindin = 0, start = 0;
    float subtotal = 0.0, sublimp = 0.0, subcomi = 0.0, subpao = 0.0;

    do {

        printf("\n---- MENU PRINCIPAL ----\n ");
        printf("1 - Material de Limpeza\n ");
        printf("2 - Venda de Alimentos\n ");
        printf("3 - Padaria\n ");
        printf("4 - Pagamento\n ");
        printf("5 - Abertura de Caixa\n ");
        printf("6 - Fechamento de Caixa\n "); 
        printf("Escolha:  ");
        scanf("%d", &opc);

    switch(opc) {
    
        case 1: while (limp != 18)
        {
           printf("\n---- Setor de Limpeza ---\n ");
            printf("11 - Detergente R$1.99 (%d)\n ", l1);
            printf("12 - Sabão em Pó (1kg) R$8.99 (%d)\n ", l2);
            printf("13 - Esponja R$1.50 (%d)\n ", l3);
            printf("14 - Amaciante(1Lt) R$15.00 (%d)\n ", l4);
            printf("15 - Bucha de Pia (kit c/3) R$4.99 (%d)\n ", l5);
            printf("16 - Desinfetante(1Lt) R$7.99 (%d)\n ", l6);
            printf("17 - Sabão em Barra(Un) R$1.00 (%d)\n ", l7);
            printf("18 - MENU PRINCIPAL\n ");
            printf("Escolha: \n");
                scanf("%d", &limp);

            switch (limp)
            {
                case 11:
                    printf("Detergente selecionado. Quantidade? ");
                    scanf("%f", &quantidade);
                    if (quantidade <= l1) {
                    l1 = quantidade - l1;
                    vendas += quantidade;
                    subtotal += (1.99 * quantidade);
                    sublimp += (1.99 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", l1);
                    break;
                
                case 12:
                    printf("Sabão em Pó selecionado. Quantidade ?");
                    scanf("%f", &quantidade);
                    if (quantidade <= l2) {
                    l2 = quantidade - l2;
                    vendas += quantidade;
                    subtotal += (8.99 * quantidade);
                    sublimp += (8.99 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", l2);
                    break;

                case 13:
                     printf("Esponja selecionado. Quantidade ?");
                    scanf("%f", &quantidade);
                    if (quantidade <= l3) {
                    l3 = quantidade - l3;
                    vendas += quantidade;
                    subtotal += (8.99 * quantidade);
                    sublimp += (8.99 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", l3);
                    break;

                case 14:
                    printf("Amaciante selecionado. Quantidade? ");
                    scanf("%f", &quantidade);
                    if (quantidade <= l4) {
                    l4 = quantidade - l4;
                    vendas += quantidade;
                    subtotal += (15.00 * quantidade);
                    sublimp += (15.00 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", l4);
                    break;

                case 15:
                    printf("Bucha de pia selecionado. Quantidade ?");
                    scanf("%f", &quantidade);
                    if (quantidade <= l5) {
                    l5 = quantidade - l5;
                    vendas += quantidade;
                    subtotal += (4.99 * quantidade);
                    sublimp += (4.99 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", l5);
                    break;

                case 16:
                    printf("Desinfetante selecionado. Quantidade ?");
                    scanf("%f", &quantidade);
                    if (quantidade <= l6) {
                    l6 = quantidade - l6;
                    vendas += quantidade;
                    subtotal += (7.99 * quantidade);
                    sublimp += (7.99 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", l6);
                    break;

                case 17:
                    printf("Sabao em barra selecionado. Quantidade ?");
                    scanf("%f", &quantidade);
                    if (quantidade <= l7) {
                    l7 = quantidade - l7;
                    vendas += quantidade;
                    subtotal += (1.00 * quantidade);
                    sublimp += (1.00 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", l7);
                    break;

                case 18:
                    printf("Voltar ao menu principal\n");
                    break;

                default:
                    printf("Opcao invalida! Selecione outra opcao.\n");
                    break;
            }
        }
        
            
            break;

        case 2: while (comi != 28)
        {
            printf("---- Setor de Alimentos ----\n ");
            printf("21 - Café (500g) R$19.99 (%d)\n ", a1);
            printf("22 - Leite (cx) R$5.90 (%d)\n ", a2);
            printf("23 - Arroz (1kg) R$4.50 (%d)\n ", a3);
            printf("24 - Feijão Preto (1kg) R$8.00 (%d)\n", a4); 
            printf("25 - Açucar (1kg) R$5.00 (%d)\n", a5); 
            printf("26 - Sal (1kg) R$2.00 (%d)\n ", a6);
            printf("27 - Farinha de Trigo (1kg) R$5.00 (%d)\n", a7);
            printf("28 - MENU PRINCIPAL\n");
            scanf("%d", &comi);

            switch (comi)
            {
            case 21:
                    printf("Café selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= a1) {
                    a1 = quantidade - a1;
                    vendas += quantidade;
                    subtotal += (19.99 * quantidade);
                    subcomi += (19.99 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a1);
                    break;

            case 22:
                    printf("Leite selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= a2) {
                    a2 = quantidade - a2;
                    vendas += quantidade;
                    subtotal += (5.90 * quantidade);
                    subcomi += (5.90 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a2);
                    break;

            case 23:
                    printf("Arroz selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= a3) {
                    a3 = quantidade - a3;
                    vendas += quantidade;
                    subtotal += (4.50 * quantidade);
                    subcomi += (4.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a3);
                    break;

            case 24:
                    printf("Feijao preto selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= a4) {
                    a4 = quantidade - a4;
                    vendas += quantidade;
                    subtotal += (8.00 * quantidade);
                    subcomi += (8.00 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a4);
                    break;

            case 25:
                    printf("Acucar selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= a5) {
                    a5 = quantidade - a5;
                    vendas += quantidade;
                    subtotal += (5.00 * quantidade);
                    subcomi += (5.00 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a5);
                    break;

            case 26:
                    printf("Sal selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= a6) {
                    a6 = quantidade - a6;
                    vendas += quantidade;
                    subtotal += (2.00 * quantidade);
                    subcomi += (2.00 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a6);
                    break;

            case 27:
                    printf("Farinha de trigo selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= a7) {
                    a7 = quantidade - a7;
                    vendas += quantidade;
                    subtotal += (5.00 * quantidade);
                    subcomi += (5.00 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a7);
                    break;

            case 28:
                    printf("Voltando para o menu principal\n");
                    break;
            
            default:
                printf("Opcao invalida, insira outra opção!\n");
                break;
            }
        }
        
            
            break;

        case 3:
            if (start == 0){
                printf("\nErro! inicie o caixa para atualizar o estoque!\n\n");
                break; 
            }
            else
            while (pao != 38)
        {   
                   
            printf("---- Setor da Padaria ----\n");
            printf("31 - Pão de Forma(pct) R$ 9,50 (%d)\n", p1);
            printf("32 - Pão de Integral(pct) R$ 12,50 (%d)\n", p2);
            printf("33 - Pão Francês (Un) R$ 1,90 (%d)\n", p3);
            printf("34 - Sonho (Un) R$ 8,50 (%d)\n", p4);
            printf("35 - Biscoito(Un) R$ 12,50 (%d)\n", p5);
            printf("36 - Pão Doce (Un) R$ 2,50 (%d)\n", p6); 
            printf("37 - Salgado (Un) R$ 17,50 (%d)\n", p7); 
            printf("38 - MENU PRINCIPAL\n\n ");
            scanf("%d", &pao);
            
            switch (pao)
            {
            case 31:
                    printf("Pão de Forma(pct) selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= p1) {
                    p1 = quantidade - p1;
                    vendas += quantidade;
                    subtotal += (9.50 * quantidade);
                    subpao += (9.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p1);
                    break;

            case 32:
                    printf("Pão de Integral(pct) selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= p2) {
                    p2 = quantidade - p2;
                    vendas += quantidade;
                    subtotal += (12.50 * quantidade);
                    subpao += (12.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p2);
                    break;

            case 33:
                    printf("Pão Francês (Un) selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= p3) {
                    p3 = quantidade - p3;
                    vendas += quantidade;
                    subtotal += (1.90 * quantidade);
                    subpao += (1.90 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p3);
                    break;

            case 34:
                    printf("Sonho (Un) selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= p4) {
                    p4 = quantidade - p4;
                    vendas += quantidade;
                    subtotal += (8.50 * quantidade);
                    subpao += (8.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p4);
                    break;

            case 35:
                    printf("Biscoito(Un) selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= p5) {
                    p5 = quantidade - p5;
                    vendas += quantidade;
                    subtotal += (12.50 * quantidade);
                    subpao += (12.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p5);
                    break;

            case 36:
                    printf("Pão Doce (Un) selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= p6) {
                    p6 = quantidade - p6;
                    vendas += quantidade;
                    subtotal += (2.50 * quantidade);
                    subpao += (2.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p6);
                    break;

            case 37:
                    printf("Salgado (Un) selecionado. Quantidade?\n");
                    scanf("%f", &quantidade);
                    if (quantidade <= p7) {
                    p7 = quantidade - p7;
                    vendas += quantidade;
                    subtotal += (17.50 * quantidade);
                    subpao += (17.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p7);
                    break;

            case 38:
                    printf("MENU PRINCIPAL\n");
                    break;

            default:
                printf("Opcao invalida, insira outra opcao.\n");
                break;
            }
        }  
        break;

        case 4:
                if (opc == 4 && subtotal <= 0) 
            printf("Err0! Selecione um item para efetuar o pagamento.\n");
            
            else if (opc == 4 && subtotal > 0) {
            printf("\nMaterial de Limpeza – R$%2.f\nVenda de Alimentos - R$%2.f\nPadaria – Total R$%2.f\nTotal geral: R$%2.f\n", sublimp, subcomi, subpao, subtotal);
            printf("\nEscolha a forma de pagamento:\n");
            printf("1. Dinheiro com desconto\n");
            printf("2. Cartão\n");
            printf("Escolha: ");
            scanf("%d", &pag);
            }       
                switch (pag)
                {
                case 1:
                    printf("\nQuanto de dinheiro você quer depositar? \n");
                    scanf("%f", &dindin);
                   
                while (subtotal > dindin) {
                    printf("\nDinheiro insuficiente! Insira um valor suficiente: \n");
                    scanf("%f", &dindin);
                    break;
                    }
                        
                    if (dindin <= 50) {
                    printf("\nVoce recebeu 5 porcento de desconto! O valor a ser pago é de R$%2.f\n", (subtotal - (subtotal * 0.05)));
                    subtotal -= (subtotal * 0.05);  
                    }  
                    
                    else if  (dindin > 50 && pag < 100) {
                        printf("\nVoce recebeu 10 porcento de desconto! O valor a ser pago é de R$%2.f\n", (subtotal - (subtotal * 0.10)));
                        subtotal -= (subtotal * 0.10);
                    }
                    
                    else {
                        printf("\nVoce recebeu 18 porcento de desconto! O valor a ser pago é de R$%2.f\n", (subtotal - (subtotal * 0.18)));
                        subtotal -= (subtotal * 0.18); 
                    }
                    if (dindin > subtotal) 
                        printf("Troco: R$%2.f\n", dindin - subtotal);
                    
                    break;

                case 2:

                    printf("\nEfetuando pagamento...\n");
                    printf("\n(1) – Pagamento OK\n");
                    printf("\n(0) – Pagamento recusado\n");
                    printf("Digite o resultado do processo de pagamento: \n");
                    scanf("%d", &cartao);

                while (cartao == 0) {
                    printf("\nErro, tente novamente\n");
                    printf("\n(1) – Pagamento OK\n");
                    printf("(0) – Pagamento recusado\n");
                    printf("Digite o resultado do processo de pagamento: \n");
                    scanf("%d", &cartao);
                
                    if (cartao == 1) 
                    printf("Pagamento efetuado\n"); 
                    }
                    
                    default:
                    printf("Opção inválida, escolha outra opção!\n");
                    break;

                }break;

                

        case 5:
                if (start != 0) {
                    printf("Erro! o caixa ja esta em funcionamento\n\n");
                    }

                    else if (start >= 0 && start <=10) { 
                    printf("Digite o valor inicial do caixa:\n");
                    scanf("%f", &start);
                    vendas = start;
                    printf("Caixa aberto. Valor: %2.f\n\n", start);

                    
                    printf("----- Atualize o estoque da padaria -----\n");
                    printf("Digite a quantidade disponivel em cada produto:\n\n");
                    printf("Pão de forma:\n");
                    scanf("%d", &p1);

                    printf("Pao integral:\n");
                    scanf("%d", &p2);

                    printf("Pao frances:\n");
                    scanf("%d", &p3);

                    printf("Sonho:\n");
                    scanf("%d", &p4);

                    printf("Biscoito:\n");
                    scanf("%d", &p5);

                    printf("Pao doce:\n");
                    scanf("%d", &p6);

                    printf("Salgado:\n");
                    scanf("%d", &p7);

                    printf("\nEstoque atualizado. Voltando ao menu principal...\n");
                    }
                    

                break;

        case 6:
            printf("Fechamento de caixa:\n");
            printf("Material de Limpeza – R$%2.f\n", sublimp);
            printf("Venda de Alimentos - R$%2.f\n", subcomi);
            printf("Padaria – Total R$%2.f\n", subpao);
            vendas = start - vendas;
            printf("Total geral: R$%2.f\n", vendas);
            printf("Saindo...\n");
            break;

        default:
            printf("Opção inválida, escolha outra opção!\n");
            break;
            }
        

        }while (opc != 6);
    
    
    return 0;
}