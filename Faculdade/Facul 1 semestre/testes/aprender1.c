/* ============================================================
   SISTEMA BERE 4 — VERSÃO DIDÁTICA
   Cada linha explicada para facilitar o aprendizado e a apresentação.
   ============================================================

   O QUE ESTE PROGRAMA FAZ?
   ─────────────────────────
   É um sistema de gerenciamento para a Mercearia Berê.
   Permite cadastrar clientes e produtos, realizar vendas,
   controlar o caixa e emitir relatórios, tudo pelo terminal.

   COMO OS DADOS SÃO GUARDADOS?
   ─────────────────────────────
   • Clientes e produtos: array dinâmico (realloc — cresce conforme necessário)
   • Vendas, itens, pagamentos e usuários: lista encadeada (cada registro é
     um "nó" que aponta para o próximo, formando uma corrente)
   • Todos os dados são salvos em arquivos binários (.dat) para não se
     perderem quando o programa fecha.

   ESTRUTURA DO ARQUIVO (de cima para baixo):
   ───────────────────────────────────────────
   1. #includes e #defines          → bibliotecas e nomes de arquivos
   2. Structs (fichas de dados)     → moldes para cada tipo de informação
   3. Variáveis globais             → dados acessíveis em todo o programa
   4. Funções utilitárias           → helpers usados em vários lugares
   5. Funções de lista encadeada    → inserção nos nós
   6. Funções de arquivo            → salvar e carregar do disco
   7. Validação de admin            → controle de acesso
   8. Módulo Cadastros              → usuários, clientes, produtos
   9. Módulo Vendas                 → carrinho, pagamento, sangria
  10. Módulo Caixa                  → abertura e fechamento
  11. Módulo Relatórios             → 7 relatórios diferentes
  12. Menus e main()               → interface e ponto de entrada
   ============================================================ */


/* ============================================================
   SEÇÃO 1 — BIBLIOTECAS (#include)
   ============================================================
   #include diz ao compilador para copiar o conteúdo da biblioteca
   antes de compilar o código. Sem elas, funções como printf e
   malloc não existem.
   ============================================================ */

#include <stdio.h>    /* Entrada/Saída: printf, scanf, fgets, fopen, fclose,
                         fread, fwrite — tudo que envolve imprimir ou ler       */
#include <stdlib.h>   /* Memória e processo: malloc, realloc, free, calloc,
                         exit — funções para gerenciar memória dinâmica          */
#include <string.h>   /* Strings: strcmp (comparar), strcpy (copiar),
                         strlen (tamanho), strcspn (achar caractere),
                         memcpy (copiar bloco de memória)                        */


/* ============================================================
   SEÇÃO 2 — CONSTANTES DE ARQUIVO (#define)
   ============================================================
   #define cria um "apelido" que o compilador substitui antes de
   compilar. Assim, se quisermos mudar o nome do arquivo, mudamos
   só aqui — não em todas as funções.
   ============================================================ */

#define ARQ_CLIENTES   "clientes.dat"    /* arquivo binário dos clientes         */
#define ARQ_PRODUTOS   "produtos.dat"    /* arquivo binário dos produtos         */
#define ARQ_VENDAS     "vendas.dat"      /* arquivo binário das vendas           */
#define ARQ_ITENS      "itens.dat"       /* arquivo binário dos itens de venda   */
#define ARQ_PAGAMENTOS "pagamentos.dat"  /* arquivo binário dos pagamentos       */
#define ARQ_CAIXA      "caixa.dat"       /* arquivo binário do estado do caixa   */
#define ARQ_USUARIOS   "usuarios.dat"    /* arquivo binário dos usuários         */


/* ============================================================
   SEÇÃO 3 — STRUCTS (fichas de dados)
   ============================================================
   Uma struct é um molde que agrupa variáveis relacionadas numa
   só "ficha". É como um formulário: você preenche os campos e
   guarda tudo junto.

   typedef struct { ... } NomeDoTipo;
   → o typedef cria um apelido para a struct, então em vez de
     escrever "struct dadosCliente", escrevemos só "Cliente".
   ============================================================ */

/* ── Ficha do Cliente ──────────────────────────────────────── */
typedef struct {
    int  codC;            /* número único gerado automaticamente (1, 2, 3...)    */
    char nome[50];        /* nome completo — até 49 caracteres + '\0' (fim)      */
    char nomeSocial[20];  /* nome preferido (respeito à diversidade)             */
    char cpf[15];         /* "000.000.000-00" cabe em 14 chars + '\0'            */
    char rua[100];        /* endereço com rua e número                           */
    char bairro[100];     /* bairro                                              */
    char numero[20];      /* celular/WhatsApp                                    */
} Cliente;                /* ← esse é o nome do tipo que vamos usar no código    */

/* ── Ficha do Produto ──────────────────────────────────────── */
typedef struct {
    int   codigo;            /* código numérico a partir de 1000 (1000, 1001...) */
    char  nomeProduto[100];  /* descrição do produto                             */
    int   categoria;         /* 1=Alimentos  2=Limpeza  3=Panificadora           */
    float precoCompra;       /* quanto a Berê paga ao fornecedor                 */
    float margemLucro;       /* percentual de lucro, ex: 30.0 = 30%             */
    float precoVenda;        /* calculado: precoCompra * (1 + margem/100)        */
    int   estoqueMin;        /* ponto de alerta: quando estoque chega aqui, avisa*/
    int   estoque;           /* quantidade atual disponível na prateleira        */
} Produto;

/* ── Ficha do Item do Carrinho ─────────────────────────────── */
/* Usado temporariamente durante a venda, antes de gravar       */
typedef struct {
    int   codigo;            /* código do produto                                */
    char  nomeProduto[100];  /* cópia do nome (para exibir no carrinho)          */
    float precoVenda;        /* preço no momento da venda                        */
    int   quantidade;        /* quantas unidades foram adicionadas               */
} ItemCarrinho;

/* ── Ficha da Venda (cabeçalho) ────────────────────────────── */
/* Guarda o "cabeçalho" da venda: quem comprou, quando e total  */
typedef struct {
    int   numVenda;    /* número sequencial da venda (1, 2, 3...)                */
    int   codCliente;  /* código do cliente; 0 = venda sem identificação         */
    int   dia, mes, ano; /* data informada pelo operador                         */
    float totalVenda;  /* valor total após desconto                              */
} DadosVenda;

/* ── Ficha do Item de Venda ────────────────────────────────── */
/* Cada produto do carrinho vira um registro aqui após a venda  */
typedef struct {
    int   numVenda;        /* FK: liga este item à sua venda pelo número          */
    int   codProduto;      /* código do produto vendido                          */
    char  descricao[100];  /* cópia do nome no momento da venda (histórico)      */
    float precoVenda;      /* preço unitário gravado no momento da venda         */
    int   quantidade;      /* quantas unidades foram vendidas                    */
    float totalItem;       /* precoVenda * quantidade                            */
    char  pagamento;       /* 'a' = aberto (não pago ainda) | 'p' = pago        */
} DadosItem;

/* ── Ficha do Pagamento ────────────────────────────────────── */
/* Uma mesma venda pode ter até 2 registros (pagamento misto)   */
typedef struct {
    int   numVenda;    /* FK: liga o pagamento à venda                            */
    float valorPago;   /* valor desta parcela do pagamento                       */
    char  tipo[3];     /* "d" =dinheiro  "c"=cartão  "md"=misto/dinheiro
                          "mc"=misto/cartão    (até 2 chars + '\0')              */
} DadosPagamento;

/* ── Ficha do Usuário do Sistema ───────────────────────────── */
typedef struct {
    int  codUsuario;  /* código sequencial do usuário                            */
    char login[13];   /* 8 a 12 caracteres + '\0'                               */
    char senha[9];    /* 6 a 8 caracteres + '\0'                                */
    int  tipo;        /* 1 = Administrador  |  2 = Usuário comum                */
} DadosUsuario;

/* ── Ficha do Caixa do Dia ─────────────────────────────────── */
/* Existe apenas um caixa; é um registro único no arquivo       */
typedef struct {
    float vlAbertura;   /* dinheiro colocado no caixa ao abrir o dia            */
    float faturamento;  /* soma de todas as vendas pagas no dia                 */
    float vlPagoD;      /* total recebido em dinheiro puro                      */
    float vlPagoC;      /* total recebido em cartão puro                        */
    float vlPagoMd;     /* parte em dinheiro dos pagamentos mistos              */
    float vlPagoMc;     /* parte em cartão dos pagamentos mistos                */
    float ajuste;       /* diferença calculada no fechamento (sobra ou falta)   */
    int   aberto;       /* 1 = caixa aberto  |  0 = caixa fechado              */
} DadosCaixa;


/* ============================================================
   SEÇÃO 4 — NÓS DAS LISTAS ENCADEADAS
   ============================================================
   Uma lista encadeada é como uma corrente: cada "elo" (nó) guarda
   um dado e um ponteiro que aponta para o próximo elo.
   O último nó aponta para NULL, indicando o fim da lista.

   Exemplo visual:
   [nó1: venda1 | →] → [nó2: venda2 | →] → [nó3: venda3 | NULL]
                                                     ↑ fim da lista

   Por que usar lista e não array para vendas/itens/pagamentos?
   → Não sabemos quantos registros teremos. A lista cresce um nó
     por vez, sem precisar reservar espaço antecipadamente.
   ============================================================ */

