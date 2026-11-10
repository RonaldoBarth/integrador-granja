/* Sistema Bere 4 - Completo
 * Modulos: Cadastros, Vendas, Caixa, Relatorios
 * Alocacao: Lista encadeada
 * Arquivo : Acesso binario sequencial
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   CONSTANTES DE ARQUIVO
   ============================================================ */
#define ARQ_USUARIOS  "usuarios.dat"
#define ARQ_CLIENTES  "clientes.dat"
#define ARQ_PRODUTOS  "produtos.dat"
#define ARQ_VENDAS    "vendas.dat"
#define ARQ_ITENS     "itens.dat"
#define ARQ_PAGAMENTOS "pagamentos.dat"
#define ARQ_CAIXA     "caixa.dat"

/* ============================================================
   STRUCTS DE DADOS
   ============================================================ */

/* Ficha do usuario do sistema */
struct dadosUsuario {
    int  codigo;
    char login[13];  /* minimo 8, maximo 12 caracteres */
    char senha[9];   /* minimo 6, maximo 8 caracteres  */
    int  tipo;       /* 1 = Administrador | 2 = Usuario */
};

/* Ficha do cliente da mercearia */
struct dadosCliente {
    int  codigo;
    char nomeCompleto[100];
    char nomeSocial[100];
    char cpf[15];
    char endereco[150];
    char bairro[60];
    char celular[20];
};

/* Ficha do produto */
struct dadosProduto {
    int   codigo;
    char  descricao[100];
    char  categoria[30];  /* Alimento / Mat.Limpeza / Panificacao */
    float precoCompra;
    float margemLucro;    /* percentual, ex: 30.0 = 30% */
    float precoVenda;     /* calculado: compra + margem */
    int   qtdEstoque;
    int   estoqueMinimo;
};

/* Ficha da venda */
struct dadosVenda {
    int   numVenda;
    int   codCliente;
    int   dia;
    int   mes;
    int   ano;
    float totalVenda;
};

/* Ficha do item de venda (cada produto do carrinho) */
struct dadosItem {
    int   numVenda;       /* vincula o item a uma venda    */
    int   codProduto;
    char  descricao[100];
    float precoVenda;     /* preco no momento da venda     */
    int   quantidade;
    float totalItem;
    char  pagamento;      /* 'a' = aberto | 'p' = pago    */
};

/* Ficha do pagamento */
struct dadosPagamento {
    int   numVenda;
    float valorPago;
    char  tipo[3];  /* "d"=dinheiro "c"=cartao "md"=misto dinheiro "mc"=misto cartao */
};

/* Ficha do caixa do dia */
struct dadosCaixa {
    float vlAbertura;
    float faturamento;
    float vlPagoD;   /* dinheiro          */
    float vlPagoC;   /* cartao            */
    float vlPagoMd;  /* misto - dinheiro  */
    float vlPagoMc;  /* misto - cartao    */
    float ajuste;
    int   aberto;    /* 1 = aberto | 0 = fechado */
};

/* ============================================================
   STRUCTS DOS NOS DAS LISTAS ENCADEADAS
   Cada no guarda um dado + ponteiro para o proximo
   ============================================================ */

struct nodoUsuario {
    struct dadosUsuario data;
    struct nodoUsuario *prox;
};

struct nodoCliente {
    struct dadosCliente data;
    struct nodoCliente *prox;
};

struct nodoProduto {
    struct dadosProduto data;
    struct nodoProduto *prox;
};

struct nodoVenda {
    struct dadosVenda data;
    struct nodoVenda *prox;
};

struct nodoItem {
    struct dadosItem data;
    struct nodoItem *prox;
};

struct nodoPagamento {
    struct dadosPagamento data;
    struct nodoPagamento *prox;
};

/* Apelidos de tipo para ponteiros */
typedef struct nodoUsuario   NodoUsuario;
typedef struct nodoCliente   NodoCliente;
typedef struct nodoProduto   NodoProduto;
typedef struct nodoVenda     NodoVenda;
typedef struct nodoItem      NodoItem;
typedef struct nodoPagamento NodoPagamento;

typedef NodoUsuario   *PtrUsuario;
typedef NodoCliente   *PtrCliente;
typedef NodoProduto   *PtrProduto;
typedef NodoVenda     *PtrVenda;
typedef NodoItem      *PtrItem;
typedef NodoPagamento *PtrPagamento;

/* ============================================================
   VARIAVEIS GLOBAIS
   ============================================================ */
PtrUsuario   listaUsuarios   = NULL;
PtrCliente   listaClientes   = NULL;
PtrProduto   listaProdutos   = NULL;
PtrVenda     listaVendas     = NULL;
PtrItem      listaItens      = NULL;
PtrPagamento listaPagamentos = NULL;

int totalUsuarios   = 0;
int totalClientes   = 0;
int totalProdutos   = 0;
int totalVendas     = 0;
int totalItens      = 0;
int totalPagamentos = 0;

struct dadosCaixa caixaDoDia;
int usuarioLogadoTipo = 0;

/* ============================================================
   PROTOTIPOS DAS FUNCOES
   ============================================================ */

/* utilitarios */
void  limparTela(void);
void  pausar(void);
float calcularPrecoVenda(float, float);

/* funcoes predicadas */
int isEmptyUsuarios(PtrUsuario);
int isEmptyClientes(PtrCliente);
int isEmptyProdutos(PtrProduto);
int isEmptyVendas(PtrVenda);
int isEmptyItens(PtrItem);
int isEmptyPagamentos(PtrPagamento);

/* insercao nas listas */
void inserirUsuario(PtrUsuario *, struct dadosUsuario);
void inserirCliente(PtrCliente *, struct dadosCliente);
void inserirProduto(PtrProduto *, struct dadosProduto);
void inserirVenda(PtrVenda *, struct dadosVenda);
void inserirItem(PtrItem *, struct dadosItem);
void inserirPagamento(PtrPagamento *, struct dadosPagamento);

/* arquivo */
void salvarUsuarios(void);
void carregarUsuarios(void);
void salvarClientes(void);
void carregarClientes(void);
void salvarProdutos(void);
void carregarProdutos(void);
void salvarVendas(void);
void carregarVendas(void);
void salvarItens(void);
void carregarItens(void);
void salvarPagamentos(void);
void carregarPagamentos(void);
void salvarCaixa(void);
void carregarCaixa(void);

/* validacao */
int validarAdmin(void);

/* cadastros */
void cadastrarUsuario(void);
void cadastrarCliente(void);
void cadastrarProduto(void);
void listarClientes(void);
void listarProdutos(void);

/* vendas */
void novaVenda(void);
void registrarPagamento(void);
void sangria(void);
void pagarVendasAbertas(void);

/* caixa */
void abrirCaixa(void);
void fecharCaixa(void);

