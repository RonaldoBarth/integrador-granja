#include <stdio.h>



int main() {

int n, i, opcao;

float soma = 0.0;



printf("Digite a quantidade de alunos (n): ");

scanf("%d", &n);



float notas[n];



  

for (i = 0; i < n; i++) {

  printf("Digite a nota do aluno %d: ", i + 1);

  scanf("%f", &notas[i]);

    soma += notas[i];

  }



  

  do {

    printf("\nMenu de Opcoes:\n");

    printf("1 - Exibir todas as notas\n");

    printf("2 - Exibir a media das notas\n");

    printf("3 - Contar alunos aprovados (nota >= 6)\n");

    printf("4 - Encerrar o programa\n");

    printf("Escolha uma opcao: ");

    scanf("%d", &opcao);



    switch (opcao) {

      case 1:

        printf("\nNotas dos alunos:\n");

        for (i = 0; i < n; i++) {

        printf("Aluno %d: %.2f\n", i + 1, notas[i]);

        }

        break;



      case 2:

       printf("\nMedia das notas: %.2f\n", soma / n);

      break;



      case 3: {

        int aprovados = 0;

        for (i = 0; i < n; i++) {

        if (notas[i] >= 6.0) {

        aprovados++;

          }

        }

        printf("\nQuantidade de alunos aprovados: %d\n", aprovados);

        break;

      }



      case 4:

        printf("\nEncerrando o programa\n");

        break;



      default:

        printf("\nOpcao invalida. Tente novamente.\n");

    }

  } while (opcao != 4);

  return 0;

}