#include <stdio.h>

int main () {
    int opc, produto, cartao;
    int limp, comi, pao, pag, str, vendas, quantidade;
    float subtotal = 0.0, sublimp = 0.0, subcomi = 0.0, subpao = 0.0;

    do {

        printf(" ---Menu Principal---\n ");
        printf("1. Material de Limpeza\n ");
        printf("2. Venda de Alimentos\n ");
        printf("3. Padaria\n");
        printf("4. Pagamento\n ");
        printf("5. Abertura de Caixa\n ");
        printf("6. Fechamento de Caixa\n"); 
        printf("7. Sair\n Escolha: ");
        scanf("%d", &opc);

    switch(opc) {
    
        case 1:
            printf("---Setor de Limpeza---\n");
            printf("11 Detergente R$1.99 (50)\n");
            printf("12 Sabão em Pó (1kg) R$8.99 (30)\n");
            printf("13 Esponja R$1.50 (S/Estoque)\n ");
            printf("14 Amaciante(1Lt) R$15.00 (20)\n ");
            printf("15 Bucha de Pia (kit c/3) R$4.99 (100)\n ");
            printf("16 Desinfetante(1Lt) R$7.99 (15)\n ");
            printf("17 Sabão em Barra(Un) R$1.00 (60)\n ");
            printf("18. Voltar ao Manu Principal\n ");
            printf("Escolha: \n");
                scanf("%d", &limp);

            switch (limp)
            {
                case 11:
                    printf("Detergente selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (1.99 * quantidade);
                    sublimp += (1.99 * quantidade);
                    break;
                
                case 12:
                    printf("Sabão em Pó selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (8.99 * quantidade);
                    sublimp += (8.99 * quantidade);
                    break;

                case 13:
                    printf("Esponja selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (1.50 * quantidade);
                    sublimp += (1.50 * quantidade);
                    break;

                case 14:
                    printf("Amaciante selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (15.00 * quantidade);
                    sublimp += (15.00 * quantidade);
                    break;

                case 15:
                    printf("Bucha de pia selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (4.99 * quantidade);
                    sublimp += (4.99 * quantidade);
                    break;

                case 16:
                    printf("Desinfetante selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (7.99 * quantidade);
                    sublimp += (7.99 * quantidade);
                    break;

                case 17:
                    printf("Sabao em barra selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (1.00 * quantidade);
                    sublimp += (1.00 * quantidade);
                    break;

                case 18:
                    break;

                default:
                    printf("Opcao invalida! Selecione outra opcao.");
                    break;
            }
            break;

        case 2:
            printf("---Setor de Alimentos---\n ");
            printf("21 Café (500g) R$19.99 (10)\n ");
            printf("22 Leite (cx) R$5.90 (15)\n ");
            printf("23 Arroz (1kg) R$4.50 (10)\n ");
            printf("24 Feijão Preto (1kg) R$8.00 (5)\n"); 
            printf("25 Açucar (1kg) R$5.00 (20)\n"); 
            printf("26 Sal (1kg) R$2.00 (6)\n ");
            printf("27 Farinha de Trigo (1kg) R$5.00 (15)\n");
            printf("28. Voltar ao Manu Principal");
            scanf("%d", &comi);

            switch (comi)
            {
            case 21:
                    printf("Cafe selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (19.99 * quantidade);
                    subcomi += (19.99 * quantidade);
                    break;

            case 22:
                    printf("Leite selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (5.90 * quantidade);
                    subcomi += (5.90 * quantidade);
                    break;

            case 23:
                    printf("Arroz selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (4.50 * quantidade);
                    subcomi += (4.50 * quantidade);
                    break;

            case 24:
                    printf("Feijao preto selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (8.00 * quantidade);
                    subcomi += (8.00 * quantidade);
                    break;

            case 25:
                    printf("Acucar selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (5.00 * quantidade);
                    subcomi += (5.00 * quantidade);
                    break;

            case 26:
                    printf("Sal selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (2.00 * quantidade);
                    subcomi += (2.00 * quantidade);
                    break;

            case 27:
                    printf("Farinha de trigo selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (5.00 * quantidade);
                    subcomi += (5.00 * quantidade);
                    break;

            case 28:
                    printf("Voltando para o menu principal.");
            
            default:
                printf("Opcao invalida, insira outra opcao.");
                break;
            }
            break;

        case 3:
            printf("---Setor da Padaria---\n ");
            printf("31 Pão de Forma(pct) R$ 9,50 (0)\n ");
            printf("32 Pão de Integral(pct) R$ 12,50 (S/Estoque) (0)\n");
            printf("33 Pão Francês (Un) R$ 1,90 (0)\n ");
            printf("34 Sonho (Un) R$ 8,50 (0)\n ");
            printf("35 Biscoito(kg) R$ 12,50 (0)\n ");
            printf("36 Pão Doce (Un) R$ 2,50 (0)\n"); 
            printf("37 Salgado (Un) R$ 17,50 (0)\n"); 
            printf("8 Voltar ao Manu Principal");
            scanf("%d", &pao);

            switch (pao)
            {
            case 31:
                    printf("Pao de forma selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (9.50 * quantidade);
                    subpao += (9.50 * quantidade);
                    break;

            case 32:
                    printf("Pao de integral selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (12.50 * quantidade);
                    subpao += (12.50 * quantidade);
                    break;

            case 33:
                    printf("Pao frances selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (1.90 * quantidade);
                    subpao += (1.90 * quantidade);
                    break;

            case 34:
                    printf("Sonho selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (8.50 * quantidade);
                    subpao += (8.50 * quantidade);
                    break;

            case 35:
                    printf("Sonho selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (8.50 * quantidade);
                    subpao += (8.50 * quantidade);
                    break;

            case 36:
                    printf("Pao doce selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (12.50 * quantidade);
                    subpao += (12.50 * quantidade);
                    break;

            case 37:
                    printf("Salgado selecionado. Quantidade?");
                    scanf("%f", &quantidade);
                    vendas += quantidade;
                    subtotal += (17.50 * quantidade);
                    subpao += (17.50 * quantidade);
                    break;

            case 38:
                    printf("Voltando para o menu principal.");
                    break;

            default:
                printf("Opcao invalida, insira outra opcao.");
                break;
            }
            break;

        case 4:
            printf("---Pagamento---\n Fomas de pagamento: \n 1- Dinheiro.\n2- Cartao de Credito/Debito.\n");
           
                if (opc == 4 && subtotal <= 0) {
            printf("Err0! Selecione um item para efetuar o pagamento.");
            }
            else if (opc == 4 && subtotal > 0) {

            printf("\nMaterial de Limpeza – R$%2.f\nVenda de Alimentos - R$%2.f\nPadaria – Total R$%2.f\nTotal geral: R$%2.f\n", subcomi, subpao, subtotal);
            printf("\nEscolha a forma de pagamento:\n");
            printf("1. Dinheiro com desconto\n");
            printf("2. Cartão\n");
            printf("Escolha: ");
            scanf("%d", &pag);
            }    
            break; 
                
                switch (pag)
                {
                case 1:
                    printf("\nQuanto de dinheiro você quer depositar? ");
                    scanf("%2.f", &pag);
                    
                while (subtotal > pag) {
                    printf("\nDinheiro insuficiente! Insira um valor suficiente: ");
                    scanf("%2.f", &pag);
                    break;
                        
                    if (pag <= 50) {
                    printf("\nVoce recebeu 5 porcento de desconto! O valor a ser pago é de R$%2.f\n", (subtotal - (subtotal * 0.05)));
                    subtotal -= (subtotal * 0.05);    
                    }
                    else if (pag > 50 && pag < 100) {
                        printf("\nVoce recebeu 10 porcento de desconto! O valor a ser pago é de R$%2.f\n", (subtotal - (subtotal * 0.10)));
                        subtotal -= (subtotal * 0.10);
                    }
                    else {
                        printf("\nVoce recebeu 18 porcento de desconto! O valor a ser pago é de R$%2.f\n", (subtotal - (subtotal * 0.18)));
                        subtotal -= (subtotal * 0.18);
                    }
                    if (pag > subtotal) {
                        printf("Troco: R$%2.f\n", pag - subtotal);
                    }
                    break;}

                case 2:

                    printf("\nEfetuando pagamento...\n");
                    printf("\n(1) – Pagamento na máquina Ok\n");
                    printf("(0) – Pagamento no cartão não Ok\n");
                    printf("Digite o resultado do processo de pagamento: ");
                    scanf("%d", &cartao);

                while (cartao == 0) {
                    printf("\nErro, tente novamente\n");
                    printf("\n(1) – Pagamento na máquina Ok\n");
                    printf("(0) – Pagamento no cartão não Ok\n");
                    printf("Digite o resultado do processo de pagamento: ");
                    scanf("%d", &cartao);
                }
                    if (cartao == 1) {
                    printf("\nPagamento efetuado\n");
                }
                    break;

        case 5:
            printf("Abertura de caixa\n");
            break;

        case 6:
            printf("Fechamento de caixa\n", vendas);
            break;

        case 7:
            printf("Saindo...");
            break;
            }
        default:
            printf("Saindo\n");
            break;
    }
    
    }while (opc != 7);
    
    return 0;
}