/* relatorios */
void relClientesOrdemAlfabetica(void);
void relClientesQueCompraram(void);
void relProdutosOrdemAlfabetica(void);
void relProdutosEstoqueMinimo(void);
void relVendasPeriodo(void);
void relFaturamentoConsolidado(void);

/* menus */
int  showMenuPrincipal(void);
int  showMenuCadastros(void);
int  showMenuVendas(void);
int  showMenuAbertura(void);
int  showMenuFechamento(void);
int  showMenuRelatorios(void);
void menuCadastros(void);
void menuVendas(void);
void menuAberturaCaixa(void);
void menuFechamentoCaixa(void);
void menuRelatorios(void);

/* inicializacao */
void inicializar(void);

/* ============================================================
   FUNCOES UTILITARIAS
   ============================================================ */

void limparTela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar(void) {
    printf("\nPressione Enter para continuar...");
    getchar();
    getchar();
}

float calcularPrecoVenda(float precoCompra, float margem) {
    return precoCompra + (precoCompra * margem / 100.0);
}

/* ============================================================
   FUNCOES PREDICADAS
   ============================================================ */

int isEmptyUsuarios(PtrUsuario inicio)   { return inicio == NULL; }
int isEmptyClientes(PtrCliente inicio)   { return inicio == NULL; }
int isEmptyProdutos(PtrProduto inicio)   { return inicio == NULL; }
int isEmptyVendas(PtrVenda inicio)       { return inicio == NULL; }
int isEmptyItens(PtrItem inicio)         { return inicio == NULL; }
int isEmptyPagamentos(PtrPagamento inicio) { return inicio == NULL; }

/* ============================================================
   FUNCOES DE INSERCAO NAS LISTAS
   ============================================================ */

void inserirUsuario(PtrUsuario *inicio, struct dadosUsuario u) {
    NodoUsuario *novoPtr;

    novoPtr = malloc(sizeof(NodoUsuario));

    if (novoPtr != NULL) {
        novoPtr->data = u;
        novoPtr->prox = *inicio;
        *inicio = novoPtr;
    } else {
        printf("Usuario nao inserido. Sem memoria!\n");
    }
}

void inserirCliente(PtrCliente *inicio, struct dadosCliente c) {
    NodoCliente *novoPtr;

    novoPtr = malloc(sizeof(NodoCliente));

    if (novoPtr != NULL) {
        novoPtr->data = c;
        novoPtr->prox = *inicio;
        *inicio = novoPtr;
    } else {
        printf("Cliente nao inserido. Sem memoria!\n");
    }
}

void inserirProduto(PtrProduto *inicio, struct dadosProduto p) {
    NodoProduto *novoPtr;

    novoPtr = malloc(sizeof(NodoProduto));

    if (novoPtr != NULL) {
        novoPtr->data = p;
        novoPtr->prox = *inicio;
        *inicio = novoPtr;
    } else {
        printf("Produto nao inserido. Sem memoria!\n");
    }
}

void inserirVenda(PtrVenda *inicio, struct dadosVenda v) {
    NodoVenda *novoPtr;

    novoPtr = malloc(sizeof(NodoVenda));

    if (novoPtr != NULL) {
        novoPtr->data = v;
        novoPtr->prox = *inicio;
        *inicio = novoPtr;
    } else {
        printf("Venda nao inserida. Sem memoria!\n");
    }
}

void inserirItem(PtrItem *inicio, struct dadosItem i) {
    NodoItem *novoPtr;

    novoPtr = malloc(sizeof(NodoItem));

    if (novoPtr != NULL) {
        novoPtr->data = i;
        novoPtr->prox = *inicio;
        *inicio = novoPtr;
    } else {
        printf("Item nao inserido. Sem memoria!\n");
    }
}

void inserirPagamento(PtrPagamento *inicio, struct dadosPagamento p) {
    NodoPagamento *novoPtr;

    novoPtr = malloc(sizeof(NodoPagamento));

    if (novoPtr != NULL) {
        novoPtr->data = p;
        novoPtr->prox = *inicio;
        *inicio = novoPtr;
    } else {
        printf("Pagamento nao inserido. Sem memoria!\n");
    }
}

/* ============================================================
   FUNCOES DE ARQUIVO
   ============================================================ */

void salvarUsuarios(void) {
    FILE *arqPtr;
    PtrUsuario currPtr;

    if ((arqPtr = fopen(ARQ_USUARIOS, "wb")) == NULL) {
        printf("Erro ao salvar usuarios!\n");
        return;
    }

    fwrite(&totalUsuarios, sizeof(int), 1, arqPtr);

    currPtr = listaUsuarios;
    while (currPtr != NULL) {
        fwrite(&currPtr->data, sizeof(struct dadosUsuario), 1, arqPtr);
        currPtr = currPtr->prox;
    }

    fclose(arqPtr);
}

void carregarUsuarios(void) {
    FILE *arqPtr;
    struct dadosUsuario u;
    int total, i;

    if ((arqPtr = fopen(ARQ_USUARIOS, "rb")) == NULL)
        return;

    fread(&total, sizeof(int), 1, arqPtr);

    for (i = 0; i < total; i++) {
        fread(&u, sizeof(struct dadosUsuario), 1, arqPtr);
        inserirUsuario(&listaUsuarios, u);
        totalUsuarios++;
    }

    fclose(arqPtr);
}

void salvarClientes(void) {
    FILE *arqPtr;
    PtrCliente currPtr;

    if ((arqPtr = fopen(ARQ_CLIENTES, "wb")) == NULL) {
        printf("Erro ao salvar clientes!\n");
        return;
    }

    fwrite(&totalClientes, sizeof(int), 1, arqPtr);

    currPtr = listaClientes;
    while (currPtr != NULL) {
        fwrite(&currPtr->data, sizeof(struct dadosCliente), 1, arqPtr);
        currPtr = currPtr->prox;
    }

    fclose(arqPtr);
}

void carregarClientes(void) {
    FILE *arqPtr;
    struct dadosCliente c;
    int total, i;

    if ((arqPtr = fopen(ARQ_CLIENTES, "rb")) == NULL)
        return;

    fread(&total, sizeof(int), 1, arqPtr);

    for (i = 0; i < total; i++) {
        fread(&c, sizeof(struct dadosCliente), 1, arqPtr);
        inserirCliente(&listaClientes, c);
        totalClientes++;
    }

    fclose(arqPtr);
}

void salvarProdutos(void) {
    FILE *arqPtr;
    PtrProduto currPtr;

    if ((arqPtr = fopen(ARQ_PRODUTOS, "wb")) == NULL) {
        printf("Erro ao salvar produtos!\n");
        return;
    }

    fwrite(&totalProdutos, sizeof(int), 1, arqPtr);

    currPtr = listaProdutos;
    while (currPtr != NULL) {
        fwrite(&currPtr->data, sizeof(struct dadosProduto), 1, arqPtr);
        currPtr = currPtr->prox;
    }

    fclose(arqPtr);
}

