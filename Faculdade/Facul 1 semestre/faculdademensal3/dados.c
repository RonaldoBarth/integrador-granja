#include <stdio.h>

struct Pessoa {
    char nome[50];
    int idade;
    float altura;

};

int main() {
    struct Pessoa p;
    struct Pessoa *pPtr = &p;

    printf("Nome: ");
    scanf(" %[^\n]", pPtr->nome);

    printf("Idade: ");
    scanf(" %d", &pPtr->idade);

    printf("Altura: ");
    scanf(" %f", pPtr->altura);

    printf("Dados: ");
    printf(" Nome: %s\nIdade: %d\nAltura: %2.f\n", pPtr->nome, pPtr->idade, pPtr->altura);

    return 0;
    
}
