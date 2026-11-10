#include <stdio.h>



int main()

{

  int escolha, produto, quantidade, din, cartao, vendas=0;

  float subLimpeza = 0, subAlimentos = 0, subPadaria = 0, subTotal = 0;

  float payment;



  do {

    printf("\n===== MENU PRINCIPAL =====\n");

    printf("1 - Material de Limpeza\n");

    printf("2 - Venda de Alimentos\n");

    printf("3 - Padaria\n");

    printf("4 - Pagamento\n");

    printf("5 - Sair\n");

    printf("Escolha: ");

    scanf("%d", &escolha);



    if (escolha == 4 && subTotal <= 0) {

      printf("\nErro! Você não pode pagar por 0 itens, escolha novamente.\n");

    }



    else if (escolha == 4 && subTotal > 0) {



      printf("\nMaterial de Limpeza – R$%f\nVenda de Alimentos - R$%f\nPadaria – Total R$%f\nTotal geral: R$%f\n",

          subLimpeza, subAlimentos, subPadaria, subTotal);



      printf("\nEscolha a forma de pagamento:\n");

      printf("1. Dinheiro com desconto\n");

      printf("2. Cartão\n");

      printf("Escolha: ");

      scanf("%d", &din);



      switch(din) {



      case 1:

        printf("\nQuanto de dinheiro você quer depositar? ");

        scanf("%f", &payment);



        while (subTotal > payment) {

          printf("\nDinheiro insuficiente! Insira um valor suficiente: ");

          scanf("%f", &payment);

        }



        if (payment <= 50) {

          printf("\nVoce recebeu 5 porcento de desconto! O valor a ser pago é de R$%f\n", (subTotal - (subTotal * 0.05)));

          subTotal -= (subTotal * 0.05);

        }

        else if (payment > 50 && payment < 100) {

          printf("\nVoce recebeu 10 porcento de desconto! O valor a ser pago é de R$%f\n", (subTotal - (subTotal * 0.10)));

          subTotal -= (subTotal * 0.10);

        }

        else {

          printf("\nVoce recebeu 18 porcento de desconto! O valor a ser pago é de R$%f\n", (subTotal - (subTotal * 0.18)));

          subTotal -= (subTotal * 0.18);

        }



        if (payment > subTotal) {

          printf("Troco: R$%f\n", payment - subTotal);

        }

        break;



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

      }

    }



    switch(escolha) {



    case 1:

      printf("\n1 - Detergente R$1.99\n");

      printf("2 - Sabão em Pó (1kg) R$8.99\n");

      printf("3 - Esponja R$1.50\n");

      printf("4 - Amaciante(1Lt) R$15.00\n");

      printf("5 - Voltar ao Menu Principal\n");

      printf("Insira o codigo do produto: ");

      scanf("%d", &produto);



      switch(produto) {

      case 1:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (1.99 * quantidade);

        subLimpeza += (1.99 * quantidade);

        break;

      case 2:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (8.99 * quantidade);

        subLimpeza += (8.99 * quantidade);

        break;

      case 3:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (1.50 * quantidade);

        subLimpeza += (1.50 * quantidade);

        break;

      case 4:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (15 * quantidade);

        subLimpeza += (15 * quantidade);

        break;

      case 15:

        break;

      }

      break;



    case 2:

      printf("\n1 - Café R$19.99\n");

      printf("2 - Leite R$5.90\n");

      printf("3 - Arroz R$4.50\n");

      printf("4 - Feijão R$8.00\n");

      printf("5 - Açúcar R$5.00\n");

      printf("6 - Voltar ao Menu Principal\n");

      printf("Insira o codigo do produto: ");

      scanf("%d", &produto);



      switch(produto) {

      case 1:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (19.99 * quantidade);

        subAlimentos += (19.99 * quantidade);

        break;

      case 2:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (5.90 * quantidade);

        subAlimentos += (5.90 * quantidade);

        break;

      case 3:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (4.50 * quantidade);

        subAlimentos += (4.50 * quantidade);

        break;

      case 4:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (8.00 * quantidade);

        subAlimentos += (8.00 * quantidade);

        break;

      case 5:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (5.00 * quantidade);

        subAlimentos += (5.00 * quantidade);

        break;

      case 6:

        break;

      }

      break;



    case 3:

      printf("\n1 - Pão de Forma R$9.50\n");

      printf("2 - Pão Integral R$12.50\n");

      printf("3 - Pão Francês R$1.90\n");

      printf("4 - Sonho R$8.50\n");

      printf("5 - Voltar ao Menu Principal\n");

      printf("Insira o codigo do produto: ");

      scanf("%d", &produto);



      switch(produto) {

      case 1:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (9.50 * quantidade);

        subPadaria += (9.50 * quantidade);

        break;

      case 2:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (12.50 * quantidade);

        subPadaria += (12.50 * quantidade);

        break;

      case 3:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (1.90 * quantidade);

        subPadaria += (1.90 * quantidade);

        break;

      case 4:

        printf("Quantidade do produto: ");

        scanf("%d", &quantidade);

        vendas += quantidade;

        subTotal += (8.50 * quantidade);

        subPadaria += (8.50 * quantidade);

        break;

      case 5:

        break;

      }

      break;

    }



  } while(escolha != 5);



  printf("\n\nValor do faturamento foi de: R$%f", subTotal);

  printf("\nQuantidade de itens vendidos: %d\n", vendas);



  printf("\n\nPrograma encerrado!");

  return 0;

}