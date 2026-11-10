/*
 * ============================================================
 *  SISTEMA BERE3 - Mercearia da Dona Berê
 *  Versão simplificada para iniciantes
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 *  CONSTANTES - limites do sistema
 * ============================================================ */
#define MAX_CLIENTES  100
#define MAX_PRODUTOS  100
#define MAX_VENDAS    100
#define MAX_ITENS     20   /* itens por venda */

/* ============================================================
 *  STRUCTS - estruturas de dados (obrigatório)
 * ============================================================ */

/* Struct do Cliente */
typedef struct {
    int  codigo;
    char nome[100];
    char nomeSocial[100];
    char cpf[15];
    char rua[80];
    char bairro[60];
    char celular[16];
} Cliente;

/* Struct do Produto */
typedef struct {
    int   codigo;
    char  descricao[100];
    char  categoria[40];
    float precoCompra;
    float margemLucro;   /* em % */
    int   qtdEstoque;
    int   estoqueMinimo;
} Produto;

/* Struct de cada item dentro de uma venda */
typedef struct {
    int   codigoProduto;
    char  descricao[100];
    float precoVenda;
    int   quantidade;
    float totalItem;
} ItemVenda;

/* Struct da Venda */
typedef struct {
    int      numVenda;
    ItemVenda itens[MAX_ITENS]; /* vetor de itens (agregado homogêneo) */
    int      qtdItens;
    float    totalCarrinho;
    float    totalFinal;
    char     pagamento;  /* 'a'=Aberto  'p'=Pago */
    char     tipoPag[3]; /* "d", "c" ou "dc" */
    float    valorPago;
} Venda;

/* ============================================================
 *  VETORES GLOBAIS - armazenam os dados (obrigatório vetor)
 * ============================================================ */
Cliente *clientes[MAX_CLIENTES]; /* vetor de ponteiros (alocação dinâmica) */
Produto *produtos[MAX_PRODUTOS];
Venda   *vendas[MAX_VENDAS];

int totalClientes = 0;
int totalProdutos = 0;
int totalVendas   = 0;

float valorAbertura = 0.0;
int   caixaAberto   = 0;

/* ============================================================
 *  FUNÇÕES UTILITÁRIAS
 * ============================================================ */

/* Limpa o buffer do teclado */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Pausa e espera Enter */
void pausar() {
    printf("\nPressione Enter para continuar...");
    limparBuffer();
    getchar();
}

/* Calcula o preço de venda de um produto */
float calcularPrecoVenda(Produto *p) {
    /* preço de venda = compra + compra * margem/100 */
    return p->precoCompra + p->precoCompra * (p->margemLucro / 100.0);
}

/* Busca produto pelo código, retorna o índice ou -1 */
int buscarProduto(int codigo) {
    int i;
    for (i = 0; i < totalProdutos; i++) {
        if (produtos[i]->codigo == codigo)
            return i;
    }
    return -1; /* não encontrado */
}

/* ============================================================
 *  CADASTRO DE CLIENTES
 * ============================================================ */