void carregarProdutos(void) {
    FILE *arqPtr;
    struct dadosProduto p;
    int total, i;

    if ((arqPtr = fopen(ARQ_PRODUTOS, "rb")) == NULL)
        return;

    fread(&total, sizeof(int), 1, arqPtr);

    for (i = 0; i < total; i++) {
        fread(&p, sizeof(struct dadosProduto), 1, arqPtr);
        inserirProduto(&listaProdutos, p);
        totalProdutos++;
    }

    fclose(arqPtr);
}

void salvarVendas(void) {
    FILE *arqPtr;
    PtrVenda currPtr;

    if ((arqPtr = fopen(ARQ_VENDAS, "wb")) == NULL) {
        printf("Erro ao salvar vendas!\n");
        return;
    }

    fwrite(&totalVendas, sizeof(int), 1, arqPtr);

    currPtr = listaVendas;
    while (currPtr != NULL) {
        fwrite(&currPtr->data, sizeof(struct dadosVenda), 1, arqPtr);
        currPtr = currPtr->prox;
    }

    fclose(arqPtr);
}

void carregarVendas(void) {
    FILE *arqPtr;
    struct dadosVenda v;
    int total, i;

    if ((arqPtr = fopen(ARQ_VENDAS, "rb")) == NULL)
        return;

    fread(&total, sizeof(int), 1, arqPtr);

    for (i = 0; i < total; i++) {
        fread(&v, sizeof(struct dadosVenda), 1, arqPtr);
        inserirVenda(&listaVendas, v);
        totalVendas++;
    }

    fclose(arqPtr);
}

void salvarItens(void) {
    FILE *arqPtr;
    PtrItem currPtr;

    if ((arqPtr = fopen(ARQ_ITENS, "wb")) == NULL) {
        printf("Erro ao salvar itens!\n");
        return;
    }

    fwrite(&totalItens, sizeof(int), 1, arqPtr);

    currPtr = listaItens;
    while (currPtr != NULL) {
        fwrite(&currPtr->data, sizeof(struct dadosItem), 1, arqPtr);
        currPtr = currPtr->prox;
    }

    fclose(arqPtr);
}

void carregarItens(void) {
    FILE *arqPtr;
    struct dadosItem it;
    int total, i;

    if ((arqPtr = fopen(ARQ_ITENS, "rb")) == NULL)
        return;

    fread(&total, sizeof(int), 1, arqPtr);

    for (i = 0; i < total; i++) {
        fread(&it, sizeof(struct dadosItem), 1, arqPtr);
        inserirItem(&listaItens, it);
        totalItens++;
    }

    fclose(arqPtr);
}

void salvarPagamentos(void) {
    FILE *arqPtr;
    PtrPagamento currPtr;

    if ((arqPtr = fopen(ARQ_PAGAMENTOS, "wb")) == NULL) {
        printf("Erro ao salvar pagamentos!\n");
        return;
    }

    fwrite(&totalPagamentos, sizeof(int), 1, arqPtr);

    currPtr = listaPagamentos;
    while (currPtr != NULL) {
        fwrite(&currPtr->data, sizeof(struct dadosPagamento), 1, arqPtr);
        currPtr = currPtr->prox;
    }

    fclose(arqPtr);
}

void carregarPagamentos(void) {
    FILE *arqPtr;
    struct dadosPagamento pg;
    int total, i;

    if ((arqPtr = fopen(ARQ_PAGAMENTOS, "rb")) == NULL)
        return;

    fread(&total, sizeof(int), 1, arqPtr);

    for (i = 0; i < total; i++) {
        fread(&pg, sizeof(struct dadosPagamento), 1, arqPtr);
        inserirPagamento(&listaPagamentos, pg);
        totalPagamentos++;
    }

    fclose(arqPtr);
}

void salvarCaixa(void) {
    FILE *arqPtr;

    if ((arqPtr = fopen(ARQ_CAIXA, "wb")) == NULL) {
        printf("Erro ao salvar o caixa!\n");
        return;
    }

    fwrite(&caixaDoDia, sizeof(struct dadosCaixa), 1, arqPtr);
    fclose(arqPtr);
}

void carregarCaixa(void) {
    FILE *arqPtr;

    if ((arqPtr = fopen(ARQ_CAIXA, "rb")) == NULL) {
        /* caixa nunca foi aberto: inicializa zerado */
        caixaDoDia.vlAbertura  = 0;
        caixaDoDia.faturamento = 0;
        caixaDoDia.vlPagoD     = 0;
        caixaDoDia.vlPagoC     = 0;
        caixaDoDia.vlPagoMd    = 0;
        caixaDoDia.vlPagoMc    = 0;
        caixaDoDia.ajuste      = 0;
        caixaDoDia.aberto      = 0;
        return;
    }

    fread(&caixaDoDia, sizeof(struct dadosCaixa), 1, arqPtr);
    fclose(arqPtr);
}

/* ============================================================
   VALIDACAO DE ADMINISTRADOR
   ============================================================ */
int validarAdmin(void) {
    char login[13], senha[9];
    PtrUsuario currPtr;

    if (usuarioLogadoTipo == 1)
        return 1;

    printf("\n--- Acesso restrito a Administrador ---\n");
    printf("Login admin : "); scanf("%12s", login);
    printf("Senha admin : "); scanf("%8s",  senha);

    currPtr = listaUsuarios;
    while (currPtr != NULL) {
        if (currPtr->data.tipo == 1
            && strcmp(currPtr->data.login, login) == 0
            && strcmp(currPtr->data.senha, senha) == 0) {
            printf("Acesso autorizado!\n");
            return 1;
        }
        currPtr = currPtr->prox;
    }

    printf("Login ou senha invalidos. Acesso negado.\n");
    return 0;
}

/* ============================================================
   MODULO 1 - CADASTRO DE USUARIOS
   ============================================================ */
void cadastrarUsuario(void) {
    struct dadosUsuario u;

    printf("\n=== CADASTRO DE USUARIO ===\n");

    do {
        printf("Login (8 a 12 caracteres): ");
        scanf("%12s", u.login);
        if (strlen(u.login) < 8)
            printf("Login muito curto! Minimo 8 caracteres.\n");
    } while (strlen(u.login) < 8);

    do {
        printf("Senha (6 a 8 caracteres): ");
        scanf("%8s", u.senha);
        if (strlen(u.senha) < 6)
            printf("Senha muito curta! Minimo 6 caracteres.\n");
    } while (strlen(u.senha) < 6);

    do {
        printf("Tipo (1-Administrador / 2-Usuario): ");
        scanf("%d", &u.tipo);
    } while (u.tipo != 1 && u.tipo != 2);

    u.codigo = totalUsuarios + 1;

    inserirUsuario(&listaUsuarios, u);
    totalUsuarios++;
    salvarUsuarios();

    printf("Usuario cadastrado com sucesso! Codigo: %d\n", u.codigo);
}

