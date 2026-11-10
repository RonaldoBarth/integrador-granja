#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {  // Cadastro de Clientes
    int codC;
    char nome[50];
    char nomeSocial[20];
    char cpf[15];
    char rua[100];
    char bairro[100];
    char numero[20];
} Cliente;

typedef struct {  // Cadastro de Produtos
    int codigo;
    char nomeProduto[100];
    int categoria;
    float precoCompra;
    float margemLucro;
    float precoVenda;
    int estoqueMin;
    int estoque;
} Produto;

typedef struct {  // Carrinho de Compras
    int codigo;
    char nomeProduto[100];
    float precoVenda;
    int quantidade;
} ItemCarrinho;

typedef struct {   // Vendas
    int numVenda;
    int codProduto;
    char descricao[100];
    float precoVenda;
    int quantidade;
    float totalItem;
    char pagamento; 
} Venda;

typedef struct {  // Pagamentos
    int numVenda;
    float valorPago;
    char tipo[2]; 
} Pagamento;

int qtdC = 0;
int qtdP = 0;
int capacidadeC = 0;
int capacidadeP = 0;

Cliente  *clientes = NULL;
Produto  *produtos = NULL;
Venda    *vendas   = NULL;
ItemCarrinho carrinho[100];
int   qtdCarrinho  = 0;
float totalCarrinho = 0.0;
float saldoCaixa = 0.0;
int   caixaAberto = 0; 