void cadastrarCliente() {
    if (totalClientes >= MAX_CLIENTES) {
        printf("Limite de clientes atingido!\n");
        pausar();
        return;
    }

    /* alocação dinâmica com ponteiro (obrigatório) */
    Cliente *novo = (Cliente*) malloc(sizeof(Cliente));
    if (novo == NULL) {
        printf("Erro de memoria!\n");
        return;
    }

    novo->codigo = totalClientes + 1; /* código automático */
    printf("\n=== CADASTRO DE CLIENTE ===\n");
    printf("Codigo: %d\n", novo->codigo);

    printf("Nome completo  : ");
    fgets(novo->nome, 100, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0'; /* remove Enter */

    printf("Nome social    : ");
    fgets(novo->nomeSocial, 100, stdin);
    novo->nomeSocial[strcspn(novo->nomeSocial, "\n")] = '\0';

    printf("CPF            : ");
    fgets(novo->cpf, 15, stdin);
    novo->cpf[strcspn(novo->cpf, "\n")] = '\0';
    limparBuffer();

    printf("Rua/Numero     : ");
    fgets(novo->rua, 80, stdin);
    novo->rua[strcspn(novo->rua, "\n")] = '\0';

    printf("Bairro         : ");
    fgets(novo->bairro, 60, stdin);
    novo->bairro[strcspn(novo->bairro, "\n")] = '\0';

    printf("Celular/Whats  : ");
    fgets(novo->celular, 16, stdin);
    novo->celular[strcspn(novo->celular, "\n")] = '\0';
    limparBuffer();

    clientes[totalClientes] = novo; /* guarda o ponteiro no vetor */
    totalClientes++;

    printf("Cliente cadastrado com sucesso!\n");
    pausar();
}

/* ============================================================
 *  CADASTRO DE PRODUTOS
 * ============================================================ */
void cadastrarProduto() {
    if (totalProdutos >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido!\n");
        pausar();
        return;
    }

    /* alocação dinâmica */
    Produto *novo = (Produto*) malloc(sizeof(Produto));
    if (novo == NULL) {
        printf("Erro de memoria!\n");
        return;
    }

    novo->codigo = 1000 + totalProdutos; /* código começa em 1000 */
    printf("\n=== CADASTRO DE PRODUTO ===\n");
    printf("Codigo: %d\n", novo->codigo);

    printf("Descricao      : ");
    fgets(novo->descricao, 100, stdin);
    novo->descricao[strcspn(novo->descricao, "\n")] = '\0';

    printf("Categoria:\n");
    printf("  1. Alimento\n");
    printf("  2. Material de Limpeza\n");
    printf("  3. Panificacao\n");
    int cat;
    printf("Opcao: ");
    scanf("%d", &cat);
    limparBuffer();
    if      (cat == 1) strcpy(novo->categoria, "Alimento");
    else if (cat == 2) strcpy(novo->categoria, "Mat.Limpeza");
    else               strcpy(novo->categoria, "Panificacao");

    printf("Preco de compra: ");
    scanf("%f", &novo->precoCompra);
    limparBuffer();

    printf("Margem de lucro (%): ");
    scanf("%f", &novo->margemLucro);
    limparBuffer();

    printf("Quantidade estoque: ");
    scanf("%d", &novo->qtdEstoque);
    limparBuffer();

    printf("Estoque minimo    : ");
    scanf("%d", &novo->estoqueMinimo);
    limparBuffer();

    printf("Preco de venda calculado: R$ %.2f\n", calcularPrecoVenda(novo));

    produtos[totalProdutos] = novo;
    totalProdutos++;

    printf("Produto cadastrado!\n");
    pausar();
}

/* ============================================================
 *  NOVA VENDA
 * ============================================================ */
void novaVenda() {
    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado!\n");
        pausar();
        return;
    }
    if (totalVendas >= MAX_VENDAS) {
        printf("Limite de vendas atingido!\n");
        pausar();
        return;
    }

    /* alocação dinâmica da venda */
    Venda *v = (Venda*) malloc(sizeof(Venda));
    if (v == NULL) { printf("Erro de memoria!\n"); return; }

    v->numVenda      = totalVendas + 1;
    v->qtdItens      = 0;
    v->totalCarrinho = 0.0;
    v->totalFinal    = 0.0;
    v->pagamento     = 'a'; /* começa aberto */

    char continua = 's';

    while (continua == 's' && v->qtdItens < MAX_ITENS) {

        /* mostra produtos disponíveis */
        printf("\n%-6s %-25s %-12s %10s %6s\n",
               "Cod","Descricao","Categoria","Pr.Venda","Estq");
        printf("-----------------------------------------------------------\n");
        int i;
        for (i = 0; i < totalProdutos; i++) {
            float pv = calcularPrecoVenda(produtos[i]);
            printf("%-6d %-25s %-12s %10.2f %6d\n",
                   produtos[i]->codigo,
                   produtos[i]->descricao,
                   produtos[i]->categoria,
                   pv,
                   produtos[i]->qtdEstoque);
        }
        printf("-----------------------------------------------------------\n");

        /* mostra carrinho atual */
        if (v->qtdItens > 0) {
            printf("\n--- Carrinho ---\n");
            for (i = 0; i < v->qtdItens; i++) {
                printf("%-25s x%d  R$ %.2f\n",
                       v->itens[i].descricao,
                       v->itens[i].quantidade,
                       v->itens[i].totalItem);
            }
            printf("Total: R$ %.2f\n", v->totalCarrinho);
        }

        int cod;
        printf("\nCodigo do produto: ");
        scanf("%d", &cod);
        limparBuffer();

        int idx = buscarProduto(cod);
        if (idx == -1) {
            printf("Produto nao encontrado!\n");
            continue;
        }

        Produto *p = produtos[idx];

        /* regra: estoque zerado */
        if (p->qtdEstoque == 0) {
            printf("ALERTA: Produto sem estoque!\n");
            continue;
        }

        int qtd;
        printf("Quantidade: ");
        scanf("%d", &qtd);
        limparBuffer();

        /* regra: qtd maior que estoque */
        if (qtd > p->qtdEstoque) {
            printf("ALERTA: Voce quer %d mas so ha %d em estoque. Continuar (s/n)? ",
                   qtd, p->qtdEstoque);
            char r = getchar();
            limparBuffer();
            if (r != 's') continue;
            qtd = p->qtdEstoque;
        }

        float pv = calcularPrecoVenda(p);

        /* preenche o item no vetor da venda */
        v->itens[v->qtdItens].codigoProduto = p->codigo;
        strcpy(v->itens[v->qtdItens].descricao, p->descricao);
        v->itens[v->qtdItens].precoVenda  = pv;
        v->itens[v->qtdItens].quantidade  = qtd;
        v->itens[v->qtdItens].totalItem   = pv * qtd;

        v->totalCarrinho += pv * qtd;
        v->qtdItens++;

        /* desconta estoque */
        p->qtdEstoque -= qtd;

        /* alerta estoque mínimo */
        if (p->qtdEstoque <= p->estoqueMinimo) {
            printf("*** ALERTA: '%s' atingiu estoque minimo! ***\n", p->descricao);
        }

        printf("Novo item no carrinho (s/n)? ");
        char resp = getchar();
        limparBuffer();
        continua = resp;
    }

    if (v->qtdItens == 0) {
        printf("Nenhum item. Venda cancelada.\n");
        free(v);
        pausar();
        return;
    }

    /* desconto */
    printf("\nTotal carrinho: R$ %.2f\n", v->totalCarrinho);
    float desc;
    printf("Desconto (%%, 0 para nenhum): ");
    scanf("%f", &desc);
    limparBuffer();
    v->totalFinal = v->totalCarrinho - v->totalCarrinho * (desc / 100.0);
    printf("Total final: R$ %.2f\n", v->totalFinal);

    /* pagamento */
    printf("\n1. Cartao\n2. Dinheiro\n3. Voltar ao menu\n");
    int opcPag;
    printf("Opcao: ");
    scanf("%d", &opcPag);
    limparBuffer();

    if (opcPag == 3) {
        /* deixa em aberto */
        vendas[totalVendas] = v;
        totalVendas++;
        printf("Venda salva em aberto.\n");
        pausar();
        return;
    }

    if (opcPag == 2) {
        /* dinheiro */
        float recebido;
        printf("Valor recebido: R$ ");
        scanf("%f", &recebido);
        limparBuffer();
        if (recebido < v->totalFinal) {
            printf("Valor insuficiente! Venda fica em aberto.\n");
            strcpy(v->tipoPag, "d");
            v->valorPago = recebido;
        } else {
            printf("Troco: R$ %.2f\n", recebido - v->totalFinal);
            strcpy(v->tipoPag, "d");
            v->valorPago  = v->totalFinal;
            v->pagamento  = 'p';
        }
    } else {
        /* cartão */
        printf("Maquinha OK? (1-Sim / 0-Nao): ");
        int ok;
        scanf("%d", &ok);
        limparBuffer();
        if (ok == 1) {
            strcpy(v->tipoPag, "c");
            v->valorPago = v->totalFinal;
            v->pagamento = 'p';
            printf("Pagamento no cartao confirmado!\n");
        } else {
            printf("Cartao recusado. Venda fica em aberto.\n");
        }
    }

    vendas[totalVendas] = v;
    totalVendas++;
    pausar();
}