/* Nó de Venda: guarda uma DadosVenda + ponteiro para o próximo nó */
typedef struct NodoVenda {
    DadosVenda data;          /* o dado (a ficha da venda)                       */
    struct NodoVenda *prox;   /* ponteiro para o próximo nó (ou NULL se último)  */
} NodoVenda;

/* Nó de Item: mesma lógica */
typedef struct NodoItem {
    DadosItem data;
    struct NodoItem *prox;
} NodoItem;

/* Nó de Pagamento */
typedef struct NodoPagamento {
    DadosPagamento data;
    struct NodoPagamento *prox;
} NodoPagamento;

/* Nó de Usuário */
typedef struct NodoUsuario {
    DadosUsuario data;
    struct NodoUsuario *prox;
} NodoUsuario;


/* ============================================================
   SEÇÃO 5 — VARIÁVEIS GLOBAIS
   ============================================================
   Variáveis globais são declaradas FORA de qualquer função.
   Isso significa que TODAS as funções do programa podem acessá-las
   e modificá-las diretamente, sem precisar passar como parâmetro.
   ============================================================ */

/* ── Arrays dinâmicos: clientes e produtos ─────────────────── */
Cliente  *clientes  = NULL;   /* ponteiro para o início do array de clientes     */
Produto  *produtos  = NULL;   /* ponteiro para o início do array de produtos     */
int qtdC = 0, capacidadeC = 0; /* qtdC = quantos existem; capacidadeC = espaço alocado */
int qtdP = 0, capacidadeP = 0; /* mesma lógica para produtos                    */

/* ── Array dinâmico do carrinho de compras ─────────────────── */
ItemCarrinho *carrinho   = NULL; /* lista de itens adicionados na venda atual    */
int   qtdCarrinho        = 0;    /* quantos itens há no carrinho agora           */
int   capacidadeCarrinho = 0;    /* espaço alocado atualmente                   */
float totalCarrinho      = 0.0;  /* soma dos valores de todos os itens           */

/* ── Cabeças das listas encadeadas ─────────────────────────── */
/* Cada ponteiro aponta para o PRIMEIRO nó da lista.
   NULL = lista vazia (nenhum registro ainda)                    */
NodoVenda     *listaVendas     = NULL;
NodoItem      *listaItens      = NULL;
NodoPagamento *listaPagamentos = NULL;
NodoUsuario   *listaUsuarios   = NULL;

/* ── Contadores das listas ──────────────────────────────────── */
int totalVendas     = 0;   /* quantas vendas foram realizadas                    */
int totalItens      = 0;   /* quantos itens de venda foram registrados           */
int totalPagamentos = 0;   /* quantos registros de pagamento existem             */
int totalUsuarios   = 0;   /* quantos usuários estão cadastrados                 */

/* ── Estado global do caixa e do usuário logado ────────────── */
DadosCaixa caixaDoDia;          /* único caixa do dia (registro global)          */
int usuarioLogadoTipo = 2;      /* tipo padrão ao iniciar: 2 = usuário comum     */


/* ============================================================
   SEÇÃO 6 — FUNÇÕES UTILITÁRIAS
   ============================================================
   Pequenas funções de uso geral, chamadas em vários módulos.
   ============================================================ */

/*
 * lerInt(msg)
 * ─────────────────────────────────────────────────────────────
 * Exibe a mensagem `msg`, lê um inteiro e VALIDA a entrada.
 * Se o usuário digitar texto (ex: "abc"), o scanf retorna 0
 * (falha). O while limpa o buffer e pede de novo — sem crash.
 *
 * Retorno: o inteiro validado
 * ─────────────────────────────────────────────────────────────
 */
int lerInt(const char *msg) {
    int valor;
    printf("%s", msg);                   /* exibe a pergunta                     */
    while (scanf("%d", &valor) != 1) {   /* tenta ler; se falhar (!=1)...        */
        printf("Entrada invalida, digite um numero inteiro: ");
        while (getchar() != '\n');       /* limpa tudo que ficou no buffer       */
    }
    while (getchar() != '\n');           /* consome o '\n' final do Enter        */
    return valor;
}

/*
 * lerFloat(msg)
 * ─────────────────────────────────────────────────────────────
 * Mesma lógica do lerInt, mas para números com casas decimais.
 * Retorno: o float validado
 * ─────────────────────────────────────────────────────────────
 */
