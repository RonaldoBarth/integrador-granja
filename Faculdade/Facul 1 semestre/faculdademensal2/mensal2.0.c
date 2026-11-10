#include <stdio.h>

// Funçâo erro ÇÇÇ//
int lerInt(const char* letra) {
    int valor;
    printf("%s", letra);
    while (scanf("%d", &valor) != 1) {
        printf("Entrada invalida! Digite um numero: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return valor;
}

float lerFloat(const char* letra) {
    float valor;
    printf("%s", letra);
    while (scanf("%f", &valor) != 1) {
        printf("Entrada invalida! Digite um numero: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return valor;
}
//Fim função erro ÇÇÇ//
int main () {
    int opc, produto, cartao;
    int estoquelimp[7] = {50, 30, 0, 20, 100, 15, 60};
    int p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0, p6 = 0, p7 = 0; // Estoque padaria //
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
        opc = lerInt("Escolha:  ");

    switch(opc) {
    
        case 1: while (limp != 18)
        {
           printf("\n---- Setor de Limpeza ---\n ");
            printf("11 - Detergente R$1.99 (%d)\n ", (estoquelimp[0]));
            printf("12 - Sabão em Pó (1kg) R$8.99 (%d)\n ", (estoquelimp[1]));
            printf("13 - Esponja R$1.50 (%d)\n ", (estoquelimp[2]));
            printf("14 - Amaciante(1Lt) R$15.00 (%d)\n ", (estoquelimp[3]));
            printf("15 - Bucha de Pia (kit c/3) R$4.99 (%d)\n ", (estoquelimp[4]));
            printf("16 - Desinfetante(1Lt) R$7.99 (%d)\n ", (estoquelimp[5]));
            printf("17 - Sabão em Barra(Un) R$1.00 (%d)\n ", (estoquelimp[6]));
            printf("18 - MENU PRINCIPAL\n ");
            limp = lerInt("Escolha: \n ");

            switch (limp)
            {
                case 11:
                    quantidade = lerFloat("Detergente selecionado. Quantidade? ");
                    if (quantidade <= estoquelimp[0]) {
                    estoquelimp[0] -= quantidade;
                    vendas += quantidade;
                    subtotal += (1.99 * quantidade);
                    sublimp += (1.99 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", estoquelimp[0]);
                    break;
                
                case 12:
                    quantidade = lerFloat("Sabão em Pó selecionado. Quantidade? ");
                    if (quantidade <= estoquelimp[1]) {
                    estoquelimp[1] -= quantidade;
                    vendas += quantidade;
                    subtotal += (8.99 * quantidade);
                    sublimp += (8.99 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", estoquelimp[1]);
                    break;

                case 13:
                    quantidade = lerFloat("Esponja selecionado. Quantidade? ");
                    if (quantidade <= estoquelimp[2]) {
                    estoquelimp[2] -= quantidade;
                    vendas += quantidade;
                    subtotal += (1.50 * quantidade);
                    sublimp += (1.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", estoquelimp[2]);
                    break;

                case 14:
                    quantidade = lerFloat("Amaciante selecionado. Quantidade? ");
                    if (quantidade <= estoquelimp[3]) {
                    estoquelimp[3] -= quantidade;
                    vendas += quantidade;
                    subtotal += (15.00 * quantidade);
                    sublimp += (15.00 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", estoquelimp[3]);
                    break;

                case 15:
                    quantidade = lerFloat("Bucha de pia selecionado. Quantidade? ");
                    if (quantidade <= estoquelimp[4]) {
                    estoquelimp[4] -= quantidade;
                    vendas += quantidade;
                    subtotal += (4.99 * quantidade);
                    sublimp += (4.99 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", estoquelimp[4]);
                    break;

                case 16:
                    quantidade = lerFloat("Desinfetante selecionado. Quantidade? ");
                    if (quantidade <= estoquelimp[5]) {
                    estoquelimp[5] -= quantidade;
                    vendas += quantidade;
                    subtotal += (7.99 * quantidade);
                    sublimp += (7.99 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", estoquelimp[5]);
                    break;

                case 17:
                    quantidade = lerFloat("Sabao em barra selecionado. Quantidade? ");
                    if (quantidade <= estoquelimp[6]) {
                    estoquelimp[6] -= quantidade;
                    vendas += quantidade;
                    subtotal += (1.00 * quantidade);
                    sublimp += (1.00 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", estoquelimp[6]);
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
            printf("25 - Açucar (1kg) R$5.00 (%d)\n ", a5); 
            printf("26 - Sal (1kg) R$2.00 (%d)\n ", a6);
            printf("27 - Farinha de Trigo (1kg) R$5.00 (%d)\n ", a7);
            printf("28 - MENU PRINCIPAL\n ");
            comi = lerInt("Escolha: ");

            switch (comi)
            {
            case 21:
                    quantidade = lerFloat("Café selecionado. Quantidade? ");
                    if (quantidade <= a1) {
                    a1 -= quantidade;
                    vendas += quantidade;
                    subtotal += (19.99 * quantidade);
                    subcomi += (19.99 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a1);
                    break;

            case 22:
                    quantidade = lerFloat("Leite selecionado. Quantidade? ");
                    if (quantidade <= a2) {
                    a2 -= quantidade;
                    vendas += quantidade;
                    subtotal += (5.90 * quantidade);
                    subcomi += (5.90 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a2);
                    break;

            case 23:
                    quantidade = lerFloat("Arroz selecionado. Quantidade? ");
                    if (quantidade <= a3) {
                    a3 -= quantidade;
                    vendas += quantidade;
                    subtotal += (4.50 * quantidade);
                    subcomi += (4.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a3);
                    break;

            case 24:
                    quantidade = lerFloat("Feijao preto selecionado. Quantidade? ");
                    if (quantidade <= a4) {
                    a4 -= quantidade;
                    vendas += quantidade;
                    subtotal += (8.00 * quantidade);
                    subcomi += (8.00 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a4);
                    break;

            case 25:
                    quantidade = lerFloat("Acucar selecionado. Quantidade? ");
                    if (quantidade <= a5) {
                    a5 -= quantidade;
                    vendas += quantidade;
                    subtotal += (5.00 * quantidade);
                    subcomi += (5.00 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a5);
                    break;

            case 26:
                    quantidade = lerFloat("Sal selecionado. Quantidade? ");
                    if (quantidade <= a6) {
                    a6 -= quantidade;
                    vendas += quantidade;
                    subtotal += (2.00 * quantidade);
                    subcomi += (2.00 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", a6);
                    break;

            case 27:
                    quantidade = lerFloat("Farinha de trigo selecionado. Quantidade? ");
                    if (quantidade <= a7) {
                    a7 -= quantidade;
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
                printf("Opcao invalida, insira outra opcao!\n");
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
                   
            printf("---- Setor da Padaria ----\n ");
            printf("31 - Pão de Forma(pct) R$ 9,50 (%d)\n ", p1);
            printf("32 - Pão de Integral(pct) R$ 12,50 (%d)\n ", p2);
            printf("33 - Pão Francês (Un) R$ 1,90 (%d)\n ", p3);
            printf("34 - Sonho (Un) R$ 8,50 (%d)\n ", p4);
            printf("35 - Biscoito(Un) R$ 12,50 (%d)\n ", p5);
            printf("36 - Pão Doce (Un) R$ 2,50 (%d)\n ", p6); 
            printf("37 - Salgado (Un) R$ 17,50 (%d)\n ", p7); 
            printf("38 - MENU PRINCIPAL\n\n ");
            pao = lerInt("Escolha: ");
            
            switch (pao)
            {
            case 31:
                    quantidade = lerFloat("Pão de Forma(pct) selecionado. Quantidade? ");
                    if (quantidade <= p1) {
                    p1 -= quantidade;
                    vendas += quantidade;
                    subtotal += (9.50 * quantidade);
                    subpao += (9.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p1);
                    break;

            case 32:
                    quantidade = lerFloat("Pão de Integral(pct) selecionado. Quantidade? ");
                    if (quantidade <= p2) {
                    p2 -= quantidade;
                    vendas += quantidade;
                    subtotal += (12.50 * quantidade);
                    subpao += (12.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p2);
                    break;

            case 33:
                    quantidade = lerFloat("Pão Francês (Un) selecionado. Quantidade? ");
                    if (quantidade <= p3) {
                    p3 -= quantidade;
                    vendas += quantidade;
                    subtotal += (1.90 * quantidade);
                    subpao += (1.90 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p3);
                    break;

            case 34:
                    quantidade = lerFloat("Sonho (Un) selecionado. Quantidade? ");
                    if (quantidade <= p4) {
                    p4 -= quantidade;
                    vendas += quantidade;
                    subtotal += (8.50 * quantidade);
                    subpao += (8.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p4);
                    break;

            case 35:
                    quantidade = lerFloat("Biscoito(Un) selecionado. Quantidade? ");
                    if (quantidade <= p5) {
                    p5 -= quantidade;
                    vendas += quantidade;
                    subtotal += (12.50 * quantidade);
                    subpao += (12.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p5);
                    break;

            case 36:
                    quantidade = lerFloat("Pão Doce (Un) selecionado. Quantidade? ");
                    if (quantidade <= p6) {
                    p6 -= quantidade;
                    vendas += quantidade;
                    subtotal += (2.50 * quantidade);
                    subpao += (2.50 * quantidade);
                    }
                    else 
                        printf("\nErro! quantidade disponivel: %d\n", p6);
                    break;

            case 37:
                    quantidade = lerFloat("Salgado (Un) selecionado. Quantidade? ");
                    if (quantidade <= p7) {
                    p7 -= quantidade;
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
            printf("Erro! Selecione um item para efetuar o pagamento.\n");
            
            else if (opc == 4 && subtotal > 0) {
            printf("\nMaterial de Limpeza – R$%.2f\nVenda de Alimentos - R$%.2f\nPadaria – Total R$%.2f\nTotal geral: R$%.2f\n", sublimp, subcomi, subpao, subtotal);
            printf("\nEscolha a forma de pagamento:\n");
            printf("1. Dinheiro com desconto\n");
            printf("2. Cartão\n");
            pag = lerInt("Escolha: ");
            }       
                switch (pag)
                {
                case 1:
                    dindin = lerFloat("\nQuanto de dinheiro voce quer depositar? ");
                   
                while (dindin < subtotal) {
                    printf("\nDinheiro insuficiente! Insira um valor suficiente: \n");
                    dindin = lerFloat("");
                    }
                        
                    if (dindin <= 50) {
                    printf("\nVoce recebeu 5 porcento de desconto! O valor a ser pago e de R$%.2f\n", (subtotal - (subtotal * 0.05)));
                    subtotal -= (subtotal * 0.05);  
                    }  
                    
                    else if (dindin > 50 && dindin < 100) {
                        printf("\nVoce recebeu 10 porcento de desconto! O valor a ser pago e de R$%.2f\n", (subtotal - (subtotal * 0.10)));
                        subtotal -= (subtotal * 0.10);
                    }
                    
                    else {
                        printf("\nVoce recebeu 18 porcento de desconto! O valor a ser pago e de R$%.2f\n", (subtotal - (subtotal * 0.18)));
                        subtotal -= (subtotal * 0.18); 
                    }
                    if (dindin > subtotal) 
                        printf("Troco: R$%.2f\n", dindin - subtotal);
                    
                    break;

                case 2:

                    printf("\nEfetuando pagamento...\n");
                    printf("\n(1) - Pagamento OK\n");
                    printf("\n(0) - Pagamento recusado\n");
                    cartao = lerInt("Digite o resultado do processo de pagamento: ");

                while (cartao == 0) {
                    printf("\nErro, tente novamente\n");
                    printf("\n(1) - Pagamento OK\n");
                    printf("(0) - Pagamento recusado\n");
                    cartao = lerInt("Digite o resultado do processo de pagamento: ");
                    }
                    printf("Pagamento efetuado\n");
                    break;
                    
                    default:
                    printf("Opcao invalida, escolha outra opcao!\n");
                    break;

                }break;

                

        case 5:
                if (start != 0) {
                    printf("Erro! o caixa ja esta em funcionamento\n\n");
                    }

                    else {
                    start = lerFloat("Digite o valor inicial do caixa: ");

                    if (start < 0) {
                        printf("Erro! Valor inicial invalido.\n");
                        start = 0;
                    } else {
                        vendas = start;
                        printf("Caixa aberto. Valor: %.2f\n\n", start);

                        printf("----- Atualize o estoque da padaria -----\n");
                        printf("Digite a quantidade disponivel em cada produto:\n\n");
                        p1 = lerInt("Pão de forma: ");
                        p2 = lerInt("Pao integral: ");
                        p3 = lerInt("Pao frances: ");
                        p4 = lerInt("Sonho: ");
                        p5 = lerInt("Biscoito: ");
                        p6 = lerInt("Pao doce: ");
                        p7 = lerInt("Salgado: ");

                        printf("\nEstoque atualizado. Voltando ao menu principal...\n");
                    }
                    }
                    

                break;

        case 6:
            printf("Fechamento de caixa:\n");
            printf("Material de Limpeza – R$%.2f\n", sublimp);
            printf("Venda de Alimentos - R$%.2f\n", subcomi);
            printf("Padaria – Total R$%.2f\n", subpao);
            printf("Total geral: R$%.2f\n", subtotal);
            printf("Saindo...\n");
            break;

        default:
            printf("Opcao invalida, escolha outra opcao!\n");
            break;
            }
        

        }while (opc != 6);
    
    
    return 0;
}