/* ============================================================
 *  ABERTURA DE CAIXA
 * ============================================================ */
void abrirCaixa() {
    if (caixaAberto) {
        printf("Caixa ja esta aberto!\n");
        pausar();
        return;
    }
    printf("\n=== ABERTURA DE CAIXA ===\n");
    do {
        printf("Valor de abertura (R$): ");
        scanf("%f", &valorAbertura);
        limparBuffer();
        if (valorAbertura < 0) printf("Valor deve ser >= 0.\n");
    } while (valorAbertura < 0);

    caixaAberto = 1;
    printf("Caixa aberto com R$ %.2f\n", valorAbertura);
    pausar();
}

/* ============================================================
 *  FECHAMENTO DE CAIXA
 * ============================================================ */
void fecharCaixa() {
    if (!caixaAberto) {
        printf("Caixa nao esta aberto!\n");
        pausar();
        return;
    }

    printf("\n=== FECHAMENTO DE CAIXA ===\n");

    int   qtdVendas   = 0;
    float faturamento = 0.0;
    float pagoD       = 0.0;
    float pagoC       = 0.0;
    float pagoDC      = 0.0;

    int i;
    for (i = 0; i < totalVendas; i++) {
        if (vendas[i]->pagamento == 'p') {
            qtdVendas++;
            faturamento += vendas[i]->totalFinal;
            if      (strcmp(vendas[i]->tipoPag, "d")  == 0) pagoD  += vendas[i]->valorPago;
            else if (strcmp(vendas[i]->tipoPag, "c")  == 0) pagoC  += vendas[i]->valorPago;
            else if (strcmp(vendas[i]->tipoPag, "dc") == 0) pagoDC += vendas[i]->valorPago;
        }
    }

    printf("Qtd vendas realizadas : %d\n",    qtdVendas);
    printf("Faturamento           : R$ %.2f\n", faturamento);
    printf("Valor abertura caixa  : R$ %.2f\n", valorAbertura);
    printf("Pago em dinheiro      : R$ %.2f\n", pagoD);
    printf("Pago em cartao        : R$ %.2f\n", pagoC);
    printf("Pago dinheiro+cartao  : R$ %.2f\n", pagoDC);

    float ajuste = faturamento - valorAbertura - pagoD - pagoC - pagoDC;
    printf("Ajuste                : R$ %.2f\n", ajuste);

    if (ajuste != 0.0) {
        printf("\nATENCAO: O caixa nao pode ser fechado - ha divergencia de valores!\n");
    } else {
        printf("\nCaixa fechado corretamente!\n");
        caixaAberto = 0;
    }
    pausar();
}