float lerFloat(const char *msg) {
    float valor;
    printf("%s", msg);
    while (scanf("%f", &valor) != 1) {
        printf("Entrada invalida, digite um numero: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return valor;
}

/*
 * limparTela()
 * ─────────────────────────────────────────────────────────────
 * Chama o comando do sistema operacional para limpar o terminal.
 * #ifdef verifica em tempo de compilação qual SO está em uso:
 *   Windows → "cls"    |    Linux/Mac → "clear"
 * ─────────────────────────────────────────────────────────────
 */
void limparTela(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*
 * pausar()
 * ─────────────────────────────────────────────────────────────
 * Aguarda o usuário pressionar Enter antes de continuar.
 * O primeiro getchar() consome o '\n' do último scanf.
 * O segundo espera o Enter do usuário.
 * ─────────────────────────────────────────────────────────────
 */
void pausar(void) {
    printf("\nPressione Enter para continuar...");
    getchar();
}

/*
 * calcularPrecoVenda(precoCompra, margem)
 * ─────────────────────────────────────────────────────────────
 * Fórmula: precoVenda = precoCompra × (1 + margem ÷ 100)
 * Exemplo: custo R$10,00 com 30% → R$10 × 1,30 = R$13,00
 * ─────────────────────────────────────────────────────────────
 */
float calcularPrecoVenda(float precoCompra, float margem) {
    return precoCompra * (1.0f + margem / 100.0f);
}


/* ============================================================
   SEÇÃO 7 — INSERÇÃO NAS LISTAS ENCADEADAS
   ============================================================
   Todas seguem o mesmo padrão de INSERÇÃO NO INÍCIO:
     1. malloc() → aloca espaço para um novo nó na memória heap
     2. Copia o dado para o campo .data do novo nó
     3. O .prox do novo nó aponta para o antigo início da lista
     4. O ponteiro de início agora aponta para o novo nó

   Por que inserir no início?
   → É O(1): não precisa percorrer a lista inteira. Rápido!

   Exemplo antes: [nó1] → [nó2] → NULL
   Após inserir nó3 no início: [nó3] → [nó1] → [nó2] → NULL
   ============================================================ */

/* Insere uma venda no início da listaVendas */
void inserirVenda(DadosVenda v) {
    NodoVenda *novo = malloc(sizeof(NodoVenda)); /* aloca memória para o novo nó */
    if (!novo) { printf("Sem memoria!\n"); return; } /* verifica se malloc funcionou */
    novo->data = v;            /* copia a ficha da venda para o nó               */
    novo->prox = listaVendas;  /* o próximo do novo aponta para o antigo início  */
    listaVendas = novo;        /* atualiza: o início agora é o novo nó           */
}

/* Insere um item no início da listaItens — mesma lógica */
void inserirItem(DadosItem it) {
    NodoItem *novo = malloc(sizeof(NodoItem));
    if (!novo) { printf("Sem memoria!\n"); return; }
    novo->data = it;
    novo->prox = listaItens;
    listaItens = novo;
}

/* Insere um pagamento no início da listaPagamentos */
void inserirPagamento(DadosPagamento pg) {
    NodoPagamento *novo = malloc(sizeof(NodoPagamento));
    if (!novo) { printf("Sem memoria!\n"); return; }
    novo->data = pg;
    novo->prox = listaPagamentos;
    listaPagamentos = novo;
}

/* Insere um usuário no início da listaUsuarios */
void inserirUsuario(DadosUsuario u) {
    NodoUsuario *novo = malloc(sizeof(NodoUsuario));
    if (!novo) { printf("Sem memoria!\n"); return; }
    novo->data = u;
    novo->prox = listaUsuarios;
    listaUsuarios = novo;
}


/* ============================================================
   SEÇÃO 8 — PERSISTÊNCIA EM ARQUIVO BINÁRIO
   ============================================================
   Por que arquivo binário e não texto?
   → Binário grava os bytes exatos da struct em disco. É mais
     rápido, ocupa menos espaço e lê de volta com perfeição.

   Padrão de escrita:
     fopen(nome, "wb")   → abre para escrita binária (cria/sobrescreve)
     fwrite(&total, ...) → grava primeiro o contador de registros
     fwrite(&dado, ...)  → grava cada registro
     fclose(f)           → fecha o arquivo

   Padrão de leitura:
     fopen(nome, "rb")   → abre para leitura binária
     fread(&total, ...)  → lê o contador
     loop fread(...)     → lê cada registro e insere na lista/array
     fclose(f)           → fecha o arquivo

   Se o arquivo não existe (fopen retorna NULL), a função retorna
   sem erro — é a primeira execução, dados começam zerados.
   ============================================================ */

/* ── Clientes: array dinâmico → arquivo ────────────────────── */

void salvarClientes(void) {
    FILE *f = fopen(ARQ_CLIENTES, "wb");      /* abre para escrita binária       */
    if (!f) { printf("Erro ao salvar clientes!\n"); return; }
    fwrite(&qtdC, sizeof(int), 1, f);          /* grava o total de clientes       */
    fwrite(clientes, sizeof(Cliente), qtdC, f);/* grava todos de uma vez (array)  */
    fclose(f);
}

void carregarClientes(void) {
    FILE *f = fopen(ARQ_CLIENTES, "rb");
    if (!f) return;                            /* arquivo não existe: sem dados    */
    fread(&qtdC, sizeof(int), 1, f);           /* lê o total gravado              */
    capacidadeC = qtdC > 0 ? qtdC : 10;       /* aloca ao menos 10 posições      */
    clientes = malloc(capacidadeC * sizeof(Cliente)); /* aloca o array            */
    if (!clientes) { fclose(f); return; }
    fread(clientes, sizeof(Cliente), qtdC, f); /* lê todos os clientes de uma vez */
    fclose(f);
}

/* ── Produtos: mesma lógica ─────────────────────────────────── */

void salvarProdutos(void) {
    FILE *f = fopen(ARQ_PRODUTOS, "wb");
    if (!f) { printf("Erro ao salvar produtos!\n"); return; }
    fwrite(&qtdP, sizeof(int), 1, f);
    fwrite(produtos, sizeof(Produto), qtdP, f);
    fclose(f);
}

void carregarProdutos(void) {
    FILE *f = fopen(ARQ_PRODUTOS, "rb");
    if (!f) return;
    fread(&qtdP, sizeof(int), 1, f);
    capacidadeP = qtdP > 0 ? qtdP : 10;
    produtos = malloc(capacidadeP * sizeof(Produto));
    if (!produtos) { fclose(f); return; }
    fread(produtos, sizeof(Produto), qtdP, f);
    fclose(f);
}

/* ── Vendas: lista encadeada → arquivo ──────────────────────── */

void salvarVendas(void) {
    FILE *f = fopen(ARQ_VENDAS, "wb");
    if (!f) { printf("Erro ao salvar vendas!\n"); return; }
    fwrite(&totalVendas, sizeof(int), 1, f);   /* grava o contador                */
    NodoVenda *cur = listaVendas;              /* ponteiro auxiliar para percorrer */
    while (cur) {                              /* percorre nó a nó até NULL        */
        fwrite(&cur->data, sizeof(DadosVenda), 1, f); /* grava apenas o dado, não o ponteiro */
        cur = cur->prox;                       /* avança para o próximo nó        */
    }
    fclose(f);
}

void carregarVendas(void) {
    FILE *f = fopen(ARQ_VENDAS, "rb");
    if (!f) return;
    int total;
    fread(&total, sizeof(int), 1, f);          /* lê o contador                   */
    for (int i = 0; i < total; i++) {          /* repete para cada registro       */
        DadosVenda v;
        fread(&v, sizeof(DadosVenda), 1, f);   /* lê um registro para variável temp */
        inserirVenda(v);                       /* insere na lista encadeada       */
        totalVendas++;                         /* incrementa o contador global    */
    }
    fclose(f);
}

/* ── Itens: mesma lógica que vendas ─────────────────────────── */

void salvarItens(void) {
    FILE *f = fopen(ARQ_ITENS, "wb");
    if (!f) { printf("Erro ao salvar itens!\n"); return; }
    fwrite(&totalItens, sizeof(int), 1, f);
    NodoItem *cur = listaItens;
    while (cur) { fwrite(&cur->data, sizeof(DadosItem), 1, f); cur = cur->prox; }
    fclose(f);
}

void carregarItens(void) {
    FILE *f = fopen(ARQ_ITENS, "rb");
    if (!f) return;
    int total; fread(&total, sizeof(int), 1, f);
    for (int i = 0; i < total; i++) {
        DadosItem it; fread(&it, sizeof(DadosItem), 1, f);
        inserirItem(it); totalItens++;
    }
    fclose(f);
}

/* ── Pagamentos ──────────────────────────────────────────────── */

void salvarPagamentos(void) {
    FILE *f = fopen(ARQ_PAGAMENTOS, "wb");
    if (!f) { printf("Erro ao salvar pagamentos!\n"); return; }
    fwrite(&totalPagamentos, sizeof(int), 1, f);
    NodoPagamento *cur = listaPagamentos;
    while (cur) { fwrite(&cur->data, sizeof(DadosPagamento), 1, f); cur = cur->prox; }
    fclose(f);
}

void carregarPagamentos(void) {
    FILE *f = fopen(ARQ_PAGAMENTOS, "rb");
    if (!f) return;
    int total; fread(&total, sizeof(int), 1, f);
    for (int i = 0; i < total; i++) {
        DadosPagamento pg; fread(&pg, sizeof(DadosPagamento), 1, f);
        inserirPagamento(pg); totalPagamentos++;
    }
    fclose(f);
}

/* ── Usuários ────────────────────────────────────────────────── */

void salvarUsuarios(void) {
    FILE *f = fopen(ARQ_USUARIOS, "wb");
    if (!f) { printf("Erro ao salvar usuarios!\n"); return; }
    fwrite(&totalUsuarios, sizeof(int), 1, f);
    NodoUsuario *cur = listaUsuarios;
    while (cur) { fwrite(&cur->data, sizeof(DadosUsuario), 1, f); cur = cur->prox; }
    fclose(f);
}

void carregarUsuarios(void) {
    FILE *f = fopen(ARQ_USUARIOS, "rb");
    if (!f) return;
    int total; fread(&total, sizeof(int), 1, f);
    for (int i = 0; i < total; i++) {
        DadosUsuario u; fread(&u, sizeof(DadosUsuario), 1, f);
        inserirUsuario(u); totalUsuarios++;
    }
    fclose(f);
}

/* ── Caixa: um único registro ───────────────────────────────── */

void salvarCaixa(void) {
    FILE *f = fopen(ARQ_CAIXA, "wb");
    if (!f) { printf("Erro ao salvar caixa!\n"); return; }
    fwrite(&caixaDoDia, sizeof(DadosCaixa), 1, f); /* grava a struct inteira de uma vez */
    fclose(f);
}

void carregarCaixa(void) {
    FILE *f = fopen(ARQ_CAIXA, "rb");
    if (!f) {
        /* arquivo não existe: primeira vez que o programa roda.
           Inicializa tudo com zero usando compound literal (DadosCaixa){0} */
        caixaDoDia = (DadosCaixa){0};
        return;
    }
    fread(&caixaDoDia, sizeof(DadosCaixa), 1, f);
    fclose(f);
}


/* ============================================================
   SEÇÃO 9 — VALIDAÇÃO DE ADMINISTRADOR
   ============================================================
   Certas operações (abertura de caixa, fechamento, sangria,
   cadastrar usuário) só podem ser feitas por um administrador.

   Fluxo:
   1. Se o usuário já logou como admin (tipo 1), libera direto.
   2. Caso contrário, pede login e senha.
   3. Percorre a lista de usuários procurando um admin com
      login e senha correspondentes.
   4. Retorna 1 (verdadeiro) se autorizado, 0 se negado.
   ============================================================ */

int validarAdmin(void) {
    char login[13], senha[9];

    /* se já está logado como admin, não precisa pedir credenciais */
    if (usuarioLogadoTipo == 1) return 1;

    printf("\n--- Acesso restrito a Administrador ---\n");
    printf("Login : "); scanf("%12s", login); while (getchar() != '\n');
    printf("Senha : "); scanf("%8s",  senha); while (getchar() != '\n');

    /* busca linear na lista de usuários */
    NodoUsuario *cur = listaUsuarios;
    while (cur) {
        if (cur->data.tipo == 1                          /* é administrador?      */
            && strcmp(cur->data.login, login) == 0       /* login confere?        */
            && strcmp(cur->data.senha, senha) == 0) {    /* senha confere?        */
            printf("Acesso autorizado!\n");
            return 1;   /* credenciais válidas                                    */
        }
        cur = cur->prox;   /* vai para o próximo nó                              */
    }

    printf("Login ou senha invalidos. Acesso negado.\n");
    return 0;   /* nenhum admin encontrado com essas credenciais                  */
}


/* ============================================================
   SEÇÃO 10 — MÓDULO CADASTROS
   ============================================================ */

/*
 * cadastrarUsuario()
 * ─────────────────────────────────────────────────────────────
 * Coleta login, senha e tipo de um novo usuário.
 * Valida tamanho mínimo: login >= 8 chars, senha >= 6 chars.
 * O loop do-while repete até a condição ser satisfeita.
 * ─────────────────────────────────────────────────────────────
 */
void cadastrarUsuario(void) {
    DadosUsuario u;   /* variável local que vai receber os dados digitados        */

    printf("\n=== CADASTRO DE USUARIO ===\n");

    /* loop: repete enquanto o login tiver menos de 8 caracteres */
    do {
        printf("Login (8 a 12 caracteres): ");
        scanf("%12s", u.login);          /* lê no máximo 12 chars + '\0'         */
        while (getchar() != '\n');       /* limpa buffer                          */
        if (strlen(u.login) < 8)
            printf("Login muito curto!\n");
    } while (strlen(u.login) < 8);

    /* loop: repete enquanto a senha tiver menos de 6 caracteres */
    do {
        printf("Senha (6 a 8 caracteres): ");
        scanf("%8s", u.senha);
        while (getchar() != '\n');
        if (strlen(u.senha) < 6)
            printf("Senha muito curta!\n");
    } while (strlen(u.senha) < 6);

    /* loop: aceita apenas 1 (admin) ou 2 (comum) */
    do {
        u.tipo = lerInt("Tipo (1-Administrador / 2-Usuario): ");
    } while (u.tipo != 1 && u.tipo != 2);

    u.codUsuario = totalUsuarios + 1;   /* gera código sequencial                */
    inserirUsuario(u);                  /* insere na lista encadeada             */
    totalUsuarios++;
    salvarUsuarios();                   /* persiste no arquivo imediatamente      */
    printf("Usuario %d cadastrado com sucesso!\n", u.codUsuario);
}

/*
 * cadastrarCliente()
 * ─────────────────────────────────────────────────────────────
 * Usa array dinâmico com realloc. Se o array estiver cheio,
 * dobra a capacidade antes de inserir o novo cliente.
 *
 * fgets + strcspn: lê a linha inteira (com espaços) e remove
 * o '\n' do final que fgets inclui automaticamente.
 *
 * c = &clientes[qtdC] → pega o endereço da próxima posição
 * livre do array e usa como ponteiro para preencher.
 * ─────────────────────────────────────────────────────────────
 */
void cadastrarCliente(void) {
    /* verifica se há espaço; se não, dobra a capacidade */
    if (qtdC >= capacidadeC) {
        capacidadeC = (capacidadeC == 0) ? 10 : capacidadeC * 2;
        /* realloc: realoca o array com novo tamanho, mantendo os dados antigos */
        clientes = realloc(clientes, capacidadeC * sizeof(Cliente));
        if (!clientes) { printf("Erro de memoria!\n"); exit(1); }
    }

    Cliente *c = &clientes[qtdC];  /* ponteiro para a posição a ser preenchida   */
    c->codC = qtdC + 1;            /* código automático sequencial                */

    printf("\n=== CADASTRO DE CLIENTE ===\n");
    /* fgets lê a linha inteira; strcspn encontra o '\n' e o substitui por '\0' */
    printf("Nome completo   : "); fgets(c->nome,       sizeof(c->nome),       stdin); c->nome[strcspn(c->nome,"\n")]             = '\0';
    printf("Nome social     : "); fgets(c->nomeSocial, sizeof(c->nomeSocial), stdin); c->nomeSocial[strcspn(c->nomeSocial,"\n")] = '\0';
    printf("CPF             : "); fgets(c->cpf,        sizeof(c->cpf),        stdin); c->cpf[strcspn(c->cpf,"\n")]               = '\0';
    printf("Rua e numero    : "); fgets(c->rua,        sizeof(c->rua),        stdin); c->rua[strcspn(c->rua,"\n")]               = '\0';
    printf("Bairro          : "); fgets(c->bairro,     sizeof(c->bairro),     stdin); c->bairro[strcspn(c->bairro,"\n")]         = '\0';
    printf("Celular/WhatsApp: "); fgets(c->numero,     sizeof(c->numero),     stdin); c->numero[strcspn(c->numero,"\n")]         = '\0';

    qtdC++;            /* incrementa o contador de clientes                       */
    salvarClientes();  /* salva imediatamente para não perder os dados            */
    printf("Cliente %d cadastrado com sucesso!\n", c->codC);
}

/*
 * cadastrarProduto()
 * ─────────────────────────────────────────────────────────────
 * Mesma lógica de crescimento dinâmico do cliente.
 * O código do produto começa em 1000 (1000, 1001, 1002...).
 * O preço de venda é calculado automaticamente pela função
 * calcularPrecoVenda e exibido antes de confirmar.
 * ─────────────────────────────────────────────────────────────
 */
void cadastrarProduto(void) {
    if (qtdP >= capacidadeP) {
        capacidadeP = (capacidadeP == 0) ? 10 : capacidadeP * 2;
        produtos = realloc(produtos, capacidadeP * sizeof(Produto));
        if (!produtos) { printf("Erro de memoria!\n"); exit(1); }
    }

    Produto *p = &produtos[qtdP];
    p->codigo = 1000 + qtdP;   /* primeiro produto = 1000, segundo = 1001...     */

    printf("\n=== CADASTRO DE PRODUTO ===\n");
    printf("Descricao       : ");
    fgets(p->nomeProduto, sizeof(p->nomeProduto), stdin);
    p->nomeProduto[strcspn(p->nomeProduto, "\n")] = '\0';

    printf("Categoria:\n  1 - Alimentos\n  2 - Material de Limpeza\n  3 - Panificadora\n");
    /* loop garante que o operador só passe com 1, 2 ou 3 */
    do { p->categoria = lerInt("Opcao: "); } while (p->categoria < 1 || p->categoria > 3);

    p->precoCompra = lerFloat("Preco de custo  : R$ ");
    p->margemLucro = lerFloat("Margem de lucro : %% ");
    p->precoVenda  = calcularPrecoVenda(p->precoCompra, p->margemLucro);
    printf("Preco de venda calculado: R$ %.2f\n", p->precoVenda);

    p->estoque    = lerInt("Qtd em estoque  : ");
    p->estoqueMin = lerInt("Estoque minimo  : ");

    qtdP++;
    salvarProdutos();
    printf("Produto %d cadastrado com sucesso!\n", p->codigo);
}

/* Exibe tabela de clientes formatada com %-Xs (alinhamento à esquerda) */
void listarClientes(void) {
    if (qtdC == 0) { printf("Nenhum cliente cadastrado.\n"); return; }
    printf("\n%-5s %-30s %-15s %-20s\n", "COD", "NOME", "CPF", "CELULAR");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < qtdC; i++)
        printf("%-5d %-30s %-15s %-20s\n",
               clientes[i].codC, clientes[i].nome,
               clientes[i].cpf,  clientes[i].numero);
}

/* Exibe tabela de produtos com categoria convertida de número para texto */
void listarProdutos(void) {
    if (qtdP == 0) { printf("Nenhum produto cadastrado.\n"); return; }
    printf("\n%-6s %-30s %-15s %10s %6s %6s\n",
           "COD", "DESCRICAO", "CATEGORIA", "VL.VENDA", "ESTQ", "MIN");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < qtdP; i++) {
        /* operador ternário encadeado para converter int → texto da categoria   */
        char *cat = (produtos[i].categoria == 1) ? "Alimentos" :
                    (produtos[i].categoria == 2) ? "Limpeza"   : "Panificadora";
        printf("%-6d %-30s %-15s R$%8.2f %6d %6d\n",
               produtos[i].codigo, produtos[i].nomeProduto, cat,
               produtos[i].precoVenda, produtos[i].estoque, produtos[i].estoqueMin);
    }
}


/* ============================================================
   SEÇÃO 11 — MÓDULO VENDAS
   ============================================================ */

/*
 * adicionarAoCarrinho(idProduto, qtd)
 * ─────────────────────────────────────────────────────────────
 * Adiciona qtd unidades do produto[idProduto] ao carrinho.
 * Se o produto já está no carrinho, apenas soma a quantidade.
 * Se é novo, expande o array do carrinho (realloc) e insere.
 *
 * idProduto = índice no array de produtos (não é o código 1000+)
 * ─────────────────────────────────────────────────────────────
 */
void adicionarAoCarrinho(int idProduto, int qtd) {
    /* primeiro verifica se o produto já está no carrinho */
    for (int i = 0; i < qtdCarrinho; i++) {
        if (carrinho[i].codigo == produtos[idProduto].codigo) {
            carrinho[i].quantidade += qtd;           /* só soma a quantidade     */
            totalCarrinho += produtos[idProduto].precoVenda * qtd;
            return;   /* sai da função: não precisa inserir um novo item         */
        }
    }

    /* produto novo: expande o array se necessário */
    if (qtdCarrinho >= capacidadeCarrinho) {
        capacidadeCarrinho = (capacidadeCarrinho == 0) ? 10 : capacidadeCarrinho * 2;
        carrinho = realloc(carrinho, capacidadeCarrinho * sizeof(ItemCarrinho));
        if (!carrinho) { printf("Erro de memoria no carrinho!\n"); exit(1); }
    }

    /* preenche a nova posição do carrinho */
    carrinho[qtdCarrinho].codigo     = produtos[idProduto].codigo;
    carrinho[qtdCarrinho].precoVenda = produtos[idProduto].precoVenda;
    carrinho[qtdCarrinho].quantidade = qtd;
    strcpy(carrinho[qtdCarrinho].nomeProduto, produtos[idProduto].nomeProduto);
    qtdCarrinho++;
    totalCarrinho += produtos[idProduto].precoVenda * qtd;
}

/*
 * registrarPagamento(numVenda, valorTotal)
 * ─────────────────────────────────────────────────────────────
 * Recebe o número da venda e o valor total a pagar.
 * O operador escolhe dinheiro ou cartão.
 *
 * DINHEIRO:
 *   → operador informa % de desconto (0 = sem desconto)
 *   → calcula valorComDesconto
 *   → se valor recebido >= valorComDesconto: troco e registra tipo "d"
 *   → se valor recebido < valorComDesconto: pagamento misto
 *     (parte em dinheiro tipo "md" + resto em cartão tipo "mc")
 *
 * CARTÃO:
 *   → loop de confirmação (máquina ok / erro tentar de novo)
 *   → se valor no cartão >= total: tipo "c" (cartão integral)
 *   → se valor no cartão < total: misto
 *     (parte cartão tipo "mc" + resto dinheiro tipo "md")
 *
 * No final: marca os itens da venda como 'p' (pago) e
 * atualiza os acumuladores do caixa.
 * ─────────────────────────────────────────────────────────────
 */
void registrarPagamento(int numVenda, float valorTotal) {
    DadosPagamento pg;       /* struct do pagamento a ser inserida               */
    int   forma;             /* 1=dinheiro  2=cartão                             */
    float valorInformado;    /* valor que o cliente entrega                      */
    float resto;             /* diferença a ser paga na outra forma              */
    float desconto = 0.0;    /* percentual de desconto informado pelo operador   */
    float valorComDesconto;  /* valorTotal após aplicar o desconto               */

    printf("\nTotal a pagar: R$ %.2f\n", valorTotal);
    printf("Forma de pagamento:\n  1 - Dinheiro\n  2 - Cartao\n");
    forma = lerInt("Opcao: ");

    pg.numVenda = numVenda;   /* vincula este pagamento à venda pelo número       */

    if (forma == 1) {
        /* ── PAGAMENTO EM DINHEIRO ──────────────────────────── */

        desconto = lerFloat("Desconto (0 para nenhum, ou % concedido): ");
        if (desconto < 0.0f) desconto = 0.0f;   /* não aceita desconto negativo  */

        /* fórmula do desconto: valorTotal - (valorTotal × desconto ÷ 100) */
        valorComDesconto = valorTotal - (valorTotal * desconto / 100.0f);

        if (desconto > 0.0f)
            printf("Desconto de %.2f%% aplicado. Valor final: R$ %.2f\n",
                   desconto, valorComDesconto);

        valorInformado = lerFloat("Valor recebido  : R$ ");

        if (valorInformado < valorComDesconto) {
            /* dinheiro insuficiente: propõe pagar o resto no cartão */
            float resto2 = valorComDesconto - valorInformado;
            printf("Valor insuficiente! Restam R$ %.2f. Pagar restante no cartao (s/n)? ", resto2);
            char resp;
            scanf(" %c", &resp); while (getchar() != '\n');

            if (resp != 's' && resp != 'S') {
                printf("Pagamento cancelado.\n");
                return;
            }

            /* registra a parte em dinheiro */
            pg.valorPago = valorInformado;
            strcpy(pg.tipo, "md");              /* md = misto-dinheiro             */
            inserirPagamento(pg); totalPagamentos++;
            caixaDoDia.vlPagoMd += valorInformado;

            /* registra a parte em cartão */
            pg.valorPago = resto2;
            strcpy(pg.tipo, "mc");              /* mc = misto-cartão               */
            inserirPagamento(pg); totalPagamentos++;
            caixaDoDia.vlPagoMc += resto2;

            printf("Pagamento misto: R$ %.2f dinheiro + R$ %.2f cartao.\n",
                   valorInformado, resto2);

        } else {
            /* dinheiro suficiente: pagamento integral */
            printf("Troco: R$ %.2f\n", valorInformado - valorComDesconto);
            pg.valorPago = valorComDesconto;
            strcpy(pg.tipo, "d");               /* d = dinheiro puro               */
            inserirPagamento(pg); totalPagamentos++;
            caixaDoDia.vlPagoD += valorComDesconto;
        }

    } else {
        /* ── PAGAMENTO NO CARTÃO ─────────────────────────────── */

        /* loop de tentativa: fica repetindo até confirmar OK (opc == 1) */
        int ok;
        do {
            printf("O pagamento deu certo?\n  1 - Efetuado!\n  2 - Erro, tentar novamente\n");
            ok = lerInt("Opcao: ");
            if (ok == 2) printf("Tentando novamente...\n");
        } while (ok != 1);

        valorInformado = lerFloat("Valor no cartao : R$ ");

        if (valorInformado >= valorTotal) {
            /* cartão cobre tudo: tipo "c" */
            pg.valorPago = valorTotal;
            strcpy(pg.tipo, "c");
            inserirPagamento(pg); totalPagamentos++;
            caixaDoDia.vlPagoC += valorTotal;
            valorComDesconto = valorTotal;      /* sem desconto no cartão          */

        } else {
            /* cartão parcial: resto em dinheiro (misto ao contrário) */
            resto = valorTotal - valorInformado;
            printf("Restante de R$ %.2f sera em dinheiro.\n", resto);

            pg.valorPago = valorInformado;
            strcpy(pg.tipo, "mc");
            inserirPagamento(pg); totalPagamentos++;
            caixaDoDia.vlPagoMc += valorInformado;

            pg.valorPago = resto;
            strcpy(pg.tipo, "md");
            inserirPagamento(pg); totalPagamentos++;
            caixaDoDia.vlPagoMd += resto;
            valorComDesconto = valorTotal;
        }
    }

    /* marca todos os itens desta venda como pagos ('p') */
    NodoItem *cur = listaItens;
    while (cur) {
        if (cur->data.numVenda == numVenda)
            cur->data.pagamento = 'p';    /* muda de 'a' (aberto) para 'p' (pago)*/
        cur = cur->prox;
    }

    /* atualiza o faturamento do caixa com o valor recebido */
    caixaDoDia.faturamento += valorComDesconto;

    /* salva tudo no disco */
    salvarCaixa();
    salvarPagamentos();
    salvarItens();

    printf("Pagamento registrado! Saldo em caixa: R$ %.2f\n",
           caixaDoDia.vlAbertura + caixaDoDia.faturamento);
}

/*
 * novaVenda()
 * ─────────────────────────────────────────────────────────────
 * Fluxo completo de uma venda:
 *   1. Verifica se o caixa está aberto e se há produtos
 *   2. Coleta cliente e data
 *   3. Zera o carrinho (nova venda = carrinho limpo)
 *   4. Loop: exibe produtos → operador informa código e qtd
 *      → valida estoque → adiciona ao carrinho
 *      → repete até digitar código 0
 *   5. Grava a venda (DadosVenda) na lista encadeada
 *   6. Grava cada item (DadosItem) na lista de itens
 *   7. Chama registrarPagamento() automaticamente
 *   8. Limpa o carrinho ao final
 * ─────────────────────────────────────────────────────────────
 */
void novaVenda(void) {
    if (!caixaDoDia.aberto) {
        printf("Caixa fechado! Abra o caixa antes de realizar vendas.\n");
        return;
    }
    if (qtdP == 0) {
        printf("Nenhum produto cadastrado!\n");
        return;
    }

    printf("\n=== NOVA VENDA ===\n");
    int codCliente = lerInt("Codigo do cliente (0 = sem cliente): ");
    int dia        = lerInt("Dia : ");
    int mes        = lerInt("Mes : ");
    int ano        = lerInt("Ano : ");

    qtdCarrinho   = 0;      /* reseta o carrinho para esta venda                 */
    totalCarrinho = 0.0;

    int continuar = 1;
    while (continuar) {
        /* exibe catálogo de produtos */
        printf("\n%-6s %-30s %-15s %10s %6s\n",
               "COD", "DESCRICAO", "CATEGORIA", "VL.VENDA", "ESTQ");
        printf("------------------------------------------------------------------------\n");
        for (int i = 0; i < qtdP; i++) {
            char *cat = (produtos[i].categoria == 1) ? "Alimentos" :
                        (produtos[i].categoria == 2) ? "Limpeza"   : "Panificadora";
            printf("%-6d %-30s %-15s R$%8.2f %6d\n",
                   produtos[i].codigo, produtos[i].nomeProduto, cat,
                   produtos[i].precoVenda, produtos[i].estoque);
        }

        /* exibe o carrinho atual (se já houver itens) */
        if (qtdCarrinho > 0) {
            printf("\n---- Carrinho Atual ----\n");
            printf("%-6s %-30s %10s %6s %10s\n",
                   "COD", "PRODUTO", "PR.UNIT", "QTD", "SUBTOTAL");
            for (int i = 0; i < qtdCarrinho; i++) {
                float sub = carrinho[i].precoVenda * carrinho[i].quantidade;
                printf("%-6d %-30s R$%8.2f %6d R$%8.2f\n",
                       carrinho[i].codigo, carrinho[i].nomeProduto,
                       carrinho[i].precoVenda, carrinho[i].quantidade, sub);
            }
            printf("TOTAL: R$ %.2f\n", totalCarrinho);
        }

        int cod = lerInt("\nCodigo do produto (0 para encerrar): ");
        if (cod == 0) { continuar = 0; break; }   /* 0 = finalizar o carrinho    */

        int qtd = lerInt("Quantidade: ");
        if (qtd <= 0) { printf("Quantidade invalida!\n"); continue; }

        /* busca linear: percorre o array de produtos até encontrar o código */
        int idx = -1;
        for (int i = 0; i < qtdP; i++) {
            if (produtos[i].codigo == cod) { idx = i; break; }
        }

        if (idx == -1) {
            printf("Produto nao encontrado!\n");
        } else if (produtos[idx].estoque == 0) {
            /* REGRA: não pode vender produto com estoque zerado */
            printf("Estoque zerado! Venda nao permitida.\n");
        } else if (produtos[idx].estoque < qtd) {
            /* REGRA: quantidade pedida maior que o estoque disponível */
            printf("Estoque insuficiente! Disponivel: %d\n", produtos[idx].estoque);
        } else {
            /* tudo ok: desconta do estoque e adiciona ao carrinho */
            produtos[idx].estoque -= qtd;
            salvarProdutos();                 /* salva estoque atualizado          */
            adicionarAoCarrinho(idx, qtd);
            printf("'%s' adicionado! Subtotal: R$ %.2f\n",
                   produtos[idx].nomeProduto, produtos[idx].precoVenda * qtd);

            /* REGRA: alerta se atingiu o estoque mínimo */
            if (produtos[idx].estoque <= produtos[idx].estoqueMin)
                printf("ATENCAO: estoque minimo atingido para '%s' (%d restantes)!\n",
                       produtos[idx].nomeProduto, produtos[idx].estoque);
        }
    }

    if (qtdCarrinho == 0) {
        printf("Nenhum item no carrinho. Venda cancelada.\n");
        return;
    }

    /* monta a ficha da venda e insere na lista */
    DadosVenda v;
    v.numVenda   = totalVendas + 1;   /* número sequencial                       */
    v.codCliente = codCliente;
    v.dia = dia; v.mes = mes; v.ano = ano;
    v.totalVenda = totalCarrinho;
    inserirVenda(v); totalVendas++;

    /* converte cada item do carrinho em um DadosItem e insere na lista */
    for (int i = 0; i < qtdCarrinho; i++) {
        DadosItem it;
        it.numVenda   = v.numVenda;
        it.codProduto = carrinho[i].codigo;
        strcpy(it.descricao, carrinho[i].nomeProduto);  /* cópia histórica        */
        it.precoVenda = carrinho[i].precoVenda;
        it.quantidade = carrinho[i].quantidade;
        it.totalItem  = carrinho[i].precoVenda * carrinho[i].quantidade;
        it.pagamento  = 'a';     /* 'a' = aberto: ainda não foi pago              */
        inserirItem(it); totalItens++;
    }

    salvarVendas();
    salvarItens();

    registrarPagamento(v.numVenda, totalCarrinho);  /* pagamento imediato         */

    /* limpa o carrinho para a próxima venda */
    qtdCarrinho   = 0;
    totalCarrinho = 0.0;
}

/*
 * pagarVendasAbertas()
 * ─────────────────────────────────────────────────────────────
 * Lista todos os itens com pagamento == 'a' (aberto).
 * Útil quando o operador esqueceu de registrar o pagamento
 * e voltou ao menu principal antes de concluir.
 * ─────────────────────────────────────────────────────────────
 */
void pagarVendasAbertas(void) {
    printf("\n=== VENDAS EM ABERTO ===\n");
    int encontrou = 0;

    NodoItem *cur = listaItens;
    while (cur) {
        if (cur->data.pagamento == 'a') {   /* 'a' = aberto (ainda não pago)     */
            printf("Venda %d | %-30s | R$ %.2f\n",
                   cur->data.numVenda, cur->data.descricao, cur->data.totalItem);
            encontrou = 1;
        }
        cur = cur->prox;
    }

    if (!encontrou) { printf("Nenhuma venda em aberto.\n"); return; }

    int numVenda = lerInt("Numero da venda para pagar: ");

    /* busca o total da venda na lista de vendas */
    float total = 0;
    NodoVenda *cv = listaVendas;
    while (cv) {
        if (cv->data.numVenda == numVenda) { total = cv->data.totalVenda; break; }
        cv = cv->prox;
    }
    if (total == 0) { printf("Venda nao encontrada!\n"); return; }

    registrarPagamento(numVenda, total);
}

/*
 * sangria()
 * ─────────────────────────────────────────────────────────────
 * Retirada de dinheiro físico do caixa.
 * REGRAS (requisito):
 *   - Só administrador pode fazer
 *   - O caixa deve estar aberto
 *   - Deve sempre sobrar ao menos R$50,00 como fundo de troco
 *   - A retirada é deduzida do vlAbertura (saldo de abertura)
 *
 * saldoDinheiro = abertura + pagamentos em dinheiro puro + misto/dinheiro
 * ─────────────────────────────────────────────────────────────
 */
void sangria(void) {
    if (!validarAdmin()) return;
    if (!caixaDoDia.aberto) { printf("Caixa fechado!\n"); return; }

    float saldoDinheiro = caixaDoDia.vlAbertura
                        + caixaDoDia.vlPagoD
                        + caixaDoDia.vlPagoMd;

    printf("\n=== RETIRADA DE CAIXA (SANGRIA) ===\n");
    printf("Saldo em dinheiro no caixa: R$ %.2f\n", saldoDinheiro);
    printf("(Minimo obrigatorio de R$ 50,00 para troco deve permanecer)\n");

    float valor = lerFloat("Valor a retirar: R$ ");

    if (valor <= 0) {
        printf("Valor invalido!\n");
    } else if (valor >= (saldoDinheiro - 50.0f)) {
        /* REGRA: não pode retirar se sobrar menos de R$50 */
        printf("ATENCAO: Nao e permitido retirar este valor.\n");
        printf("Deve sobrar ao menos R$ 50,00 para troco.\n");
        /* exibe o máximo permitido; se saldo <= 50, mostra R$0,00 */
        printf("Maximo permitido para retirada: R$ %.2f\n",
               (saldoDinheiro - 50.0f) > 0 ? (saldoDinheiro - 50.0f) : 0.0f);
    } else {
        caixaDoDia.vlAbertura -= valor;   /* deduz do saldo de abertura           */
        salvarCaixa();
        printf("Sangria de R$ %.2f realizada com sucesso!\n", valor);
        printf("Saldo restante em dinheiro: R$ %.2f\n", saldoDinheiro - valor);
    }
}


/* ============================================================
   SEÇÃO 12 — MÓDULO CAIXA
   ============================================================ */

/*
 * abrirCaixa()
 * ─────────────────────────────────────────────────────────────
 * REGRAS:
 *   - Só administrador pode abrir
 *   - Não pode abrir se já estiver aberto
 *   - Valor de abertura >= 0 (pode ser R$0,00)
 * Inicializa todos os acumuladores com zero e salva.
 * ─────────────────────────────────────────────────────────────
 */
void abrirCaixa(void) {
    if (!validarAdmin()) return;
    if (caixaDoDia.aberto) {
        printf("Caixa ja esta aberto! Saldo: R$ %.2f\n",
               caixaDoDia.vlAbertura + caixaDoDia.faturamento);
        return;
    }

    float troco;
    do {
        troco = lerFloat("Informe o troco inicial: R$ ");
        if (troco < 0) printf("Valor invalido!\n");
    } while (troco < 0);

    /* (DadosCaixa){0} inicializa todos os campos da struct com zero */
    caixaDoDia = (DadosCaixa){0};
    caixaDoDia.vlAbertura = troco;
    caixaDoDia.aberto     = 1;      /* marca como aberto                         */
    salvarCaixa();
    printf("Caixa aberto com R$ %.2f.\n", troco);
}

/*
 * fecharCaixa()
 * ─────────────────────────────────────────────────────────────
 * REGRAS:
 *   - Só administrador pode fechar
 *   - Não pode fechar se houver carrinho em aberto
 *   - Verifica divergência: faturamento - pagamentos deve ser 0
 *     Se resultado != 0: exibe sobra/falta e BLOQUEIA o fechamento
 *     Se resultado == 0: fecha e salva
 *
 * Fórmula da divergência:
 *   resultado = faturamento - vlPagoD - vlPagoC - vlPagoMd - vlPagoMc
 * (O vlAbertura não entra na fórmula pois não é receita, é fundo de caixa)
 * ─────────────────────────────────────────────────────────────
 */
void fecharCaixa(void) {
    if (!validarAdmin()) return;
    if (!caixaDoDia.aberto) { printf("Caixa ja esta fechado!\n"); return; }

    if (qtdCarrinho > 0) {
        printf("Ha itens no carrinho! Finalize o pagamento antes de fechar.\n");
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

    /* calcula a diferença: se zero, tudo bate; se não, há divergência */
    float resultado = caixaDoDia.faturamento
                    - caixaDoDia.vlPagoD
                    - caixaDoDia.vlPagoC
                    - caixaDoDia.vlPagoMd
                    - caixaDoDia.vlPagoMc;

    caixaDoDia.ajuste = resultado;   /* guarda para referência futura             */

    if (resultado != 0.0f) {
        printf("\nATENCAO: Divergencia de valores.\n");
        if (resultado > 0)
            printf("Sobra de R$ %.2f.\n",  resultado);   /* entrou mais do que saiu */
        else
            printf("Falta de R$ %.2f.\n", -resultado);   /* -resultado para positivo */
        printf("Ajuste necessario antes de fechar.\n");
    } else {
        caixaDoDia.aberto = 0;   /* marca como fechado                           */
        salvarCaixa();
        printf("\nCaixa fechado com sucesso!\n");
    }
}


/* ============================================================
   SEÇÃO 13 — MÓDULO RELATÓRIOS (7 relatórios)
   ============================================================
   Todos os relatórios que ordenam dados usam BUBBLE SORT.

   Como funciona o Bubble Sort:
   → Percorre o array comparando cada elemento com o seguinte.
   → Se estiver fora de ordem, troca os dois.
   → Repete passagens até nenhuma troca ocorrer.
   → A variável `trocou` controla se houve troca na passagem atual.
   → Complexidade O(n²) — simples e suficiente para poucos dados.

   IMPORTANTE: os relatórios que ordenam fazem uma CÓPIA do array
   (malloc + memcpy) para não alterar a ordem original dos dados.
   ============================================================ */

/* ── 5.1.1 Clientes em ordem alfabética ─────────────────────── */
void relClientesAlfabetica(void) {
    if (qtdC == 0) { printf("Nenhum cliente cadastrado.\n"); return; }

    /* cria cópia temporária para ordenar sem mexer no original */
    Cliente *tmp = malloc(qtdC * sizeof(Cliente));
    memcpy(tmp, clientes, qtdC * sizeof(Cliente));   /* copia bloco de memória   */

    int trocou;
    do {
        trocou = 0;
        for (int i = 0; i < qtdC - 1; i++) {
            /* strcmp: > 0 significa que tmp[i] vem DEPOIS de tmp[i+1] na ordem */
            if (strcmp(tmp[i].nome, tmp[i+1].nome) > 0) {
                Cliente aux = tmp[i];        /* guarda temporariamente           */
                tmp[i]      = tmp[i+1];      /* troca os elementos               */
                tmp[i+1]    = aux;
                trocou = 1;
            }
        }
    } while (trocou);   /* repete enquanto houve pelo menos uma troca            */

    printf("\n=== CLIENTES - ORDEM ALFABETICA ===\n");
    printf("%-5s %-30s %-15s %-20s\n", "COD", "NOME", "CPF", "CELULAR");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < qtdC; i++)
        printf("%-5d %-30s %-15s %-20s\n",
               tmp[i].codC, tmp[i].nome, tmp[i].cpf, tmp[i].numero);

    free(tmp);   /* libera a memória da cópia temporária                          */
}

/* ── 5.1.2 Clientes que compraram num período ───────────────── */
void relClientesQueCompraram(void) {
    int dI, mI, aI, dF, mF, aF;
    printf("\n=== CLIENTES QUE COMPRARAM ===\n");
    printf("Periodo inicial (dd/mm/aaaa): "); scanf("%d/%d/%d", &dI, &mI, &aI); while (getchar() != '\n');
    printf("Periodo final   (dd/mm/aaaa): "); scanf("%d/%d/%d", &dF, &mF, &aF); while (getchar() != '\n');

    printf("\n%-5s %-30s %-12s\n", "COD", "CLIENTE", "DATA");
    printf("--------------------------------------------------\n");

    NodoVenda *cv = listaVendas;
    while (cv) {
        /* verifica se a data da venda está dentro do intervalo [ini, fim]
           A lógica compara: ano primeiro, depois mês, depois dia           */
        int dentroP =
            (cv->data.ano > aI  || (cv->data.ano == aI && cv->data.mes > mI)  ||
             (cv->data.ano == aI && cv->data.mes == mI && cv->data.dia >= dI)) &&
            (cv->data.ano < aF  || (cv->data.ano == aF && cv->data.mes < mF)  ||
             (cv->data.ano == aF && cv->data.mes == mF && cv->data.dia <= dF));

        if (dentroP) {
            /* busca o nome do cliente pelo código guardado na venda */
            for (int i = 0; i < qtdC; i++) {
                if (clientes[i].codC == cv->data.codCliente) {
                    printf("%-5d %-30s %02d/%02d/%04d\n",
                           clientes[i].codC, clientes[i].nome,
                           cv->data.dia, cv->data.mes, cv->data.ano);
                    break;
                }
            }
        }
        cv = cv->prox;
    }
}

/* ── 5.2.1 Produtos em ordem alfabética ─────────────────────── */
void relProdutosAlfabetica(void) {
    if (qtdP == 0) { printf("Nenhum produto cadastrado.\n"); return; }
    Produto *tmp = malloc(qtdP * sizeof(Produto));
    memcpy(tmp, produtos, qtdP * sizeof(Produto));
    int trocou;
    do {
        trocou = 0;
        for (int i = 0; i < qtdP - 1; i++) {
            if (strcmp(tmp[i].nomeProduto, tmp[i+1].nomeProduto) > 0) {
                Produto aux = tmp[i]; tmp[i] = tmp[i+1]; tmp[i+1] = aux;
                trocou = 1;
            }
        }
    } while (trocou);

    printf("\n=== PRODUTOS - ORDEM ALFABETICA ===\n");
    printf("%-6s %-30s %-15s %10s %6s\n", "COD", "DESCRICAO", "CATEGORIA", "VL.VENDA", "ESTQ");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < qtdP; i++) {
        char *cat = (tmp[i].categoria == 1) ? "Alimentos" :
                    (tmp[i].categoria == 2) ? "Limpeza"   : "Panificadora";
        printf("%-6d %-30s %-15s R$%8.2f %6d\n",
               tmp[i].codigo, tmp[i].nomeProduto, cat, tmp[i].precoVenda, tmp[i].estoque);
    }
    free(tmp);
}

