#include <stdio.h>

struct Aluno {
    char nome[50];
    float nota1, nota2;
};

float calcularMedia(struct Aluno *a) {
    return (a->nota1 + a->nota2) / 2;
}

void situacao(struct Aluno *a) {
    float media = calcularMedia(a);

    printf("%s - Media: %.2f - %s\n", a->nome, media, (media >= 6) ? "Aprovado" : "Reprovado");

}

int main() {
    struct Aluno alunos[3];

    for (int i = 0; i < 3; i++) {
        printf("\nAluno %d\n", i + 1);
        scanf(" %[^\n]", alunos[i].nome);
        scanf(" %f", alunos[i].nota1);
        scanf(" %f", alunos[i].nota2);

    }

    printf("\nResultado: \n");
    for (int i = 0; i < 3; i++) {
        situacao(&alunos[i]);
    }


    return 0;
} 