/* ============================================================
 *  RELATÓRIOS
 * ============================================================ */
void relatorioClientes() {
    printf("\n=== CLIENTES ===\n");
    if (totalClientes == 0) { printf("Nenhum cliente.\n"); pausar(); return; }
    int i;
    for (i = 0; i < totalClientes; i++) {
        printf("[%d] %s | CPF: %s | Cel: %s\n",
               clientes[i]->codigo,
               clientes[i]->nome,
               clientes[i]->cpf,
               clientes[i]->celular);
    }
    printf("Total: %d clientes\n", totalClientes);
    pausar();
}

void relatorioProdutos() {
    printf("\n=== PRODUTOS ===\n");
    if (totalProdutos == 0) { printf("Nenhum produto.\n"); pausar(); return; }
    int i;
    for (i = 0; i < totalProdutos; i++) {
        float pv = calcularPrecoVenda(produtos[i]);
        printf("[%d] %-25s | %s | Venda: R$ %.2f | Estq: %d",
               produtos[i]->codigo,
               produtos[i]->descricao,
               produtos[i]->categoria,
               pv,
               produtos[i]->qtdEstoque);
        if (produtos[i]->qtdEstoque <= produtos[i]->estoqueMinimo)
            printf(" *** ESTOQUE BAIXO ***");
        printf("\n");
    }
    pausar();
}

void relatorioVendas() {
    printf("\n=== VENDAS ===\n");
    if (totalVendas == 0) { printf("Nenhuma venda.\n"); pausar(); return; }
    int i, j;
    float totalGeral = 0.0;
    for (i = 0; i < totalVendas; i++) {
        printf("Venda #%d | %s | Total: R$ %.2f\n",
               vendas[i]->numVenda,
               (vendas[i]->pagamento == 'p') ? "PAGO" : "ABERTO",
               vendas[i]->totalFinal);
        for (j = 0; j < vendas[i]->qtdItens; j++) {
            printf("  - %-25s x%d  R$ %.2f\n",
                   vendas[i]->itens[j].descricao,
                   vendas[i]->itens[j].quantidade,
                   vendas[i]->itens[j].totalItem);
        }
        if (vendas[i]->pagamento == 'p') totalGeral += vendas[i]->totalFinal;
    }
    printf("Faturamento total: R$ %.2f\n", totalGeral);
    pausar();
}

