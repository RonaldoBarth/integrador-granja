/*  Desafio 1 — Arquivo Binário (Básico)
Crie um programa que:

Tenha uma struct Produto com int codigo, char nome[30], float preco
Preencha 3 produtos no código (sem scanf)
Salve os 3 em "produtos.dat" com fopen("wb") e fwrite
Reabra com fopen("rb") e fread, e imprima os 3 na tela
Pergunta: Qual a diferença entre abrir com "wb" e "rb"? */

#include <stdio.h>

struct Dados {
    int codigo;
    char nome[30];
    float preco;
};

int main() {
    struct Dados dado;
    FILE *f;

    f = fopen("produtos.dat", "wb");
    fwrite(&dado, sizeof(struct Dados), 1, f);
    fclose(f);

    f = fopen("produtos.dat", "rb");
    fread(&dado, sizeof(struct Dados), 1, f);
    fclose(f);

    return 0;
}