int lerInt(const char *letra) {
    int valor;
    printf("%s", letra);
    while (scanf("%d", &valor) != 1) {
        printf("Entrada invalida, digite um numero inteiro: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return valor;
}

float lerFloat(const char *letra) {
    float valor;
    printf("%s", letra);
    while (scanf("%f", &valor) != 1) {
        printf("Entrada invalida, digite um numero: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return valor;
}

// ─── MENU DE CADASTROS ─────────────────────────────────────────────────────────────

void menuCadastro() {
    int opc;

    printf("\n===== Cadastros =====\n");
    printf("1 - Cadastrar cliente\n");
    printf("2 - Cadastrar produto\n");
    printf("3 - Voltar ao menu principal\n");
    opc = lerInt("Escolha: ");

    switch (opc) {

        case 1: { 
            if (qtdC >= capacidadeC) {
                capacidadeC = (capacidadeC == 0) ? 10 : capacidadeC * 2;
                clientes = realloc(clientes, capacidadeC * sizeof(Cliente));
                if (!clientes) { printf("Erro de memoria!\n"); exit(1); }
            }

            clientes[qtdC].codC = qtdC + 1;

            printf("Digite o nome: ");
            fgets(clientes[qtdC].nome, sizeof(clientes[qtdC].nome), stdin);
            clientes[qtdC].nome[strcspn(clientes[qtdC].nome, "\n")] = '\0';

            printf("Digite o nome social: ");
            fgets(clientes[qtdC].nomeSocial, sizeof(clientes[qtdC].nomeSocial), stdin);
            clientes[qtdC].nomeSocial[strcspn(clientes[qtdC].nomeSocial, "\n")] = '\0';

            printf("Digite o CPF: ");
            fgets(clientes[qtdC].cpf, sizeof(clientes[qtdC].cpf), stdin);
            clientes[qtdC].cpf[strcspn(clientes[qtdC].cpf, "\n")] = '\0';

            printf("Digite a rua: ");
            fgets(clientes[qtdC].rua, sizeof(clientes[qtdC].rua), stdin);
            clientes[qtdC].rua[strcspn(clientes[qtdC].rua, "\n")] = '\0';

            printf("Digite o bairro: ");
            fgets(clientes[qtdC].bairro, sizeof(clientes[qtdC].bairro), stdin);
            clientes[qtdC].bairro[strcspn(clientes[qtdC].bairro, "\n")] = '\0';

            printf("Digite o telefone/WhatsApp: ");
            fgets(clientes[qtdC].numero, sizeof(clientes[qtdC].numero), stdin);
            clientes[qtdC].numero[strcspn(clientes[qtdC].numero, "\n")] = '\0';

            qtdC++;
            printf("Cliente %d cadastrado com sucesso!\n", clientes[qtdC - 1].codC);
            break;
        }

        case 2: {
            if (qtdP >= capacidadeP) {
                capacidadeP = (capacidadeP == 0) ? 10 : capacidadeP * 2;
                produtos = realloc(produtos, capacidadeP * sizeof(Produto));
                if (!produtos) { printf("Erro de memoria!\n"); exit(1); }
            }

            produtos[qtdP].codigo = 1000 + qtdP;

            printf("Digite o nome do produto: ");
            fgets(produtos[qtdP].nomeProduto, sizeof(produtos[qtdP].nomeProduto), stdin);
            produtos[qtdP].nomeProduto[strcspn(produtos[qtdP].nomeProduto, "\n")] = '\0';

            printf("Categoria:\n");
            printf("  1 - Alimentos\n");
            printf("  2 - Material de limpeza\n");
            printf("  3 - Panificadora\n");
            produtos[qtdP].categoria = lerInt("Escolha: ");
            produtos[qtdP].precoCompra = lerFloat("Preco de custo: R$ ");
            produtos[qtdP].margemLucro = lerFloat("Margem de lucro (%): ");
            produtos[qtdP].precoVenda  = produtos[qtdP].precoCompra * (1.0 + produtos[qtdP].margemLucro / 100.0);

            printf("Preco de venda calculado: R$ %.2f\n", produtos[qtdP].precoVenda);

            produtos[qtdP].estoque    = lerInt("Quantidade em estoque: ");
            produtos[qtdP].estoqueMin = lerInt("Estoque minimo: ");

            qtdP++;
            printf("Produto %d cadastrado com sucesso!\n", produtos[qtdP - 1].codigo);
            break;
        }

        case 3:
            printf("Voltando ao menu principal...\n");
            break;

        default:
            printf("Opcao invalida!\n");
            break;
    }
}

// ─── MENU VENDAS ─────────────────────────────────────────────────────────────

void menuVendas() {
    int opc, continuar = 1, forma = 0, cod, qtd, idproduto, posCarrinho = -1;
    float sub, valor, valorComDesconto, recebido = 0.0, desconto = 0.0;

    printf("\n===== Menu de Vendas =====\n");
    printf("1 - Nova venda\n");
    printf("2 - Retirada de caixa (SANGRIA)\n");
    printf("3 - Pagamento\n");
    printf("4 - Menu principal\n");
    opc = lerInt("Escolha: ");

    switch (opc) {

        case 1: {
            if (!caixaAberto) {
                printf("Caixa fechado! Abra o caixa antes de realizar vendas.\n");
            } else if (qtdP == 0) {
                printf("Nenhum produto cadastrado! Retornando ao menu...\n");
            } else {

                while (continuar) {
                    printf("\n---- Produtos Disponíveis ----\n");
                    printf("%-6s %-30s %-15s %-10s %-8s\n",
                           "Cod", "Descricao", "Categoria", "Pr.Venda", "Estoque");
                    for (int i = 0; i < qtdP; i++) {
                        char *qualcat = (produtos[i].categoria == 1) ? "Alimentos" :
                                        (produtos[i].categoria == 2) ? "Limpeza"   : "Panificadora";
                        printf("%-6d %-30s %-15s %-10.2f %-8d\n",
                               produtos[i].codigo,
                               produtos[i].nomeProduto,
                               qualcat,
                               produtos[i].precoVenda,
                               produtos[i].estoque);
                    }
                    if (qtdCarrinho > 0) {
                        printf("\n---- Carrinho Atual ----\n");
                        printf("%-6s %-30s %-10s %-6s %-10s\n",
                               "Cod", "Produto", "Pr.Unit", "Qtd", "Subtotal");
                        for (int i = 0; i < qtdCarrinho; i++) {
                            sub = carrinho[i].precoVenda * carrinho[i].quantidade;
                            printf("%-6d %-30s %-10.2f %-6d %-10.2f\n",
                                   carrinho[i].codigo,
                                   carrinho[i].nomeProduto,
                                   carrinho[i].precoVenda,
                                   carrinho[i].quantidade,
                                   sub);
                        }
                        printf("TOTAL: R$ %.2f\n", totalCarrinho);
                    }

                    cod = lerInt("\nCodigo do produto (0 para encerrar): ");

                    if (cod == 0) {
                        continuar = 0;
                    } else {
                        qtd = lerInt("Quantidade: ");

                        if (qtd <= 0) {
                            printf("Quantidade invalida!\n");
                        } else {
                            idproduto = -1;
                            for (int i = 0; i < qtdP; i++) {
                                if (produtos[i].codigo == cod) { idproduto = i; break; }
                            }

                            if (idproduto == -1) {
                                printf("Produto nao encontrado!\n");
                            } else if (produtos[idproduto].estoque < qtd) {
                                printf("Estoque insuficiente! Disponivel: %d\n",
                                       produtos[idproduto].estoque);
                            } else {
                                produtos[idproduto].estoque -= qtd;
                                for (int i = 0; i < qtdCarrinho; i++) {
                                    if (carrinho[i].codigo == cod) { posCarrinho = i; break; }
                                }

                                if (posCarrinho != -1) {
                                    carrinho[posCarrinho].quantidade += qtd;
                                } else if (qtdCarrinho >= 100) {
                                    printf("Carrinho cheio!\n");
                                } else {
                                    carrinho[qtdCarrinho].codigo     = produtos[idproduto].codigo;
                                    carrinho[qtdCarrinho].precoVenda = produtos[idproduto].precoVenda;
                                    carrinho[qtdCarrinho].quantidade = qtd;
                                    strcpy(carrinho[qtdCarrinho].nomeProduto, produtos[idproduto].nomeProduto);
                                    qtdCarrinho++;
                                }

                                totalCarrinho += produtos[idproduto].precoVenda * qtd;

                                printf("'%s' adicionado! Subtotal: R$ %.2f\n",
                                       produtos[idproduto].nomeProduto,
                                       produtos[idproduto].precoVenda * qtd);

                                if (produtos[idproduto].estoque <= produtos[idproduto].estoqueMin) {
                                    printf("Estoque de '%s' abaixo do minimo (%d restantes)!\n",
                                           produtos[idproduto].nomeProduto,
                                           produtos[idproduto].estoque);
                                }
                            }
                    }
                    }
                    } 
                if (qtdCarrinho > 0)
                    printf("\nCarrinho salvo. Acesse 'Pagamento' para finalizar a venda.\n");
            }

            break;
        }

        case 2: {
            if (!caixaAberto) {
                printf("Caixa fechado! Abra o caixa primeiro.\n");
            } else if (saldoCaixa <= 0) {
                printf("Caixa vazio! Sem saldo para retirada.\n");
            } else {
                printf("\n===== Retirada de Caixa (SANGRIA) =====\n");
                printf("Saldo atual em caixa: R$ %.2f\n", saldoCaixa);

                valor = lerFloat("Valor a retirar: R$ ");

                if (valor <= 0) {
                    printf("Valor invalido!\n");
                } else if (valor > saldoCaixa) {
                    printf("Saldo insuficiente! Disponivel: R$ %.2f\n", saldoCaixa);
                } else {
                    saldoCaixa -= valor;
                    printf("Sangria de R$ %.2f realizada com sucesso!\n", valor);
                    printf("Saldo restante: R$ %.2f\n", saldoCaixa);
                }
            }
            break;
        }

        case 3: 
            printf("Qual a forma de pagamento?\n");
            printf("1 - Dinheiro.\n");
            printf("2 - Cartão debito/credito.\n");
            forma = lerInt("Escolha: ");
            
            if (forma == 1) {
                recebido = lerFloat("Valor recebido: R$ ");
                if (recebido < totalCarrinho) {
                    printf("Valor insuficiente!\n");
                } else {
                if (totalCarrinho <= 50) {
                    desconto = totalCarrinho * 0.05;
                    printf("Desconto de 5%% aplicado!\n");
                } else if (totalCarrinho <= 100) {
                    desconto = totalCarrinho * 0.10;
                    printf("Desconto de 10%% aplicado!\n");
                } else {
                    desconto = totalCarrinho * 0.18;
                    printf("Desconto de 18%% aplicado!\n");
                }

                valorComDesconto = totalCarrinho - desconto;
                printf("Valor com desconto: R$ %.2f\n", valorComDesconto);
                printf("Troco: R$ %.2f\n", recebido - valorComDesconto);
                saldoCaixa += valorComDesconto;
                printf("Pagamento em dinheiro confirmado!\n");
                qtdCarrinho   = 0;
                totalCarrinho = 0.0;
                printf("Venda finalizada! Saldo em caixa: R$ %.2f\n", saldoCaixa);
                }
                } else if (forma == 2) { 
                int opc;
                do {
                    printf("O pagamento deu certo?\n");
                    printf("1 - Efetuado!\n");
                    printf("2 - Erro no pagamento! tente novamente.\n");
                    opc = lerInt("Escolha: ");

                switch(opc) {
                    case 1: 
                        printf("Pagamento no cartao de R$ %.2f confirmado.\n", valorComDesconto);
                        saldoCaixa += valorComDesconto;
                        qtdCarrinho   = 0;
                        totalCarrinho = 0.0;
                        printf("Venda finalizada! Saldo em caixa: R$ %.2f\n", saldoCaixa);
                    break;

                    case 2: 
                        printf("Tentando novamente...\n");
                        break;

            default:
                printf("Opcao invalida!\n");
            }
            } while(opc != 1);
        } 
        case 4:
            printf("Retornando ao menu principal...\n");
            break;

        default:
            printf("Opcao invalida!\n");
            break;
    }
}


void abrirCaixa() { //função abertura de caixa
    float troco;
    if (caixaAberto) {
        printf("Caixa ja esta aberto! Saldo atual: R$ %.2f\n", saldoCaixa);
    } else {
        troco = lerFloat("Informe o troco inicial: R$ ");
        if (troco < 0) {
            printf("Valor invalido!\n");
        } else {
            saldoCaixa = troco;
            caixaAberto = 1;
            printf("Caixa aberto com R$ %.2f.\n", saldoCaixa);
        }
    }
}


void fecharCaixa() { // função fechamento de caixa
    if (!caixaAberto) {
        printf("Caixa ja esta fechado!\n");
    } else if (qtdCarrinho > 0) {
        printf("Ha uma venda em aberto no carrinho! Finalize o pagamento antes de fechar.\n");
    } else {
        printf("\n===== Fechamento de Caixa =====\n");
        printf("Saldo final em caixa: R$ %.2f\n", saldoCaixa);
        saldoCaixa  = 0.0;
        caixaAberto = 0;
        printf("Caixa fechado com sucesso!\n");
    }
}


void relatorio() { // função relatorio
    int opc;
    char *qualcat;
    
    printf("\n===== Relatorios =====\n");
    printf("1 - Clientes\n");
    printf("2 - Produtos\n");
    opc = lerInt("Escolha: ");

    if (opc == 1) {
        if (qtdC == 0) {
            printf("Nenhum cliente cadastrado.\n");
        } else {
            printf("\n=== Lista de Clientes ===\n");
            for (int i = 0; i < qtdC; i++) {
                printf("Codigo: %d\n", clientes[i].codC);
                printf("Nome: %s\n", clientes[i].nome);
                printf("Nome Social: %s\n", clientes[i].nomeSocial);
                printf("CPF: %s\n", clientes[i].cpf);
                printf("Rua: %s\n", clientes[i].rua);
                printf("Bairro: %s\n", clientes[i].bairro);
                printf("Numero: %s\n", clientes[i].numero);
                printf("-----------------------------\n");
            }
        }
    } else if (opc == 2) {
        if (qtdP == 0) {
            printf("Nenhum produto cadastrado.\n");
        } else {
            printf("\n=== Lista de Produtos ===\n");
            printf("%-6s %-30s %-15s %-10s %-8s %-8s\n",
                   "Cod", "Descricao", "Categoria", "Pr.Venda", "Estoque", "Est.Min");
            for (int i = 0; i < qtdP; i++) {
                qualcat = (produtos[i].categoria == 1) ? "Alimentos" : (produtos[i].categoria == 2) ? "Limpeza"   : "Panificadora";
                printf("%-6d %-30s %-15s %-10.2f %-8d %-8d\n",
                       produtos[i].codigo,
                       produtos[i].nomeProduto,
                       qualcat,
                       produtos[i].precoVenda,
                       produtos[i].estoque,
                       produtos[i].estoqueMin);
            }
        }
    } else {
        printf("Opcao invalida!\n");
    }
}


int main() {
    int opc;

    do {
        printf("\n===== Mercearia da DONA TEREZA =====\n");
        printf("1 - Cadastros\n");
        printf("2 - Vendas\n");
        printf("3 - Abertura de caixa\n");
        printf("4 - Fechamento de caixa\n");
        printf("5 - Relatorios\n");
        printf("6 - Sair\n");
        opc = lerInt("Escolha: ");

        switch (opc) {
            case 1: menuCadastro(); break;
            case 2: menuVendas();   break;
            case 3: abrirCaixa();   break;
            case 4: fecharCaixa();  break;
            case 5: relatorio(); break;
            case 6: printf("Encerrando o sistema. Ate logo!\n"); break;
            default: printf("Opcao invalida!\n"); break;
        }

    } while (opc != 6);

    return 0;
}