/* ============================================================
   MODULO 2 - CADASTRO DE CLIENTES
   ============================================================ */
void cadastrarCliente(void) {
    struct dadosCliente c;

    printf("\n=== CADASTRO DE CLIENTE ===\n");

    c.codigo = totalClientes + 1;

    printf("Nome completo   : "); scanf(" %[^\n]", c.nomeCompleto);
    printf("Nome social     : "); scanf(" %[^\n]", c.nomeSocial);
    printf("CPF             : "); scanf(" %[^\n]", c.cpf);
    printf("Rua e numero    : "); scanf(" %[^\n]", c.endereco);
    printf("Bairro          : "); scanf(" %[^\n]", c.bairro);
    printf("Celular/WhatsApp: "); scanf(" %[^\n]", c.celular);

    inserirCliente(&listaClientes, c);
    totalClientes++;
    salvarClientes();

    printf("Cliente cadastrado com sucesso! Codigo: %d\n", c.codigo);
}

void listarClientes(void) {
    PtrCliente currPtr;

    if (isEmptyClientes(listaClientes)) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    printf("\n%-5s %-40s %-15s %-20s\n", "COD", "NOME", "CPF", "CELULAR");
    printf("-----------------------------------------------------------------------\n");

    currPtr = listaClientes;
    while (currPtr != NULL) {
        printf("%-5d %-40s %-15s %-20s\n",
               currPtr->data.codigo,
               currPtr->data.nomeCompleto,
               currPtr->data.cpf,
               currPtr->data.celular);
        currPtr = currPtr->prox;
    }
}

/* ============================================================
   MODULO 3 - CADASTRO DE PRODUTOS
   ============================================================ */
void cadastrarProduto(void) {
    struct dadosProduto p;
    int opc;

    printf("\n=== CADASTRO DE PRODUTO ===\n");

    p.codigo = totalProdutos + 1;

    printf("Descricao       : "); scanf(" %[^\n]", p.descricao);

    do {
        printf("Categoria:\n  1 - Alimento\n  2 - Material de Limpeza\n  3 - Panificacao\nOpcao: ");
        scanf("%d", &opc);
    } while (opc < 1 || opc > 3);

    if      (opc == 1) strcpy(p.categoria, "Alimento");
    else if (opc == 2) strcpy(p.categoria, "Material de Limpeza");
    else               strcpy(p.categoria, "Panificacao");

    printf("Preco de compra : R$ "); scanf("%f", &p.precoCompra);
    printf("Margem de lucro : %% "); scanf("%f", &p.margemLucro);

    p.precoVenda = calcularPrecoVenda(p.precoCompra, p.margemLucro);
    printf("Preco de venda calculado: R$ %.2f\n", p.precoVenda);

    printf("Qtd em estoque  : "); scanf("%d", &p.qtdEstoque);
    printf("Estoque minimo  : "); scanf("%d", &p.estoqueMinimo);

    inserirProduto(&listaProdutos, p);
    totalProdutos++;
    salvarProdutos();

    printf("Produto cadastrado com sucesso! Codigo: %d\n", p.codigo);
}

void listarProdutos(void) {
    PtrProduto currPtr;

    if (isEmptyProdutos(listaProdutos)) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("\n%-5s %-30s %-20s %10s %6s %6s\n",
           "COD", "DESCRICAO", "CATEGORIA", "VL.VENDA", "ESTQ", "MIN");
    printf("-------------------------------------------------------------------------\n");

    currPtr = listaProdutos;
    while (currPtr != NULL) {
        printf("%-5d %-30s %-20s R$%8.2f %6d %6d\n",
               currPtr->data.codigo,
               currPtr->data.descricao,
               currPtr->data.categoria,
               currPtr->data.precoVenda,
               currPtr->data.qtdEstoque,
               currPtr->data.estoqueMinimo);
        currPtr = currPtr->prox;
    }
}

/* ============================================================
   MODULO 4 - VENDAS
   ============================================================ */

/* Registra uma nova venda com carrinho de compras */
void novaVenda(void) {
    struct dadosVenda v;
    struct dadosItem  it;
    PtrProduto        currPtr;
    int    codProd, qtd, codCliente;
    char   continua;
    float  totalCarrinho, desconto, totalFinal;

    /* Regra: nao permite vender sem caixa aberto */
    if (caixaDoDia.aberto == 0) {
        printf("ATENCAO: Abra o caixa antes de realizar vendas!\n");
        return;
    }

    printf("\n=== NOVA VENDA ===\n");
    printf("Codigo do cliente (0 para venda sem cliente): ");
    scanf("%d", &codCliente);

    v.numVenda   = totalVendas + 1;
    v.codCliente = codCliente;
    v.totalVenda = 0;
    /* data simplificada: informada pelo operador */
    printf("Dia : "); scanf("%d", &v.dia);
    printf("Mes : "); scanf("%d", &v.mes);
    printf("Ano : "); scanf("%d", &v.ano);

    totalCarrinho = 0;

    /* exibe tabela de produtos disponiveis */
    printf("\n%-6s %-30s %-20s %10s %6s\n", "COD", "DESCRICAO", "CATEGORIA", "VL.VENDA", "ESTQ");
    printf("------------------------------------------------------------------------\n");
    currPtr = listaProdutos;
    while (currPtr != NULL) {
        printf("%-6d %-30s %-20s R$%8.2f %6d\n",
               currPtr->data.codigo,
               currPtr->data.descricao,
               currPtr->data.categoria,
               currPtr->data.precoVenda,
               currPtr->data.qtdEstoque);
        currPtr = currPtr->prox;
    }

    /* loop do carrinho */
    do {
        printf("\nInforme o codigo do produto: ");
        scanf("%d", &codProd);

        /* busca o produto na lista */
        currPtr = listaProdutos;
        while (currPtr != NULL && currPtr->data.codigo != codProd)
            currPtr = currPtr->prox;

        if (currPtr == NULL) {
            printf("Produto nao encontrado!\n");
        } else {
            /* regra: nao vender com estoque zerado */
            if (currPtr->data.qtdEstoque == 0) {
                printf("ATENCAO: Estoque zerado para este produto! Venda nao permitida.\n");
            } else {
                printf("Quantidade: ");
                scanf("%d", &qtd);

                /* regra: quantidade maior que estoque */
                if (qtd > currPtr->data.qtdEstoque) {
                    char resp;
                    printf("ATENCAO: Voce deseja vender %d produto(s). Ha somente %d em estoque! Continuar (s/n)? ",
                           qtd, currPtr->data.qtdEstoque);
                    scanf(" %c", &resp);
                    if (resp != 's' && resp != 'S') {
                        printf("Item cancelado.\n");
                        goto proximoItem;
                    }
                    qtd = currPtr->data.qtdEstoque; /* ajusta para o maximo disponivel */
                }

                /* monta o item */
                it.numVenda   = v.numVenda;
                it.codProduto = currPtr->data.codigo;
                strcpy(it.descricao, currPtr->data.descricao);
                it.precoVenda = currPtr->data.precoVenda;
                it.quantidade = qtd;
                it.totalItem  = it.precoVenda * qtd;
                it.pagamento  = 'a'; /* aberto ate o pagamento */

                inserirItem(&listaItens, it);
                totalItens++;

                /* atualiza estoque */
                currPtr->data.qtdEstoque -= qtd;
                salvarProdutos();

                /* regra: alerta estoque minimo */
                if (currPtr->data.qtdEstoque <= currPtr->data.estoqueMinimo)
                    printf("ATENCAO: Estoque minimo atingido para '%s'!\n", currPtr->data.descricao);

                totalCarrinho += it.totalItem;
                printf("Item adicionado! Total do carrinho: R$ %.2f\n", totalCarrinho);
            }
        }

        proximoItem:
        printf("Novo item no carrinho (s/n)? ");
        scanf(" %c", &continua);

    } while (continua == 's' || continua == 'S');

    /* exibe resumo do carrinho */
    printf("\n%-30s %8s %6s %10s\n", "DESCRICAO", "VL.UNIT", "QTD", "TOTAL");
    printf("--------------------------------------------------------------\n");

    PtrItem currItem = listaItens;
    while (currItem != NULL) {
        if (currItem->data.numVenda == v.numVenda) {
            printf("%-30s R$%6.2f %6d R$%8.2f\n",
                   currItem->data.descricao,
                   currItem->data.precoVenda,
                   currItem->data.quantidade,
                   currItem->data.totalItem);
        }
        currItem = currItem->prox;
    }

    printf("--------------------------------------------------------------\n");
    printf("Total carrinho: R$ %.2f\n", totalCarrinho);

    printf("Desconto (informe 0 para nao ou %% concedido): ");
    scanf("%f", &desconto);

    totalFinal = totalCarrinho - (totalCarrinho * desconto / 100.0);
    printf("Total final: R$ %.2f\n", totalFinal);

    v.totalVenda = totalFinal;
    inserirVenda(&listaVendas, v);
    totalVendas++;
    salvarVendas();
    salvarItens();

    /* chama o pagamento direto apos a venda */
    registrarPagamento();
}

