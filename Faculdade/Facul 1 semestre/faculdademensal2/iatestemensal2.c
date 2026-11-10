#include <stdio.h>

int main() {
    int opc, produto, cartao;
    int limp, comi, pao, pag, str;
    int vendas = 0, quantidade = 0;
    float subtotal = 0.0, sublimp = 0.0, subcomi = 0.0, subpao = 0.0;

    do {
        printf("\n ---Menu Principal---\n");
        printf(" 1. Material de Limpeza\n");
        printf(" 2. Venda de Alimentos\n");
        printf(" 3. Padaria\n");
        printf(" 4. Pagamento\n");
        printf(" 5. Abertura de Caixa\n");
        printf(" 6. Fechamento de Caixa\n");
        printf(" 7. Sair\n Escolha: ");
        scanf("%d", &opc);

        switch (opc) {

            case 1:
                printf("---Setor de Limpeza---\n");
                printf("11 Detergente R$1.99 (50)\n");
                printf("12 Sabao em Po (1kg) R$8.99 (30)\n");
                printf("13 Esponja R$1.50 (S/Estoque)\n");
                printf("14 Amaciante(1Lt) R$15.00 (20)\n");
                printf("15 Bucha de Pia (kit c/3) R$4.99 (100)\n");
                printf("16 Desinfetante(1Lt) R$7.99 (15)\n");
                printf("17 Sabao em Barra(Un) R$1.00 (60)\n");
                printf("18. Voltar ao Menu Principal\n");
                printf("Escolha: ");
                scanf("%d", &limp);

                switch (limp) {
                    case 11:
                        printf("Detergente selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (1.99 * quantidade);
                        sublimp  += (1.99 * quantidade);
                        break;

                    case 12:
                        printf("Sabao em Po selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (8.99 * quantidade);
                        sublimp  += (8.99 * quantidade);
                        break;

                    case 13:
                        printf("Esponja selecionada. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (1.50 * quantidade);
                        sublimp  += (1.50 * quantidade);
                        break;

                    case 14:
                        printf("Amaciante selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (15.00 * quantidade);
                        sublimp  += (15.00 * quantidade);
                        break;

                    case 15:
                        printf("Bucha de pia selecionada. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (4.99 * quantidade);
                        sublimp  += (4.99 * quantidade);
                        break;

                    case 16:
                        printf("Desinfetante selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (7.99 * quantidade);
                        sublimp  += (7.99 * quantidade);
                        break;

                    case 17:
                        printf("Sabao em barra selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (1.00 * quantidade);
                        sublimp  += (1.00 * quantidade);
                        break;

                    case 18:
                        printf("Voltando ao Menu Principal.\n");
                        break;

                    default:
                        printf("Opcao invalida! Selecione outra opcao.\n");
                        break;
                }
                break; /* FIX: impede fall-through para o case 2 */

            case 2:
                printf("---Setor de Alimentos---\n");
                printf("21 Cafe (500g) R$19.99 (10)\n");
                printf("22 Leite (cx) R$5.90 (15)\n");
                printf("23 Arroz (1kg) R$4.50 (10)\n");
                printf("24 Feijao Preto (1kg) R$8.00 (5)\n");
                printf("25 Acucar (1kg) R$5.00 (20)\n");
                printf("26 Sal (1kg) R$2.00 (6)\n");
                printf("27 Farinha de Trigo (1kg) R$5.00 (15)\n");
                printf("28. Voltar ao Menu Principal\n");
                printf("Escolha: ");
                scanf("%d", &comi);

                switch (comi) {
                    case 21:
                        printf("Cafe selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (19.99 * quantidade);
                        subcomi  += (19.99 * quantidade);
                        break;

                    case 22:
                        printf("Leite selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (5.90 * quantidade);
                        subcomi  += (5.90 * quantidade);
                        break;

                    case 23:
                        printf("Arroz selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (4.50 * quantidade);
                        subcomi  += (4.50 * quantidade);
                        break;

                    case 24:
                        printf("Feijao preto selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (8.00 * quantidade);
                        subcomi  += (8.00 * quantidade);
                        break;

                    case 25:
                        printf("Acucar selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (5.00 * quantidade);
                        subcomi  += (5.00 * quantidade);
                        break;

                    case 26:
                        printf("Sal selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (2.00 * quantidade);
                        subcomi  += (2.00 * quantidade);
                        break;

                    case 27:
                        printf("Farinha de trigo selecionada. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (5.00 * quantidade);
                        subcomi  += (5.00 * quantidade);
                        break;

                    case 28:
                        printf("Voltando ao Menu Principal.\n");
                        break;

                    default:
                        printf("Opcao invalida, insira outra opcao.\n");
                        break;
                }
                break; /* FIX: impede fall-through para o case 3 */

            case 3:
                printf("---Setor da Padaria---\n");
                printf("31 Pao de Forma(pct) R$9.50\n");
                printf("32 Pao Integral(pct) R$12.50\n");
                printf("33 Pao Frances (Un) R$1.90\n");
                printf("34 Sonho (Un) R$8.50\n");
                printf("35 Biscoito(kg) R$12.50\n");  /* FIX: era Sonho */
                printf("36 Pao Doce (Un) R$2.50\n");
                printf("37 Salgado (Un) R$17.50\n");
                printf("38. Voltar ao Menu Principal\n");
                printf("Escolha: ");
                scanf("%d", &pao);

                switch (pao) {
                    case 31:
                        printf("Pao de forma selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (9.50 * quantidade);
                        subpao   += (9.50 * quantidade);
                        break;

                    case 32:
                        printf("Pao integral selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (12.50 * quantidade);
                        subpao   += (12.50 * quantidade);
                        break;

                    case 33:
                        printf("Pao frances selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (1.90 * quantidade);
                        subpao   += (1.90 * quantidade);
                        break;

                    case 34:
                        printf("Sonho selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (8.50 * quantidade);
                        subpao   += (8.50 * quantidade);
                        break;

                    case 35: /* FIX: era copia do Sonho; corrigido para Biscoito */
                        printf("Biscoito selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (12.50 * quantidade);
                        subpao   += (12.50 * quantidade);
                        break;

                    case 36: /* FIX: preco era 12.50; correto e 2.50 */
                        printf("Pao doce selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (2.50 * quantidade);
                        subpao   += (2.50 * quantidade);
                        break;

                    case 37:
                        printf("Salgado selecionado. Quantidade? ");
                        scanf("%d", &quantidade);
                        vendas += quantidade;
                        subtotal += (17.50 * quantidade);
                        subpao   += (17.50 * quantidade);
                        break;

                    case 38:
                        printf("Voltando ao Menu Principal.\n");
                        break;

                    default:
                        printf("Opcao invalida, insira outra opcao.\n");
                        break;
                }
                break; /* FIX: impede fall-through para o case 4 */

            case 4: /* FIX: movido para fora do switch da padaria */
                if (subtotal <= 0) {
                    printf("Erro! Selecione um item para efetuar o pagamento.\n");
                    break;
                }

                /* FIX: formato corrigido de %2.f para %.2f; sublimp incluida */
                printf("\nMaterial de Limpeza  – R$%.2f\n", sublimp);
                printf("Venda de Alimentos   – R$%.2f\n", subcomi);
                printf("Padaria              – R$%.2f\n", subpao);
                printf("Total geral          – R$%.2f\n", subtotal);

                printf("\nEscolha a forma de pagamento:\n");
                printf("1. Dinheiro (com desconto)\n");
                printf("2. Cartao\n");
                printf("Escolha: ");
                scanf("%d", &pag);

                /* FIX: switch(pag) agora esta apos o scanf, nao apos o break */
                switch (pag) {
                    case 1: {
                        float dinheiro = 0.0;
                        printf("\nQuanto de dinheiro voce quer depositar? ");
                        scanf("%f", &dinheiro);

                        while (dinheiro < subtotal) { /* FIX: loop correto sem break prematuro */
                            printf("Dinheiro insuficiente! Insira um valor suficiente: ");
                            scanf("%f", &dinheiro);
                        }

                        /* FIX: desconto baseado no total, nao no valor pago */
                        float desconto = 0.0;
                        if (subtotal <= 50.0) {
                            desconto = 0.05;
                            printf("Voce recebeu 5%% de desconto!\n");
                        } else if (subtotal <= 100.0) {
                            desconto = 0.10;
                            printf("Voce recebeu 10%% de desconto!\n");
                        } else {
                            desconto = 0.18;
                            printf("Voce recebeu 18%% de desconto!\n");
                        }

                        float total_com_desconto = subtotal - (subtotal * desconto);
                        printf("Valor a pagar: R$%.2f\n", total_com_desconto);

                        if (dinheiro > total_com_desconto) {
                            printf("Troco: R$%.2f\n", dinheiro - total_com_desconto);
                        }

                        /* Zera o carrinho apos pagamento */
                        subtotal = sublimp = subcomi = subpao = 0.0;
                        break;
                    }

                    case 2:
                        printf("\nEfetuando pagamento...\n");
                        printf("(1) Pagamento na maquina Ok\n");
                        printf("(0) Pagamento no cartao nao Ok\n");
                        printf("Digite o resultado: ");
                        scanf("%d", &cartao);

                        while (cartao == 0) {
                            printf("\nErro, tente novamente.\n");
                            printf("(1) Pagamento na maquina Ok\n");
                            printf("(0) Pagamento no cartao nao Ok\n");
                            printf("Digite o resultado: ");
                            scanf("%d", &cartao);
                        }

                        if (cartao == 1) {
                            printf("\nPagamento efetuado com sucesso!\n");
                            subtotal = sublimp = subcomi = subpao = 0.0;
                        }
                        break;

                    default:
                        printf("Opcao invalida.\n");
                        break;
                }
                break;

            case 5:
                printf("Abertura de caixa\n");
                scanf("%d", &str);
                break;

            case 6:
                printf("Fechamento de caixa\n");
                printf("Total de itens vendidos: %d\n", vendas);
                printf("Total arrecadado: R$%.2f\n", subtotal);
                break;

            case 7:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                break;
        }

    } while (opc != 7);

    return 0;
}