/* ── 5.2.2 Produtos com estoque zero ou no mínimo ───────────── */
void relEstoqueMinimo(void) {
    printf("\n=== PRODUTOS COM ESTOQUE CRITICO ===\n");
    printf("%-6s %-30s %6s %6s\n", "COD", "DESCRICAO", "ESTQ", "MIN");
    printf("--------------------------------------------------\n");
    int achou = 0;
    for (int i = 0; i < qtdP; i++) {
        /* condição: estoque atual <= estoque mínimo configurado */
        if (produtos[i].estoque <= produtos[i].estoqueMin) {
            printf("%-6d %-30s %6d %6d\n",
                   produtos[i].codigo, produtos[i].nomeProduto,
                   produtos[i].estoque, produtos[i].estoqueMin);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhum produto com estoque critico.\n");
}

/* ── 5.2.3 Produtos mais vendidos num período ───────────────── */
/*
 * Algoritmo:
 * 1. Aloca três arrays auxiliares (um por produto): qtdVendida[], totVendido[], idxProd[]
 * 2. Percorre vendas do período → para cada venda percorre os itens → acumula quantidade
 * 3. Ordena o vetor de índices (idxProd) por qtdVendida decrescente (bubble sort)
 * 4. Exibe o ranking pulando produtos com qtd = 0
 */
void relProdutosMaisVendidos(void) {
    int dI, mI, aI, dF, mF, aF;

    printf("\n=== PRODUTOS MAIS VENDIDOS POR PERIODO ===\n");
    printf("Periodo inicial (dd/mm/aaaa): "); scanf("%d/%d/%d", &dI, &mI, &aI); while (getchar() != '\n');
    printf("Periodo final   (dd/mm/aaaa): "); scanf("%d/%d/%d", &dF, &mF, &aF); while (getchar() != '\n');

    if (qtdP == 0) { printf("Nenhum produto cadastrado.\n"); return; }

    /* calloc: aloca E inicializa tudo com zero (diferente do malloc) */
    int   *qtdVendida = calloc(qtdP, sizeof(int));
    float *totVendido = calloc(qtdP, sizeof(float));
    int   *idxProd    = malloc(qtdP * sizeof(int));

    if (!qtdVendida || !totVendido || !idxProd) {
        printf("Erro de memoria!\n");
        free(qtdVendida); free(totVendido); free(idxProd);
        return;
    }

    for (int i = 0; i < qtdP; i++) idxProd[i] = i;   /* índices 0,1,2,3...      */

    /* percorre as vendas do período e acumula */
    NodoVenda *cv = listaVendas;
    while (cv) {
        int dentroP =
            (cv->data.ano > aI  || (cv->data.ano == aI && cv->data.mes > mI)  ||
             (cv->data.ano == aI && cv->data.mes == mI && cv->data.dia >= dI)) &&
            (cv->data.ano < aF  || (cv->data.ano == aF && cv->data.mes < mF)  ||
             (cv->data.ano == aF && cv->data.mes == mF && cv->data.dia <= dF));

        if (dentroP) {
            NodoItem *ci = listaItens;
            while (ci) {
                if (ci->data.numVenda == cv->data.numVenda) {
                    /* localiza qual índice do array corresponde a este produto */
                    for (int i = 0; i < qtdP; i++) {
                        if (produtos[i].codigo == ci->data.codProduto) {
                            qtdVendida[i] += ci->data.quantidade;
                            totVendido[i] += ci->data.totalItem;
                            break;
                        }
                    }
                }
                ci = ci->prox;
            }
        }
        cv = cv->prox;
    }

    /* bubble sort nos ÍNDICES (ordena idxProd comparando qtdVendida) */
    int trocou;
    do {
        trocou = 0;
        for (int i = 0; i < qtdP - 1; i++) {
            /* se o produto na posição i foi MENOS vendido que o i+1: troca */
            if (qtdVendida[idxProd[i]] < qtdVendida[idxProd[i+1]]) {
                int tmp = idxProd[i]; idxProd[i] = idxProd[i+1]; idxProd[i+1] = tmp;
                trocou = 1;
            }
        }
    } while (trocou);

    printf("\n%-4s %-30s %-15s %8s %12s\n",
           "POS", "PRODUTO", "CATEGORIA", "QTD.VND", "TOTAL.VND");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < qtdP; i++) {
        int idx = idxProd[i];
        if (qtdVendida[idx] == 0) continue;   /* pula produtos sem vendas          */
        char *cat = (produtos[idx].categoria == 1) ? "Alimentos" :
                    (produtos[idx].categoria == 2) ? "Limpeza"   : "Panificadora";
        printf("%-4d %-30s %-15s %8d R$%10.2f\n",
               i + 1, produtos[idx].nomeProduto, cat,
               qtdVendida[idx], totVendido[idx]);
    }

    free(qtdVendida);
    free(totVendido);
    free(idxProd);
}

/* ── 5.3.1 Vendas num período ───────────────────────────────── */
void relVendasPeriodo(void) {
    int dI, mI, aI, dF, mF, aF;
    printf("\n=== VENDAS POR PERIODO ===\n");
    printf("Periodo inicial (dd/mm/aaaa): "); scanf("%d/%d/%d", &dI, &mI, &aI); while (getchar() != '\n');
    printf("Periodo final   (dd/mm/aaaa): "); scanf("%d/%d/%d", &dF, &mF, &aF); while (getchar() != '\n');

    printf("\n%-8s %-12s %12s\n", "N.VENDA", "DATA", "TOTAL");
    printf("----------------------------------\n");

    NodoVenda *cv = listaVendas;
    while (cv) {
        int dentroP =
            (cv->data.ano > aI  || (cv->data.ano == aI && cv->data.mes > mI)  ||
             (cv->data.ano == aI && cv->data.mes == mI && cv->data.dia >= dI)) &&
            (cv->data.ano < aF  || (cv->data.ano == aF && cv->data.mes < mF)  ||
             (cv->data.ano == aF && cv->data.mes == mF && cv->data.dia <= dF));
        if (dentroP)
            printf("%-8d %02d/%02d/%04d R$%10.2f\n",
                   cv->data.numVenda, cv->data.dia, cv->data.mes, cv->data.ano,
                   cv->data.totalVenda);
        cv = cv->prox;
    }
}

/* ── 5.3.2 Faturamento consolidado num período ──────────────── */
void relFaturamentoConsolidado(void) {
    int dI, mI, aI, dF, mF, aF;
    float totDinheiro = 0, totCartao = 0, totGeral = 0;

    printf("\n=== FATURAMENTO CONSOLIDADO ===\n");
    printf("Periodo inicial (dd/mm/aaaa): "); scanf("%d/%d/%d", &dI, &mI, &aI); while (getchar() != '\n');
    printf("Periodo final   (dd/mm/aaaa): "); scanf("%d/%d/%d", &dF, &mF, &aF); while (getchar() != '\n');

    NodoVenda *cv = listaVendas;
    while (cv) {
        int dentroP =
            (cv->data.ano > aI  || (cv->data.ano == aI && cv->data.mes > mI)  ||
             (cv->data.ano == aI && cv->data.mes == mI && cv->data.dia >= dI)) &&
            (cv->data.ano < aF  || (cv->data.ano == aF && cv->data.mes < mF)  ||
             (cv->data.ano == aF && cv->data.mes == mF && cv->data.dia <= dF));

        if (dentroP) {
            totGeral += cv->data.totalVenda;

            /* percorre pagamentos desta venda para separar dinheiro/cartão */
            NodoPagamento *cp = listaPagamentos;
            while (cp) {
                if (cp->data.numVenda == cv->data.numVenda) {
                    /* tipo[0]=='d' pega "d"; strcmp pega "md" */
                    if (cp->data.tipo[0] == 'd' || strcmp(cp->data.tipo, "md") == 0)
                        totDinheiro += cp->data.valorPago;
                    else
                        totCartao += cp->data.valorPago;
                }
                cp = cp->prox;
            }
        }
        cv = cv->prox;
    }

    printf("-------------------------------\n");
    printf("Total em dinheiro : R$ %.2f\n", totDinheiro);
    printf("Total em cartao   : R$ %.2f\n", totCartao);
    printf("Total geral       : R$ %.2f\n", totGeral);
}


/* ============================================================
   SEÇÃO 14 — MENUS
   ============================================================
   Cada menu exibe as opções, lê a escolha com lerInt() e
   chama a função correspondente via switch-case.
   ============================================================ */

/* Menu de relatórios com as 7 opções */
void menuRelatorios(void) {
    int opc;
    printf("\n===== Relatorios =====\n");
    printf("1 - Clientes - Ordem Alfabetica\n");
    printf("2 - Clientes que Compraram (periodo)\n");
    printf("3 - Produtos - Ordem Alfabetica\n");
    printf("4 - Produtos com Estoque Critico\n");
    printf("5 - Produtos mais Vendidos (periodo)\n");
    printf("6 - Vendas por Periodo\n");
    printf("7 - Faturamento Consolidado\n");
    printf("0 - Voltar\n");
    opc = lerInt("Escolha: ");

    switch (opc) {
        case 1: relClientesAlfabetica();     break;
        case 2: relClientesQueCompraram();   break;
        case 3: relProdutosAlfabetica();     break;
        case 4: relEstoqueMinimo();          break;
        case 5: relProdutosMaisVendidos();   break;
        case 6: relVendasPeriodo();          break;
        case 7: relFaturamentoConsolidado(); break;
        case 0: break;
        default: printf("Opcao invalida!\n");
    }
}

/* Menu de cadastros */
void menuCadastro(void) {
    int opc;
    printf("\n===== Cadastros =====\n");
    printf("1 - Cadastrar usuario\n");
    printf("2 - Cadastrar cliente\n");
    printf("3 - Cadastrar produto\n");
    printf("4 - Listar clientes\n");
    printf("5 - Listar produtos\n");
    printf("6 - Voltar\n");
    opc = lerInt("Escolha: ");

    switch (opc) {
        /* cadastrar usuário exige validação de admin */
        case 1: if (validarAdmin()) cadastrarUsuario(); break;
        case 2: cadastrarCliente(); break;
        case 3: cadastrarProduto(); break;
        case 4: listarClientes();   break;
        case 5: listarProdutos();   break;
        case 6: break;
        default: printf("Opcao invalida!\n");
    }
}

/* Menu de vendas */
void menuVendas(void) {
    int opc;
    printf("\n===== Menu de Vendas =====\n");
    printf("1 - Nova venda\n");
    printf("2 - Retirada de caixa (SANGRIA)\n");
    printf("3 - Pagar vendas em aberto\n");
    printf("4 - Voltar\n");
    opc = lerInt("Escolha: ");

    switch (opc) {
        case 1: novaVenda();          break;
        case 2: sangria();            break;
        case 3: pagarVendasAbertas(); break;
        case 4: break;
        default: printf("Opcao invalida!\n");
    }
}


/* ============================================================
   SEÇÃO 15 — INICIALIZAÇÃO E MAIN
   ============================================================ */

/*
 * inicializar()
 * ─────────────────────────────────────────────────────────────
 * Primeira função chamada pelo main. Carrega todos os arquivos
 * do disco para a memória antes de exibir qualquer menu.
 * Se o arquivo não existe (primeira execução), as funções
 * carregar* simplesmente retornam sem erro.
 * ─────────────────────────────────────────────────────────────
 */
void inicializar(void) {
    carregarUsuarios();    /* popula listaUsuarios                                */
    carregarClientes();    /* popula array clientes[]                             */
    carregarProdutos();    /* popula array produtos[]                             */
    carregarVendas();      /* popula listaVendas                                  */
    carregarItens();       /* popula listaItens                                   */
    carregarPagamentos();  /* popula listaPagamentos                              */
    carregarCaixa();       /* carrega caixaDoDia (ou inicializa zerado)           */
}

/*
 * main()
 * ─────────────────────────────────────────────────────────────
 * Ponto de entrada do programa (onde tudo começa).
 *
 * Fluxo:
 *   1. inicializar() → carrega dados do disco
 *   2. Define sessão como usuário comum (tipo 2)
 *   3. do-while: exibe o menu principal e executa até digitar 6
 *   4. Ao sair: libera toda a memória alocada
 *
 * Por que liberar a memória?
 * → Boa prática: evita memory leak (vazamento de memória).
 *   Em arrays: free() libera o bloco inteiro.
 *   Em listas: precisa percorrer nó a nó porque cada nó foi
 *   alocado separadamente com malloc.
 * ─────────────────────────────────────────────────────────────
 */
int main(void) {
    int opc;

    inicializar();
    usuarioLogadoTipo = 2;   /* começa como usuário comum                        */

    /* do-while: executa o bloco pelo menos uma vez e repete até opc == 6 */
    do {
        printf("\n===== Mercearia BERE =====\n");
        printf("1 - Cadastros\n");
        printf("2 - Vendas\n");
        printf("3 - Abertura de caixa\n");
        printf("4 - Fechamento de caixa\n");
        printf("5 - Relatorios\n");
        printf("6 - Sair\n");
        opc = lerInt("Escolha: ");

        switch (opc) {
            case 1: menuCadastro();   break;
            case 2: menuVendas();     break;
            case 3: abrirCaixa();     break;
            case 4: fecharCaixa();    break;
            case 5: menuRelatorios(); break;
            case 6: printf("Encerrando o sistema. Ate logo!\n"); break;
            default: printf("Opcao invalida!\n");
        }

    } while (opc != 6);

    /* ── Liberação de memória dos arrays dinâmicos ─────────── */
    free(clientes);   /* libera o bloco inteiro do array de clientes              */
    free(produtos);   /* libera o bloco inteiro do array de produtos              */
    free(carrinho);   /* libera o bloco inteiro do array do carrinho              */

    /* ── Liberação das listas encadeadas (nó a nó) ─────────── */
    /* Padrão: guarda o próximo em variável temp → libera o atual → avança       */
    NodoVenda     *vt;
    while (listaVendas) {
        vt = listaVendas->prox;   /* salva o ponteiro para o próximo             */
        free(listaVendas);        /* libera o nó atual                           */
        listaVendas = vt;         /* avança para o próximo                       */
    }

    NodoItem *it;
    while (listaItens) {
        it = listaItens->prox;
        free(listaItens);
        listaItens = it;
    }

    NodoPagamento *pt;
    while (listaPagamentos) {
        pt = listaPagamentos->prox;
        free(listaPagamentos);
        listaPagamentos = pt;
    }

    NodoUsuario *ut;
    while (listaUsuarios) {
        ut = listaUsuarios->prox;
        free(listaUsuarios);
        listaUsuarios = ut;
    }

    return 0;   /* retorna 0 ao sistema operacional: encerramento sem erros      */
}