/* Registra pagamento de uma venda */
void registrarPagamento(void) {
    struct dadosPagamento pg;
    PtrVenda  currVenda;
    PtrItem   currItem;
    int       numVenda, opcPag;
    float     valorTotal, valorInformado, resto;

    /* busca vendas em aberto */
    printf("\n=== PAGAMENTO ===\n");
    printf("Numero da venda: ");
    scanf("%d", &numVenda);

    /* localiza a venda */
    currVenda = listaVendas;
    while (currVenda != NULL && currVenda->data.numVenda != numVenda)
        currVenda = currVenda->prox;

    if (currVenda == NULL) {
        printf("Venda nao encontrada!\n");
        return;
    }

    valorTotal = currVenda->data.totalVenda;
    printf("Total a pagar: R$ %.2f\n", valorTotal);

    printf("Pagamento:\n  1 -> Cartao\n  2 -> Dinheiro\nOpcao: ");
    scanf("%d", &opcPag);

    pg.numVenda = numVenda;

    if (opcPag == 2) {
        /* pagamento em dinheiro */
        printf("Valor recebido: R$ ");
        scanf("%f", &valorInformado);

        if (valorInformado >= valorTotal) {
            printf("Troco: R$ %.2f\n", valorInformado - valorTotal);
            pg.valorPago = valorTotal;
            strcpy(pg.tipo, "d");
            inserirPagamento(&listaPagamentos, pg);
            totalPagamentos++;
            caixaDoDia.vlPagoD += valorTotal;
        } else {
            /* pagamento misto: parte dinheiro, parte cartao */
            resto = valorTotal - valorInformado;
            printf("Valor restante R$ %.2f sera no cartao.\n", resto);

            pg.valorPago = valorInformado;
            strcpy(pg.tipo, "md");
            inserirPagamento(&listaPagamentos, pg);
            totalPagamentos++;
            caixaDoDia.vlPagoMd += valorInformado;

            pg.valorPago = resto;
            strcpy(pg.tipo, "mc");
            inserirPagamento(&listaPagamentos, pg);
            totalPagamentos++;
            caixaDoDia.vlPagoMc += resto;
        }
    } else {
        /* pagamento em cartao */
        int okCartao;
        printf("Maquina OK? (1-Sim / 0-Nao): ");
        scanf("%d", &okCartao);

        if (!okCartao) {
            printf("Pagamento no cartao nao confirmado. Escolha outra forma.\n");
            return;
        }

        printf("Valor no cartao: R$ ");
        scanf("%f", &valorInformado);

        if (valorInformado >= valorTotal) {
            pg.valorPago = valorTotal;
            strcpy(pg.tipo, "c");
            inserirPagamento(&listaPagamentos, pg);
            totalPagamentos++;
            caixaDoDia.vlPagoC += valorTotal;
        } else {
            /* misto: parte cartao, parte dinheiro */
            resto = valorTotal - valorInformado;
            printf("Valor restante R$ %.2f sera em dinheiro.\n", resto);

            pg.valorPago = valorInformado;
            strcpy(pg.tipo, "mc");
            inserirPagamento(&listaPagamentos, pg);
            totalPagamentos++;
            caixaDoDia.vlPagoMc += valorInformado;

            pg.valorPago = resto;
            strcpy(pg.tipo, "md");
            inserirPagamento(&listaPagamentos, pg);
            totalPagamentos++;
            caixaDoDia.vlPagoMd += resto;
        }
    }

    /* marca os itens da venda como pagos */
    currItem = listaItens;
    while (currItem != NULL) {
        if (currItem->data.numVenda == numVenda)
            currItem->data.pagamento = 'p';
        currItem = currItem->prox;
    }

    caixaDoDia.faturamento += valorTotal;
    salvarCaixa();
    salvarPagamentos();
    salvarItens();

    printf("Pagamento registrado com sucesso!\n");
}

/* Apresenta vendas em aberto para pagamento posterior */
void pagarVendasAbertas(void) {
    PtrItem currItem;
    int encontrou;

    printf("\n=== VENDAS EM ABERTO ===\n");
    encontrou = 0;

    currItem = listaItens;
    while (currItem != NULL) {
        if (currItem->data.pagamento == 'a') {
            printf("Venda %d | Produto: %-30s | Total: R$ %.2f\n",
                   currItem->data.numVenda,
                   currItem->data.descricao,
                   currItem->data.totalItem);
            encontrou = 1;
        }
        currItem = currItem->prox;
    }

    if (!encontrou) {
        printf("Nenhuma venda em aberto.\n");
        return;
    }

    registrarPagamento();
}