/* ============================================================
 *  SALVAR / CARREGAR ARQUIVOS (desejável)
 * ============================================================ */
void salvarDados() {
    FILE *f;
    int i;

    f = fopen("clientes.dat", "wb");
    if (f) {
        fwrite(&totalClientes, sizeof(int), 1, f);
        for (i = 0; i < totalClientes; i++)
            fwrite(clientes[i], sizeof(Cliente), 1, f);
        fclose(f);
    }

    f = fopen("produtos.dat", "wb");
    if (f) {
        fwrite(&totalProdutos, sizeof(int), 1, f);
        for (i = 0; i < totalProdutos; i++)
            fwrite(produtos[i], sizeof(Produto), 1, f);
        fclose(f);
    }

    f = fopen("vendas.dat", "wb");
    if (f) {
        fwrite(&totalVendas, sizeof(int), 1, f);
        for (i = 0; i < totalVendas; i++)
            fwrite(vendas[i], sizeof(Venda), 1, f);
        fclose(f);
    }

    printf("Dados salvos!\n");
}

void carregarDados() {
    FILE *f;
    int i, total;

    f = fopen("clientes.dat", "rb");
    if (f) {
        fread(&total, sizeof(int), 1, f);
        for (i = 0; i < total; i++) {
            clientes[i] = (Cliente*) malloc(sizeof(Cliente));
            fread(clientes[i], sizeof(Cliente), 1, f);
        }
        totalClientes = total;
        fclose(f);
    }

    f = fopen("produtos.dat", "rb");
    if (f) {
        fread(&total, sizeof(int), 1, f);
        for (i = 0; i < total; i++) {
            produtos[i] = (Produto*) malloc(sizeof(Produto));
            fread(produtos[i], sizeof(Produto), 1, f);
        }
        totalProdutos = total;
        fclose(f);
    }

    f = fopen("vendas.dat", "rb");
    if (f) {
        fread(&total, sizeof(int), 1, f);
        for (i = 0; i < total; i++) {
            vendas[i] = (Venda*) malloc(sizeof(Venda));
            fread(vendas[i], sizeof(Venda), 1, f);
        }
        totalVendas = total;
        fclose(f);
    }
}

/* ============================================================
 *  MENUS
 * ============================================================ */

void menuCadastros() {
    int op;
    do {
        printf("\n=== CADASTROS ===\n");
        printf("1. Cadastro de Clientes\n");
        printf("2. Cadastro de Produtos\n");
        printf("3. Retornar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();
        if      (op == 1) cadastrarCliente();
        else if (op == 2) cadastrarProduto();
    } while (op != 3);
}

void menuVendas() {
    int op;
    do {
        printf("\n=== VENDAS ===\n");
        printf("1. Nova Venda\n");
        printf("2. Retornar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();
        if (op == 1) novaVenda();
    } while (op != 2);
}

void menuRelatorios() {
    int op;
    do {
        printf("\n=== RELATORIOS ===\n");
        printf("1. Clientes\n");
        printf("2. Produtos\n");
        printf("3. Vendas\n");
        printf("4. Retornar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();
        if      (op == 1) relatorioClientes();
        else if (op == 2) relatorioProdutos();
        else if (op == 3) relatorioVendas();
    } while (op != 4);
}

/* ============================================================
 *  FUNÇÃO PRINCIPAL - main() enxuta (obrigatório)
 * ============================================================ */
int main() {
    carregarDados(); /* carrega dados salvos, se existirem */

    int opcao;
    do {
        printf("\n========================================\n");
        printf("   SISTEMA BERE3 - Mercearia Dona Bere\n");
        printf("========================================\n");
        printf("1. Cadastros\n");
        printf("2. Vendas\n");
        printf("3. Abertura de Caixa\n");
        printf("4. Fechamento de Caixa\n");
        printf("5. Relatorios\n");
        printf("7. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if      (opcao == 1) menuCadastros();
        else if (opcao == 2) menuVendas();
        else if (opcao == 3) abrirCaixa();
        else if (opcao == 4) fecharCaixa();
        else if (opcao == 5) menuRelatorios();
        else if (opcao == 7) {
            printf("Deseja salvar os dados (s/n)? ");
            char r = getchar();
            limparBuffer();
            if (r == 's') salvarDados();
            printf("Ate logo!\n");
        }

    } while (opcao != 7);

    return 0;
}