/* Retirada de dinheiro do caixa (Sangria) */
void sangria(void) {
    PtrPagamento currPtr;
    float totalDinheiro, valorRetirada;

    if (!validarAdmin()) return;

    /* totaliza o dinheiro disponivel em caixa */
    totalDinheiro = 0;
    currPtr = listaPagamentos;
    while (currPtr != NULL) {
        if (currPtr->data.tipo[0] == 'd' || strcmp(currPtr->data.tipo, "md") == 0)
            totalDinheiro += currPtr->data.valorPago;
        currPtr = currPtr->prox;
    }

    printf("\n=== RETIRADA DE CAIXA (SANGRIA) ===\n");
    printf("Total em dinheiro no caixa: R$ %.2f\n", totalDinheiro);
    printf("Valor da retirada: R$ ");
    scanf("%f", &valorRetirada);

    /* regra: deve sobrar ao menos R$50,00 */
    if (valorRetirada >= (totalDinheiro - 50.0)) {
        printf("ATENCAO: Nao e permitido retirar este valor. Deve sobrar R$50,00 de troco.\n");
        return;
    }

    printf("Retirada de R$ %.2f realizada com sucesso!\n", valorRetirada);
    printf("Saldo restante: R$ %.2f\n", totalDinheiro - valorRetirada);
}

/* ============================================================
   MODULO 5 - ABERTURA DE CAIXA
   ============================================================ */
void abrirCaixa(void) {
    float valor;

    if (!validarAdmin()) return;

    if (caixaDoDia.aberto == 1) {
        printf("O caixa ja esta aberto!\n");
        return;
    }

    printf("\n=== ABERTURA DE CAIXA ===\n");

    do {
        printf("Valor de abertura (>= 0): R$ ");
        scanf("%f", &valor);
        if (valor < 0)
            printf("Valor invalido! Informe zero ou mais.\n");
    } while (valor < 0);

    caixaDoDia.vlAbertura  = valor;
    caixaDoDia.faturamento = 0;
    caixaDoDia.vlPagoD     = 0;
    caixaDoDia.vlPagoC     = 0;
    caixaDoDia.vlPagoMd    = 0;
    caixaDoDia.vlPagoMc    = 0;
    caixaDoDia.ajuste      = 0;
    caixaDoDia.aberto      = 1;

    salvarCaixa();
    printf("Caixa aberto com sucesso! Valor inicial: R$ %.2f\n", valor);
}

/* ============================================================
   MODULO 6 - FECHAMENTO DE CAIXA
   ============================================================ */
void fecharCaixa(void) {
    float resultado;

    if (!validarAdmin()) return;

    if (caixaDoDia.aberto == 0) {
        printf("O caixa nao esta aberto!\n");
        return;
    }

    printf("\n=== FECHAMENTO DE CAIXA ===\n");
    printf("-------------------------------\n");
    printf("Qtd de vendas realizadas  : %d\n",        totalVendas);
    printf("Total faturado            : R$ %10.2f\n", caixaDoDia.faturamento);
    printf("Valor de abertura         : R$ %10.2f\n", caixaDoDia.vlAbertura);
    printf("Pagamentos em dinheiro    : R$ %10.2f\n", caixaDoDia.vlPagoD);
    printf("Pagamentos em cartao      : R$ %10.2f\n", caixaDoDia.vlPagoC);
    printf("Misto (parte dinheiro)    : R$ %10.2f\n", caixaDoDia.vlPagoMd);
    printf("Misto (parte cartao)      : R$ %10.2f\n", caixaDoDia.vlPagoMc);
    printf("-------------------------------\n");

    resultado = caixaDoDia.faturamento
              - caixaDoDia.vlAbertura
              - caixaDoDia.vlPagoD
              - caixaDoDia.vlPagoC
              - caixaDoDia.vlPagoMd
              - caixaDoDia.vlPagoMc;

    caixaDoDia.ajuste = resultado;

    if (resultado != 0.0) {
        printf("\nATENCAO: O caixa nao podera ser fechado porque ha divergencia de valores.\n");
        if (resultado > 0)
            printf("Sobra de R$ %.2f em caixa.\n",  resultado);
        else
            printf("Falta de R$ %.2f em caixa.\n", -resultado);
        printf("Ajuste necessario antes de fechar.\n");
    } else {
        caixaDoDia.aberto = 0;
        salvarCaixa();
        printf("\nCaixa fechado com sucesso! Fechamento: R$ 0,00 (sem divergencia).\n");
    }
}

/* ============================================================
   MODULO 7 - RELATORIOS
   ============================================================ */

/* 5.1.1 Clientes em ordem alfabetica (bubble sort por nome) */
void relClientesOrdemAlfabetica(void) {
    PtrCliente currPtr, prevPtr;
    struct dadosCliente temp;
    int trocou;

    if (isEmptyClientes(listaClientes)) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    /* ordena a lista por nomeCompleto usando bubble sort */
    do {
        trocou = 0;
        currPtr = listaClientes;
        while (currPtr->prox != NULL) {
            if (strcmp(currPtr->data.nomeCompleto, currPtr->prox->data.nomeCompleto) > 0) {
                temp             = currPtr->data;
                currPtr->data    = currPtr->prox->data;
                currPtr->prox->data = temp;
                trocou = 1;
            }
            currPtr = currPtr->prox;
        }
    } while (trocou);

    printf("\n=== CLIENTES - ORDEM ALFABETICA ===\n");
    printf("%-5s %-40s %-15s %-20s\n", "COD", "NOME", "CPF", "CELULAR");
    printf("-----------------------------------------------------------------------\n");

    currPtr = listaClientes;
    while (currPtr != NULL) {
        printf("%-5d %-40s %-15s %-20s\n",
               currPtr->data.codigo,
               currPtr->data.nomeCompleto,
               currPtr->data.cpf,
               currPtr->data.celular);
        currPtr = currPtr->prox;
    }
}

/* 5.1.2 Clientes que compraram em um periodo */
void relClientesQueCompraram(void) {
    int diaIni, mesIni, anoIni, diaFim, mesFim, anoFim;
    PtrVenda    currVenda;
    PtrCliente  currCliente;

    printf("\n=== CLIENTES QUE COMPRARAM ===\n");
    printf("Periodo inicial - Dia/Mes/Ano: ");
    scanf("%d/%d/%d", &diaIni, &mesIni, &anoIni);
    printf("Periodo final   - Dia/Mes/Ano: ");
    scanf("%d/%d/%d", &diaFim, &mesFim, &anoFim);

    printf("\n%-5s %-40s %-10s\n", "COD", "CLIENTE", "DATA");
    printf("--------------------------------------------------------\n");

    currVenda = listaVendas;
    while (currVenda != NULL) {
        /* filtra pelo periodo informado (comparacao simples por ano/mes/dia) */
        int dentroDoperiodo =
            (currVenda->data.ano > anoIni || (currVenda->data.ano == anoIni && currVenda->data.mes > mesIni) ||
            (currVenda->data.ano == anoIni && currVenda->data.mes == mesIni && currVenda->data.dia >= diaIni)) &&
            (currVenda->data.ano < anoFim || (currVenda->data.ano == anoFim && currVenda->data.mes < mesFim) ||
            (currVenda->data.ano == anoFim && currVenda->data.mes == mesFim && currVenda->data.dia <= diaFim));

        if (dentroDoperiodo) {
            /* busca nome do cliente */
            currCliente = listaClientes;
            while (currCliente != NULL && currCliente->data.codigo != currVenda->data.codCliente)
                currCliente = currCliente->prox;

            if (currCliente != NULL) {
                printf("%-5d %-40s %02d/%02d/%04d\n",
                       currCliente->data.codigo,
                       currCliente->data.nomeCompleto,
                       currVenda->data.dia,
                       currVenda->data.mes,
                       currVenda->data.ano);
            }
        }
        currVenda = currVenda->prox;
    }
}

/* 5.2.1 Produtos em ordem alfabetica */
void relProdutosOrdemAlfabetica(void) {
    PtrProduto currPtr;
    struct dadosProduto temp;
    int trocou;

    if (isEmptyProdutos(listaProdutos)) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    /* ordena por descricao */
    do {
        trocou = 0;
        currPtr = listaProdutos;
        while (currPtr->prox != NULL) {
            if (strcmp(currPtr->data.descricao, currPtr->prox->data.descricao) > 0) {
                temp                = currPtr->data;
                currPtr->data       = currPtr->prox->data;
                currPtr->prox->data = temp;
                trocou = 1;
            }
            currPtr = currPtr->prox;
        }
    } while (trocou);

    printf("\n=== PRODUTOS - ORDEM ALFABETICA ===\n");
    printf("%-5s %-30s %-20s %10s %6s\n", "COD", "DESCRICAO", "CATEGORIA", "VL.VENDA", "ESTQ");
    printf("------------------------------------------------------------------------\n");

    currPtr = listaProdutos;
    while (currPtr != NULL) {
        printf("%-5d %-30s %-20s R$%8.2f %6d\n",
               currPtr->data.codigo,
               currPtr->data.descricao,
               currPtr->data.categoria,
               currPtr->data.precoVenda,
               currPtr->data.qtdEstoque);
        currPtr = currPtr->prox;
    }
}

/* 5.2.2 Produtos com estoque zero ou no minimo */
void relProdutosEstoqueMinimo(void) {
    PtrProduto currPtr;
    int encontrou;

    printf("\n=== PRODUTOS COM ESTOQUE ZERO OU MINIMO ===\n");
    printf("%-5s %-30s %6s %6s\n", "COD", "DESCRICAO", "ESTQ", "MIN");
    printf("--------------------------------------------------\n");

    encontrou = 0;
    currPtr = listaProdutos;
    while (currPtr != NULL) {
        if (currPtr->data.qtdEstoque <= currPtr->data.estoqueMinimo) {
            printf("%-5d %-30s %6d %6d\n",
                   currPtr->data.codigo,
                   currPtr->data.descricao,
                   currPtr->data.qtdEstoque,
                   currPtr->data.estoqueMinimo);
            encontrou = 1;
        }
        currPtr = currPtr->prox;
    }

    if (!encontrou)
        printf("Nenhum produto com estoque critico.\n");
}

/* 5.3.1 Vendas em um periodo */
void relVendasPeriodo(void) {
    int diaIni, mesIni, anoIni, diaFim, mesFim, anoFim;
    PtrVenda currVenda;

    printf("\n=== VENDAS POR PERIODO ===\n");
    printf("Periodo inicial - Dia/Mes/Ano: ");
    scanf("%d/%d/%d", &diaIni, &mesIni, &anoIni);
    printf("Periodo final   - Dia/Mes/Ano: ");
    scanf("%d/%d/%d", &diaFim, &mesFim, &anoFim);

    printf("\n%-8s %-10s %12s\n", "N.VENDA", "DATA", "TOTAL");
    printf("----------------------------------\n");

    currVenda = listaVendas;
    while (currVenda != NULL) {
        int dentroDoperiodo =
            (currVenda->data.ano > anoIni || (currVenda->data.ano == anoIni && currVenda->data.mes > mesIni) ||
            (currVenda->data.ano == anoIni && currVenda->data.mes == mesIni && currVenda->data.dia >= diaIni)) &&
            (currVenda->data.ano < anoFim || (currVenda->data.ano == anoFim && currVenda->data.mes < mesFim) ||
            (currVenda->data.ano == anoFim && currVenda->data.mes == mesFim && currVenda->data.dia <= diaFim));

        if (dentroDoperiodo) {
            printf("%-8d %02d/%02d/%04d R$%10.2f\n",
                   currVenda->data.numVenda,
                   currVenda->data.dia,
                   currVenda->data.mes,
                   currVenda->data.ano,
                   currVenda->data.totalVenda);
        }
        currVenda = currVenda->prox;
    }
}

/* 5.3.2 Faturamento consolidado por periodo */
void relFaturamentoConsolidado(void) {
    int diaIni, mesIni, anoIni, diaFim, mesFim, anoFim;
    PtrVenda     currVenda;
    PtrPagamento currPag;
    float totalDinheiro, totalCartao, totalGeral;

    printf("\n=== FATURAMENTO CONSOLIDADO ===\n");
    printf("Periodo inicial - Dia/Mes/Ano: ");
    scanf("%d/%d/%d", &diaIni, &mesIni, &anoIni);
    printf("Periodo final   - Dia/Mes/Ano: ");
    scanf("%d/%d/%d", &diaFim, &mesFim, &anoFim);

    totalDinheiro = 0;
    totalCartao   = 0;
    totalGeral    = 0;

    /* percorre as vendas do periodo */
    currVenda = listaVendas;
    while (currVenda != NULL) {
        int dentroDoperiodo =
            (currVenda->data.ano > anoIni || (currVenda->data.ano == anoIni && currVenda->data.mes > mesIni) ||
            (currVenda->data.ano == anoIni && currVenda->data.mes == mesIni && currVenda->data.dia >= diaIni)) &&
            (currVenda->data.ano < anoFim || (currVenda->data.ano == anoFim && currVenda->data.mes < mesFim) ||
            (currVenda->data.ano == anoFim && currVenda->data.mes == mesFim && currVenda->data.dia <= diaFim));

        if (dentroDoperiodo) {
            totalGeral += currVenda->data.totalVenda;

            /* busca pagamentos desta venda */
            currPag = listaPagamentos;
            while (currPag != NULL) {
                if (currPag->data.numVenda == currVenda->data.numVenda) {
                    if (currPag->data.tipo[0] == 'd' || strcmp(currPag->data.tipo, "md") == 0)
                        totalDinheiro += currPag->data.valorPago;
                    else
                        totalCartao += currPag->data.valorPago;
                }
                currPag = currPag->prox;
            }
        }
        currVenda = currVenda->prox;
    }

    printf("-------------------------------\n");
    printf("Total recebido em dinheiro: R$ %.2f\n", totalDinheiro);
    printf("Total recebido em cartao  : R$ %.2f\n", totalCartao);
    printf("Total geral               : R$ %.2f\n", totalGeral);
}

/* ============================================================
   MENUS - retornam int
   ============================================================ */

int showMenuPrincipal(void) {
    int op;
    limparTela();
    printf("============================================\n");
    printf("      SISTEMA BERE - MERCEARIA DONA BERE   \n");
    printf("============================================\n");
    printf("  1 -> Cadastros\n");
    printf("  2 -> Vendas\n");
    printf("  3 -> Abertura de Caixa\n");
    printf("  4 -> Fechamento de Caixa\n");
    printf("  5 -> Relatorios\n");
    printf("  0 -> Sair\n");
    printf("============================================\n");
    printf("Opcao: ");
    scanf("%d", &op);
    return op;
}

int showMenuCadastros(void) {
    int op;
    limparTela();
    printf("========== CADASTROS ==========\n");
    printf("  1 -> Cadastro de Usuarios\n");
    printf("  2 -> Cadastro de Clientes\n");
    printf("  3 -> Cadastro de Produtos\n");
    printf("  4 -> Listar Clientes\n");
    printf("  5 -> Listar Produtos\n");
    printf("  0 -> Retornar ao Menu Principal\n");
    printf("================================\n");
    printf("Opcao: ");
    scanf("%d", &op);
    return op;
}

int showMenuVendas(void) {
    int op;
    limparTela();
    printf("========== VENDAS ==========\n");
    printf("  1 -> Nova Venda\n");
    printf("  2 -> Retirada de Caixa (Sangria)\n");
    printf("  3 -> Pagar Vendas em Aberto\n");
    printf("  0 -> Retornar ao Menu Principal\n");
    printf("============================\n");
    printf("Opcao: ");
    scanf("%d", &op);
    return op;
}

int showMenuAbertura(void) {
    int op;
    limparTela();
    printf("======= ABERTURA DE CAIXA =======\n");
    printf("  1 -> Abrir o Caixa\n");
    printf("  0 -> Retornar ao Menu Principal\n");
    printf("==================================\n");
    printf("Opcao: ");
    scanf("%d", &op);
    return op;
}

int showMenuFechamento(void) {
    int op;
    limparTela();
    printf("======= FECHAMENTO DE CAIXA =======\n");
    printf("  1 -> Fechar o Caixa\n");
    printf("  0 -> Retornar ao Menu Principal\n");
    printf("====================================\n");
    printf("Opcao: ");
    scanf("%d", &op);
    return op;
}

int showMenuRelatorios(void) {
    int op;
    limparTela();
    printf("========== RELATORIOS ==========\n");
    printf("  1 -> Clientes - Ordem Alfabetica\n");
    printf("  2 -> Clientes que Compraram (periodo)\n");
    printf("  3 -> Produtos - Ordem Alfabetica\n");
    printf("  4 -> Produtos com Estoque Zero ou Minimo\n");
    printf("  5 -> Vendas por Periodo\n");
    printf("  6 -> Faturamento Consolidado\n");
    printf("  0 -> Retornar ao Menu Principal\n");
    printf("=================================\n");
    printf("Opcao: ");
    scanf("%d", &op);
    return op;
}

/* ============================================================
   CONTROLADORES DE MENU
   ============================================================ */

void menuCadastros(void) {
    int op;

    op = showMenuCadastros();

    while (op != 0) {
        switch (op) {
            case 1: cadastrarUsuario(); pausar(); break;
            case 2: cadastrarCliente(); pausar(); break;
            case 3: cadastrarProduto(); pausar(); break;
            case 4: listarClientes();   pausar(); break;
            case 5: listarProdutos();   pausar(); break;
            default:
                printf("Opcao invalida!\n");
                pausar();
                break;
        }
        op = showMenuCadastros();
    }
}

void menuVendas(void) {
    int op;

    op = showMenuVendas();

    while (op != 0) {
        switch (op) {
            case 1: novaVenda();          pausar(); break;
            case 2: sangria();            pausar(); break;
            case 3: pagarVendasAbertas(); pausar(); break;
            default:
                printf("Opcao invalida!\n");
                pausar();
                break;
        }
        op = showMenuVendas();
    }
}

void menuAberturaCaixa(void) {
    int op;

    op = showMenuAbertura();

    while (op != 0) {
        switch (op) {
            case 1: abrirCaixa(); pausar(); break;
            default:
                printf("Opcao invalida!\n");
                pausar();
                break;
        }
        op = showMenuAbertura();
    }
}

void menuFechamentoCaixa(void) {
    int op;

    op = showMenuFechamento();

    while (op != 0) {
        switch (op) {
            case 1: fecharCaixa(); pausar(); break;
            default:
                printf("Opcao invalida!\n");
                pausar();
                break;
        }
        op = showMenuFechamento();
    }
}

void menuRelatorios(void) {
    int op;

    op = showMenuRelatorios();

    while (op != 0) {
        switch (op) {
            case 1: relClientesOrdemAlfabetica(); pausar(); break;
            case 2: relClientesQueCompraram();    pausar(); break;
            case 3: relProdutosOrdemAlfabetica(); pausar(); break;
            case 4: relProdutosEstoqueMinimo();   pausar(); break;
            case 5: relVendasPeriodo();           pausar(); break;
            case 6: relFaturamentoConsolidado();  pausar(); break;
            default:
                printf("Opcao invalida!\n");
                pausar();
                break;
        }
        op = showMenuRelatorios();
    }
}

/* ============================================================
   INICIALIZACAO
   ============================================================ */
void inicializar(void) {
    carregarUsuarios();
    carregarClientes();
    carregarProdutos();
    carregarVendas();
    carregarItens();
    carregarPagamentos();
    carregarCaixa();
}

/* ============================================================
   MAIN
   ============================================================ */
int main() {
    int op;

    inicializar();

    usuarioLogadoTipo = 2;  /* comeca como usuario comum */

    op = showMenuPrincipal();

    while (op != 0) {
        switch (op) {
            case 1:
                menuCadastros();
                break;
            case 2:
                menuVendas();
                break;
            case 3:
                menuAberturaCaixa();
                break;
            case 4:
                menuFechamentoCaixa();
                break;
            case 5:
                menuRelatorios();
                break;
            default:
                printf("Opcao invalida!\n");
                pausar();
                break;
        }
        op = showMenuPrincipal();
    }

    free(listaUsuarios);
    free(listaClientes);
    free(listaProdutos);
    free(listaVendas);
    free(listaItens);
    free(listaPagamentos);

    return 0;
}