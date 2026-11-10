#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ARQUIVO_CLIENTES "clientes.dat"
#define ARQUIVO_PRODUTOS "produtos.dat"
#define ARQUIVO_VENDAS "vendas.dat"
#define ARQUIVO_ITENS "itens.dat"
#define ARQUIVO_PAGAMENTOS "pagamentos.dat"
#define ARQUIVO_CAIXA "caixa.dat"
#define ARQUIVO_USUARIOS "usuarios.dat"


/* Dados de um cliente da mercearia */
typedef struct {
    int codigoCliente;
    char nomeCompleto[50];
    char nomeSocial[20];
    char cpf[15];
    char rua[100];
    char bairro[100];
    char telefone[20];
} Cliente;

/* Dados de um produto a venda */
typedef struct {
    int codigoProduto;
    char nomeProduto[100];
    int categoria; /* 1 = Alimentos, 2 = Limpeza, 3 = Panificadora */
    float precoDeCompra;
    float margemDeLucro;
    float precoDeVenda;
    int estoqueMinimo;
    int quantidadeEmEstoque;
} Produto;

/* Um item que esta dentro do carrinho de compras durante uma venda */
typedef struct {
    int codigoProduto;
    char nomeProduto[100];
    float precoDeVenda;
    int quantidade;
} ItemDoCarrinho;

/* Cabecalho de uma venda (quem comprou, quando, e o total) */
typedef struct {
    int numeroDaVenda;
    int codigoDoCliente;
    int dia;
    int mes;
    int ano;
    float valorTotalDaVenda;
} DadosDaVenda;

/* Cada produto vendido dentro de uma venda */
typedef struct {
    int numeroDaVenda;
    int codigoDoProduto;
    char descricaoDoProduto[100];
    float precoDeVenda;
    int quantidade;
    float valorTotalDoItem;
    char statusPagamento; /* 'a' = aberto (nao pago), 'p' = pago */
} DadosDoItem;

/* Um pagamento feito para uma venda */
typedef struct {
    int numeroDaVenda;
    float valorPago;
    char formaDePagamento[3]; /* "d"=dinheiro, "c"=cartao, "md"/"mc"=misto */
} DadosDoPagamento;

/* Dados de login de um usuario do sistema */
typedef struct {
    int codigoUsuario;
    char login[13];
    char senha[9];
    int tipoUsuario; /* 1 = Administrador, 2 = Usuario comum */
} DadosDoUsuario;

/* Controle financeiro do caixa do dia */
typedef struct {
    float valorDeAbertura;
    float faturamentoTotal;
    float totalPagoEmDinheiro;
    float totalPagoEmCartao;
    float totalMistoParteDinheiro;
    float totalMistoParteCartao;
    float valorDeAjuste;
    int caixaEstaAberto; /* 1 = aberto, 0 = fechado */
} DadosDoCaixa;

typedef struct NoDaVenda {
    DadosDaVenda dados;
    struct NoDaVenda *proximo;
} NoDaVenda;

typedef struct NoDoItem {
    DadosDoItem dados;
    struct NoDoItem *proximo;
} NoDoItem;

typedef struct NoDoPagamento {
    DadosDoPagamento dados;
    struct NoDoPagamento *proximo;
} NoDoPagamento;

typedef struct NoDoUsuario {
    DadosDoUsuario dados;
    struct NoDoUsuario *proximo;
} NoDoUsuario;

/* Vetores dinamicos de clientes e produtos */
Cliente *listaDeClientes = NULL;
Produto *listaDeProdutos = NULL;

int quantidadeDeClientes = 0;
int capacidadeDeClientes = 0;
int quantidadeDeProdutos = 0;
int capacidadeDeProdutos = 0;

/* Carrinho de compras usado durante uma venda */
ItemDoCarrinho *carrinhoDeCompras = NULL;
int quantidadeDeItensNoCarrinho = 0;
int capacidadeDoCarrinho = 0;
float valorTotalDoCarrinho = 0.0;

/* Listas encadeadas */
NoDaVenda *inicioDaListaDeVendas = NULL;
NoDoItem *inicioDaListaDeItens = NULL;
NoDoPagamento *inicioDaListaDePagamentos = NULL;
NoDoUsuario *inicioDaListaDeUsuarios = NULL;

int totalDeVendas = 0;
int totalDeItens = 0;
int totalDePagamentos = 0;
int totalDeUsuarios = 0;

/* Caixa do dia e tipo do usuario atualmente logado */
DadosDoCaixa caixaDoDia;
int tipoDoUsuarioLogado = 2; /* Comeca como usuario comum */

int lerInt(const char *msg) {
    int valor;
    printf("%s", msg);
    while (scanf("%d", &valor) != 1) {
        printf("Entrada invalida, digite um numero inteiro: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return valor;
}

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
}

/* Calcula o preco de venda a partir do preco de compra e da margem de lucro */
float calcularPrecoDeVenda(float precoDeCompra, float margemDeLucro) {
    return precoDeCompra * (1.0f + margemDeLucro / 100.0f);
}

/* ============================================================
   FUNCOES PARA INSERIR NO'S NAS LISTAS ENCADEADAS
   ============================================================ */

/* Insere uma nova venda no inicio da lista de vendas */
void inserirVendaNaLista(DadosDaVenda venda) {
    NoDaVenda *novoNo = malloc(sizeof(NoDaVenda));

    if (novoNo == NULL) {
        printf("Sem memoria!\n");
        return;
    }

    novoNo->dados = venda;
    novoNo->proximo = inicioDaListaDeVendas;
    inicioDaListaDeVendas = novoNo;
}

/* Insere um novo item no inicio da lista de itens */
void inserirItemNaLista(DadosDoItem item) {
    NoDoItem *novoNo = malloc(sizeof(NoDoItem));

    if (novoNo == NULL) {
        printf("Sem memoria!\n");
        return;
    }

    novoNo->dados = item;
    novoNo->proximo = inicioDaListaDeItens;
    inicioDaListaDeItens = novoNo;
}

/* Insere um novo pagamento no inicio da lista de pagamentos */
void inserirPagamentoNaLista(DadosDoPagamento pagamento) {
    NoDoPagamento *novoNo = malloc(sizeof(NoDoPagamento));

    if (novoNo == NULL) {
        printf("Sem memoria!\n");
        return;
    }

    novoNo->dados = pagamento;
    novoNo->proximo = inicioDaListaDePagamentos;
    inicioDaListaDePagamentos = novoNo;
}

/* Insere um novo usuario no inicio da lista de usuarios */
void inserirUsuarioNaLista(DadosDoUsuario usuario) {
    NoDoUsuario *novoNo = malloc(sizeof(NoDoUsuario));

    if (novoNo == NULL) {
        printf("Sem memoria!\n");
        return;
    }

    novoNo->dados = usuario;
    novoNo->proximo = inicioDaListaDeUsuarios;
    inicioDaListaDeUsuarios = novoNo;
}

/* ============================================================
   FUNCOES PARA SALVAR E CARREGAR DADOS DOS ARQUIVOS
   ============================================================ */

/* Salva todos os clientes no arquivo */
void salvarClientes(void) {
    FILE *arquivo = fopen(ARQUIVO_CLIENTES, "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar clientes!\n");
        return;
    }

    fwrite(&quantidadeDeClientes, sizeof(int), 1, arquivo);
    fwrite(listaDeClientes, sizeof(Cliente), quantidadeDeClientes, arquivo);
    fclose(arquivo);
}

/* Carrega todos os clientes do arquivo para a memoria */
void carregarClientes(void) {
    FILE *arquivo = fopen(ARQUIVO_CLIENTES, "rb");

    if (arquivo == NULL) {
        return;
    }

    fread(&quantidadeDeClientes, sizeof(int), 1, arquivo);

    capacidadeDeClientes = (quantidadeDeClientes > 0) ? quantidadeDeClientes : 10;
    listaDeClientes = malloc(capacidadeDeClientes * sizeof(Cliente));

    if (listaDeClientes == NULL) {
        fclose(arquivo);
        return;
    }

    fread(listaDeClientes, sizeof(Cliente), quantidadeDeClientes, arquivo);
    fclose(arquivo);
}

/* Salva todos os produtos no arquivo */
void salvarProdutos(void) {
    FILE *arquivo = fopen(ARQUIVO_PRODUTOS, "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar produtos!\n");
        return;
    }

    fwrite(&quantidadeDeProdutos, sizeof(int), 1, arquivo);
    fwrite(listaDeProdutos, sizeof(Produto), quantidadeDeProdutos, arquivo);
    fclose(arquivo);
}

/* Carrega todos os produtos do arquivo para a memoria */
void carregarProdutos(void) {
    FILE *arquivo = fopen(ARQUIVO_PRODUTOS, "rb");

    if (arquivo == NULL) {
        return;
    }

    fread(&quantidadeDeProdutos, sizeof(int), 1, arquivo);

    capacidadeDeProdutos = (quantidadeDeProdutos > 0) ? quantidadeDeProdutos : 10;
    listaDeProdutos = malloc(capacidadeDeProdutos * sizeof(Produto));

    if (listaDeProdutos == NULL) {
        fclose(arquivo);
        return;
    }

    fread(listaDeProdutos, sizeof(Produto), quantidadeDeProdutos, arquivo);
    fclose(arquivo);
}

/* Salva todas as vendas no arquivo */
void salvarVendas(void) {
    FILE *arquivo = fopen(ARQUIVO_VENDAS, "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar vendas!\n");
        return;
    }

    fwrite(&totalDeVendas, sizeof(int), 1, arquivo);

    NoDaVenda *noAtual = inicioDaListaDeVendas;
    while (noAtual != NULL) {
        fwrite(&noAtual->dados, sizeof(DadosDaVenda), 1, arquivo);
        noAtual = noAtual->proximo;
    }

    fclose(arquivo);
}

/* Carrega todas as vendas do arquivo para a lista */
void carregarVendas(void) {
    FILE *arquivo = fopen(ARQUIVO_VENDAS, "rb");

    if (arquivo == NULL) {
        return;
    }

    int totalLido;
    fread(&totalLido, sizeof(int), 1, arquivo);

    for (int i = 0; i < totalLido; i++) {
        DadosDaVenda venda;
        fread(&venda, sizeof(DadosDaVenda), 1, arquivo);
        inserirVendaNaLista(venda);
        totalDeVendas++;
    }

    fclose(arquivo);
}

/* Salva todos os itens no arquivo */
void salvarItens(void) {
    FILE *arquivo = fopen(ARQUIVO_ITENS, "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar itens!\n");
        return;
    }

    fwrite(&totalDeItens, sizeof(int), 1, arquivo);

    NoDoItem *noAtual = inicioDaListaDeItens;
    while (noAtual != NULL) {
        fwrite(&noAtual->dados, sizeof(DadosDoItem), 1, arquivo);
        noAtual = noAtual->proximo;
    }

    fclose(arquivo);
}

/* Carrega todos os itens do arquivo para a lista */
void carregarItens(void) {
    FILE *arquivo = fopen(ARQUIVO_ITENS, "rb");

    if (arquivo == NULL) {
        return;
    }

    int totalLido;
    fread(&totalLido, sizeof(int), 1, arquivo);

    for (int i = 0; i < totalLido; i++) {
        DadosDoItem item;
        fread(&item, sizeof(DadosDoItem), 1, arquivo);
        inserirItemNaLista(item);
        totalDeItens++;
    }

    fclose(arquivo);
}

/* Salva todos os pagamentos no arquivo */
void salvarPagamentos(void) {
    FILE *arquivo = fopen(ARQUIVO_PAGAMENTOS, "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar pagamentos!\n");
        return;
    }

    fwrite(&totalDePagamentos, sizeof(int), 1, arquivo);

    NoDoPagamento *noAtual = inicioDaListaDePagamentos;
    while (noAtual != NULL) {
        fwrite(&noAtual->dados, sizeof(DadosDoPagamento), 1, arquivo);
        noAtual = noAtual->proximo;
    }

    fclose(arquivo);
}

/* Carrega todos os pagamentos do arquivo para a lista */
void carregarPagamentos(void) {
    FILE *arquivo = fopen(ARQUIVO_PAGAMENTOS, "rb");

    if (arquivo == NULL) {
        return;
    }

    int totalLido;
    fread(&totalLido, sizeof(int), 1, arquivo);

    for (int i = 0; i < totalLido; i++) {
        DadosDoPagamento pagamento;
        fread(&pagamento, sizeof(DadosDoPagamento), 1, arquivo);
        inserirPagamentoNaLista(pagamento);
        totalDePagamentos++;
    }

    fclose(arquivo);
}

/* Salva todos os usuarios no arquivo */
void salvarUsuarios(void) {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar usuarios!\n");
        return;
    }

    fwrite(&totalDeUsuarios, sizeof(int), 1, arquivo);

    NoDoUsuario *noAtual = inicioDaListaDeUsuarios;
    while (noAtual != NULL) {
        fwrite(&noAtual->dados, sizeof(DadosDoUsuario), 1, arquivo);
        noAtual = noAtual->proximo;
    }

    fclose(arquivo);
}

/* Carrega todos os usuarios do arquivo para a lista */
void carregarUsuarios(void) {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "rb");

    if (arquivo == NULL) {
        return;
    }

    int totalLido;
    fread(&totalLido, sizeof(int), 1, arquivo);

    for (int i = 0; i < totalLido; i++) {
        DadosDoUsuario usuario;
        fread(&usuario, sizeof(DadosDoUsuario), 1, arquivo);
        inserirUsuarioNaLista(usuario);
        totalDeUsuarios++;
    }

    fclose(arquivo);
}

/* Salva os dados do caixa no arquivo */
void salvarCaixa(void) {
    FILE *arquivo = fopen(ARQUIVO_CAIXA, "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar caixa!\n");
        return;
    }

    fwrite(&caixaDoDia, sizeof(DadosDoCaixa), 1, arquivo);
    fclose(arquivo);
}

/* Carrega os dados do caixa do arquivo (ou zera tudo se nao existir) */
void carregarCaixa(void) {
    FILE *arquivo = fopen(ARQUIVO_CAIXA, "rb");

    if (arquivo == NULL) {
        caixaDoDia = (DadosDoCaixa){0};
        return;
    }

    fread(&caixaDoDia, sizeof(DadosDoCaixa), 1, arquivo);
    fclose(arquivo);
}


/* ============================================================
   VALIDACAO DE ADMINISTRADOR
   ============================================================ */

/* Pede login e senha de administrador. Retorna 1 se autorizado, 0 se nao */
int validarAdministrador(void) {
    char loginDigitado[13];
    char senhaDigitada[9];

    /* Se o usuario logado ja e administrador, libera direto */
    if (tipoDoUsuarioLogado == 1) {
        return 1;
    }

    printf("\n--- Acesso restrito a Administrador ---\n");

    printf("Login : ");
    scanf("%12s", loginDigitado);
    while (getchar() != '\n');

    printf("Senha : ");
    scanf("%8s", senhaDigitada);
    while (getchar() != '\n');

    NoDoUsuario *noAtual = inicioDaListaDeUsuarios;
    while (noAtual != NULL) {
        int ehAdministrador = (noAtual->dados.tipoUsuario == 1);
        int loginConfere = (strcmp(noAtual->dados.login, loginDigitado) == 0);
        int senhaConfere = (strcmp(noAtual->dados.senha, senhaDigitada) == 0);

        if (ehAdministrador && loginConfere && senhaConfere) {
            printf("Acesso autorizado!\n");
            return 1;
        }

        noAtual = noAtual->proximo;
    }

    printf("Login ou senha invalidos. Acesso negado.\n");
    return 0;
}


/* ============================================================
   CADASTROS
   ============================================================ */

/* Cadastra um novo usuario do sistema */
void cadastrarUsuario(void) {
    DadosDoUsuario novoUsuario;

    printf("\n=== CADASTRO DE USUARIO ===\n");

    /* Pede o login ate ter pelo menos 8 caracteres */
    do {
        printf("Login (8 a 12 caracteres): ");
        scanf("%12s", novoUsuario.login);
        while (getchar() != '\n');

        if (strlen(novoUsuario.login) < 8) {
            printf("Login muito curto!\n");
        }
    } while (strlen(novoUsuario.login) < 8);

    /* Pede a senha ate ter pelo menos 6 caracteres */
    do {
        printf("Senha (6 a 8 caracteres): ");
        scanf("%8s", novoUsuario.senha);
        while (getchar() != '\n');

        if (strlen(novoUsuario.senha) < 6) {
            printf("Senha muito curta!\n");
        }
    } while (strlen(novoUsuario.senha) < 6);

    /* Pede o tipo ate ser 1 ou 2 */
    do {
        novoUsuario.tipoUsuario = lerInt("Tipo (1-Administrador / 2-Usuario): ");
    } while (novoUsuario.tipoUsuario != 1 && novoUsuario.tipoUsuario != 2);

    novoUsuario.codigoUsuario = totalDeUsuarios + 1;

    inserirUsuarioNaLista(novoUsuario);
    totalDeUsuarios++;
    salvarUsuarios();

    printf("Usuario %d cadastrado com sucesso!\n", novoUsuario.codigoUsuario);
}

/* Cadastra um novo cliente */
void cadastrarCliente(void) {
    /* Se o vetor estiver cheio, dobra a capacidade */
    if (quantidadeDeClientes >= capacidadeDeClientes) {
        capacidadeDeClientes = (capacidadeDeClientes == 0) ? 10 : capacidadeDeClientes * 2;
        listaDeClientes = realloc(listaDeClientes, capacidadeDeClientes * sizeof(Cliente));

        if (listaDeClientes == NULL) {
            printf("Erro de memoria!\n");
            exit(1);
        }
    }

    Cliente *clienteAtual = &listaDeClientes[quantidadeDeClientes];
    clienteAtual->codigoCliente = quantidadeDeClientes + 1;

    printf("\n=== CADASTRO DE CLIENTE ===\n");

    printf("Nome completo: ");
    fgets(clienteAtual->nomeCompleto, sizeof(clienteAtual->nomeCompleto), stdin);
    clienteAtual->nomeCompleto[strcspn(clienteAtual->nomeCompleto, "\n")] = '\0';

    printf("Nome social: ");
    fgets(clienteAtual->nomeSocial, sizeof(clienteAtual->nomeSocial), stdin);
    clienteAtual->nomeSocial[strcspn(clienteAtual->nomeSocial, "\n")] = '\0';

    printf("CPF: ");
    fgets(clienteAtual->cpf, sizeof(clienteAtual->cpf), stdin);
    clienteAtual->cpf[strcspn(clienteAtual->cpf, "\n")] = '\0';

    printf("Rua e numero: ");
    fgets(clienteAtual->rua, sizeof(clienteAtual->rua), stdin);
    clienteAtual->rua[strcspn(clienteAtual->rua, "\n")] = '\0';

    printf("Bairro: ");
    fgets(clienteAtual->bairro, sizeof(clienteAtual->bairro), stdin);
    clienteAtual->bairro[strcspn(clienteAtual->bairro, "\n")] = '\0';

    printf("Celular/WhatsApp: ");
    fgets(clienteAtual->telefone, sizeof(clienteAtual->telefone), stdin);
    clienteAtual->telefone[strcspn(clienteAtual->telefone, "\n")] = '\0';

    quantidadeDeClientes++;
    salvarClientes();

    printf("Cliente %d cadastrado com sucesso!\n", clienteAtual->codigoCliente);
}

/* Cadastra um novo produto */
void cadastrarProduto(void) {
    /* Se o vetor estiver cheio, dobra a capacidade */
    if (quantidadeDeProdutos >= capacidadeDeProdutos) {
        capacidadeDeProdutos = (capacidadeDeProdutos == 0) ? 10 : capacidadeDeProdutos * 2;
        listaDeProdutos = realloc(listaDeProdutos, capacidadeDeProdutos * sizeof(Produto));

        if (listaDeProdutos == NULL) {
            printf("Erro de memoria!\n");
            exit(1);
        }
    }

    Produto *produtoAtual = &listaDeProdutos[quantidadeDeProdutos];
    produtoAtual->codigoProduto = 1000 + quantidadeDeProdutos;

    printf("\n=== CADASTRO DE PRODUTO ===\n");

    printf("Descricao : ");
    fgets(produtoAtual->nomeProduto, sizeof(produtoAtual->nomeProduto), stdin);
    produtoAtual->nomeProduto[strcspn(produtoAtual->nomeProduto, "\n")] = '\0';

    printf("Categoria:\n");
    printf(" 1 - Alimentos\n");
    printf(" 2 - Material de Limpeza\n");
    printf(" 3 - Panificadora\n");

    do {
        produtoAtual->categoria = lerInt("Opcao: ");
    } while (produtoAtual->categoria < 1 || produtoAtual->categoria > 3);

    produtoAtual->precoDeCompra = lerFloat("Preco de custo : R$ ");
    produtoAtual->margemDeLucro = lerFloat("Margem de lucro : %% ");
    produtoAtual->precoDeVenda = calcularPrecoDeVenda(produtoAtual->precoDeCompra,
                                                       produtoAtual->margemDeLucro);

    printf("Preco de venda calculado: R$ %.2f\n", produtoAtual->precoDeVenda);

    produtoAtual->quantidadeEmEstoque = lerInt("Qtd em estoque : ");
    produtoAtual->estoqueMinimo = lerInt("Estoque minimo : ");

    quantidadeDeProdutos++;
    salvarProdutos();

    printf("Produto %d cadastrado com sucesso!\n", produtoAtual->codigoProduto);
}


/* ============================================================
   LISTAGENS
   ============================================================ */

/* Mostra todos os clientes cadastrados */
void listarClientes(void) {
    if (quantidadeDeClientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    printf("\n%-5s %-30s %-15s %-20s\n", "COD", "NOME", "CPF", "CELULAR");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < quantidadeDeClientes; i++) {
        printf("%-5d %-30s %-15s %-20s\n",
               listaDeClientes[i].codigoCliente,
               listaDeClientes[i].nomeCompleto,
               listaDeClientes[i].cpf,
               listaDeClientes[i].telefone);
    }
}

/* Mostra todos os produtos cadastrados */
void listarProdutos(void) {
    if (quantidadeDeProdutos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("\n%-6s %-30s %-15s %10s %6s %6s\n",
           "COD", "DESCRICAO", "CATEGORIA", "VL.VENDA", "ESTQ", "MIN");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < quantidadeDeProdutos; i++) {
        char *nomeCategoria;

        if (listaDeProdutos[i].categoria == 1) {
            nomeCategoria = "Alimentos";
        } else if (listaDeProdutos[i].categoria == 2) {
            nomeCategoria = "Limpeza";
        } else {
            nomeCategoria = "Panificadora";
        }

        printf("%-6d %-30s %-15s R$%8.2f %6d %6d\n",
               listaDeProdutos[i].codigoProduto,
               listaDeProdutos[i].nomeProduto,
               nomeCategoria,
               listaDeProdutos[i].precoDeVenda,
               listaDeProdutos[i].quantidadeEmEstoque,
               listaDeProdutos[i].estoqueMinimo);
    }
}


/* ============================================================
   CARRINHO DE COMPRAS
   ============================================================ */

/* Adiciona um produto ao carrinho (ou soma a quantidade se ja existir) */
void adicionarAoCarrinho(int indiceDoProduto, int quantidade) {
    /* Se o produto ja esta no carrinho, apenas soma a quantidade */
    for (int i = 0; i < quantidadeDeItensNoCarrinho; i++) {
        if (carrinhoDeCompras[i].codigoProduto == listaDeProdutos[indiceDoProduto].codigoProduto) {
            carrinhoDeCompras[i].quantidade += quantidade;
            valorTotalDoCarrinho += listaDeProdutos[indiceDoProduto].precoDeVenda * quantidade;
            return;
        }
    }

    /* Se o carrinho estiver cheio, dobra a capacidade */
    if (quantidadeDeItensNoCarrinho >= capacidadeDoCarrinho) {
        capacidadeDoCarrinho = (capacidadeDoCarrinho == 0) ? 10 : capacidadeDoCarrinho * 2;
        carrinhoDeCompras = realloc(carrinhoDeCompras, capacidadeDoCarrinho * sizeof(ItemDoCarrinho));

        if (carrinhoDeCompras == NULL) {
            printf("Erro de memoria no carrinho!\n");
            exit(1);
        }
    }

    /* Coloca o novo produto no carrinho */
    carrinhoDeCompras[quantidadeDeItensNoCarrinho].codigoProduto = listaDeProdutos[indiceDoProduto].codigoProduto;
    carrinhoDeCompras[quantidadeDeItensNoCarrinho].precoDeVenda = listaDeProdutos[indiceDoProduto].precoDeVenda;
    carrinhoDeCompras[quantidadeDeItensNoCarrinho].quantidade = quantidade;
    strcpy(carrinhoDeCompras[quantidadeDeItensNoCarrinho].nomeProduto,
           listaDeProdutos[indiceDoProduto].nomeProduto);

    quantidadeDeItensNoCarrinho++;
    valorTotalDoCarrinho += listaDeProdutos[indiceDoProduto].precoDeVenda * quantidade;
}


/* ============================================================
   PAGAMENTO
   ============================================================ */

/* Registra o pagamento de uma venda (dinheiro, cartao ou misto) */
void registrarPagamento(int numeroDaVenda, float valorTotal) {
    DadosDoPagamento pagamento;
    int formaEscolhida;
    float valorRecebido;
    float valorRestante;
    float percentualDesconto = 0.0;
    float valorComDesconto;

    printf("\nTotal a pagar: R$ %.2f\n", valorTotal);
    printf("Forma de pagamento:\n");
    printf(" 1 - Dinheiro\n");
    printf(" 2 - Cartao\n");
    formaEscolhida = lerInt("Opcao: ");

    pagamento.numeroDaVenda = numeroDaVenda;

    /* ----- PAGAMENTO EM DINHEIRO ----- */
    if (formaEscolhida == 1) {
        percentualDesconto = lerFloat("Desconto (0 para nenhum, ou % concedido): ");

        if (percentualDesconto < 0.0f) {
            percentualDesconto = 0.0f;
        }

        valorComDesconto = valorTotal - (valorTotal * percentualDesconto / 100.0f);

        if (percentualDesconto > 0.0f) {
            printf("Desconto de %.2f%% aplicado. Valor final: R$ %.2f\n",
                   percentualDesconto, valorComDesconto);
        }

        valorRecebido = lerFloat("Valor recebido : R$ ");

        /* Se o cliente nao deu dinheiro suficiente */
        if (valorRecebido < valorComDesconto) {
            valorRestante = valorComDesconto - valorRecebido;

            printf("Valor insuficiente! Restam R$ %.2f. Pagar restante no cartao (s/n)? ",
                   valorRestante);

            char resposta;
            scanf(" %c", &resposta);
            while (getchar() != '\n');

            if (resposta != 's' && resposta != 'S') {
                printf("Pagamento cancelado.\n");
                return;
            }

            /* Parte paga em dinheiro */
            pagamento.valorPago = valorRecebido;
            strcpy(pagamento.formaDePagamento, "md");
            inserirPagamentoNaLista(pagamento);
            totalDePagamentos++;
            caixaDoDia.totalMistoParteDinheiro += valorRecebido;

            /* Parte paga no cartao */
            pagamento.valorPago = valorRestante;
            strcpy(pagamento.formaDePagamento, "mc");
            inserirPagamentoNaLista(pagamento);
            totalDePagamentos++;
            caixaDoDia.totalMistoParteCartao += valorRestante;

            printf("Pagamento misto registrado: R$ %.2f dinheiro + R$ %.2f cartao.\n",
                   valorRecebido, valorRestante);

        } else {
            /* O cliente pagou tudo (e talvez sobre troco) */
            printf("Troco: R$ %.2f\n", valorRecebido - valorComDesconto);

            pagamento.valorPago = valorComDesconto;
            strcpy(pagamento.formaDePagamento, "d");
            inserirPagamentoNaLista(pagamento);
            totalDePagamentos++;
            caixaDoDia.totalPagoEmDinheiro += valorComDesconto;
        }

    /* ----- PAGAMENTO EM CARTAO ----- */
    } else {
        int pagamentoDeuCerto;

        /* Repete ate o pagamento no cartao ser confirmado */
        do {
            printf("O pagamento deu certo?\n");
            printf(" 1 - Efetuado!\n");
            printf(" 2 - Erro, tentar novamente\n");
            pagamentoDeuCerto = lerInt("Opcao: ");

            if (pagamentoDeuCerto == 2) {
                printf("Tentando novamente...\n");
            }
        } while (pagamentoDeuCerto != 1);

        valorRecebido = lerFloat("Valor no cartao : R$ ");

        /* Se o cartao cobriu o valor total */
        if (valorRecebido >= valorTotal) {
            pagamento.valorPago = valorTotal;
            strcpy(pagamento.formaDePagamento, "c");
            inserirPagamentoNaLista(pagamento);
            totalDePagamentos++;
            caixaDoDia.totalPagoEmCartao += valorTotal;
            valorComDesconto = valorTotal;

        } else {
            /* Faltou no cartao, o resto sera em dinheiro */
            valorRestante = valorTotal - valorRecebido;
            printf("Restante de R$ %.2f sera em dinheiro.\n", valorRestante);

            /* Parte paga no cartao */
            pagamento.valorPago = valorRecebido;
            strcpy(pagamento.formaDePagamento, "mc");
            inserirPagamentoNaLista(pagamento);
            totalDePagamentos++;
            caixaDoDia.totalMistoParteCartao += valorRecebido;

            /* Parte paga em dinheiro */
            pagamento.valorPago = valorRestante;
            strcpy(pagamento.formaDePagamento, "md");
            inserirPagamentoNaLista(pagamento);
            totalDePagamentos++;
            caixaDoDia.totalMistoParteDinheiro += valorRestante;

            valorComDesconto = valorTotal;
        }
    }

    /* Marca todos os itens dessa venda como pagos */
    NoDoItem *noAtual = inicioDaListaDeItens;
    while (noAtual != NULL) {
        if (noAtual->dados.numeroDaVenda == numeroDaVenda) {
            noAtual->dados.statusPagamento = 'p';
        }
        noAtual = noAtual->proximo;
    }

    caixaDoDia.faturamentoTotal += valorComDesconto;

    salvarCaixa();
    salvarPagamentos();
    salvarItens();

    printf("Pagamento registrado! Saldo em caixa: R$ %.2f\n",
           caixaDoDia.valorDeAbertura + caixaDoDia.faturamentoTotal);
}


/* ============================================================
   VENDAS
   ============================================================ */

/* Realiza uma nova venda do inicio ao fim */
void novaVenda(void) {
    if (!caixaDoDia.caixaEstaAberto) {
        printf("Caixa fechado! Abra o caixa antes de realizar vendas.\n");
        return;
    }

    if (quantidadeDeProdutos == 0) {
        printf("Nenhum produto cadastrado!\n");
        return;
    }

    printf("\n=== NOVA VENDA ===\n");
    int codigoDoCliente = lerInt("Codigo do cliente (0 = sem cliente): ");
    int dia = lerInt("Dia : ");
    int mes = lerInt("Mes : ");
    int ano = lerInt("Ano : ");

    /* Zera o carrinho para comecar a venda */
    quantidadeDeItensNoCarrinho = 0;
    valorTotalDoCarrinho = 0.0;

    int continuarComprando = 1;
    while (continuarComprando) {

        /* Mostra a lista de produtos disponiveis */
        printf("\n%-6s %-30s %-15s %10s %6s\n",
               "COD", "DESCRICAO", "CATEGORIA", "VL.VENDA", "ESTQ");
        printf("------------------------------------------------------------------------\n");

        for (int i = 0; i < quantidadeDeProdutos; i++) {
            char *nomeCategoria;

            if (listaDeProdutos[i].categoria == 1) {
                nomeCategoria = "Alimentos";
            } else if (listaDeProdutos[i].categoria == 2) {
                nomeCategoria = "Limpeza";
            } else {
                nomeCategoria = "Panificadora";
            }

            printf("%-6d %-30s %-15s R$%8.2f %6d\n",
                   listaDeProdutos[i].codigoProduto,
                   listaDeProdutos[i].nomeProduto,
                   nomeCategoria,
                   listaDeProdutos[i].precoDeVenda,
                   listaDeProdutos[i].quantidadeEmEstoque);
        }

        /* Mostra o carrinho atual, se tiver algo */
        if (quantidadeDeItensNoCarrinho > 0) {
            printf("\n---- Carrinho Atual ----\n");
            printf("%-6s %-30s %10s %6s %10s\n",
                   "COD", "PRODUTO", "PR.UNIT", "QTD", "SUBTOTAL");

            for (int i = 0; i < quantidadeDeItensNoCarrinho; i++) {
                float subtotal = carrinhoDeCompras[i].precoDeVenda * carrinhoDeCompras[i].quantidade;
                printf("%-6d %-30s R$%8.2f %6d R$%8.2f\n",
                       carrinhoDeCompras[i].codigoProduto,
                       carrinhoDeCompras[i].nomeProduto,
                       carrinhoDeCompras[i].precoDeVenda,
                       carrinhoDeCompras[i].quantidade,
                       subtotal);
            }

            printf("TOTAL: R$ %.2f\n", valorTotalDoCarrinho);
        }

        int codigoDigitado = lerInt("\nCodigo do produto (0 para encerrar): ");

        if (codigoDigitado == 0) {
            continuarComprando = 0;
            break;
        }

        int quantidadeDigitada = lerInt("Quantidade: ");

        if (quantidadeDigitada <= 0) {
            printf("Quantidade invalida!\n");
            continue;
        }

        /* Procura o produto pelo codigo digitado */
        int indiceDoProduto = -1;
        for (int i = 0; i < quantidadeDeProdutos; i++) {
            if (listaDeProdutos[i].codigoProduto == codigoDigitado) {
                indiceDoProduto = i;
                break;
            }
        }

        /* Trata os casos possiveis */
        if (indiceDoProduto == -1) {
            printf("Produto nao encontrado!\n");

        } else if (listaDeProdutos[indiceDoProduto].quantidadeEmEstoque == 0) {
            printf("Estoque zerado! Venda nao permitida.\n");

        } else if (listaDeProdutos[indiceDoProduto].quantidadeEmEstoque < quantidadeDigitada) {
            printf("Estoque insuficiente! Disponivel: %d\n",
                   listaDeProdutos[indiceDoProduto].quantidadeEmEstoque);

        } else {
            /* Tudo certo: tira do estoque e adiciona ao carrinho */
            listaDeProdutos[indiceDoProduto].quantidadeEmEstoque -= quantidadeDigitada;
            salvarProdutos();
            adicionarAoCarrinho(indiceDoProduto, quantidadeDigitada);

            printf("'%s' adicionado! Subtotal: R$ %.2f\n",
                   listaDeProdutos[indiceDoProduto].nomeProduto,
                   listaDeProdutos[indiceDoProduto].precoDeVenda * quantidadeDigitada);

            /* Avisa se chegou no estoque minimo */
            if (listaDeProdutos[indiceDoProduto].quantidadeEmEstoque <= listaDeProdutos[indiceDoProduto].estoqueMinimo) {
                printf("ATENCAO: estoque minimo atingido para '%s' (%d restantes)!\n",
                       listaDeProdutos[indiceDoProduto].nomeProduto,
                       listaDeProdutos[indiceDoProduto].quantidadeEmEstoque);
            }
        }
    }

    /* Se nada foi comprado, cancela a venda */
    if (quantidadeDeItensNoCarrinho == 0) {
        printf("Nenhum item no carrinho. Venda cancelada.\n");
        return;
    }

    /* Cria o registro da venda */
    DadosDaVenda novaVendaCriada;
    novaVendaCriada.numeroDaVenda = totalDeVendas + 1;
    novaVendaCriada.codigoDoCliente = codigoDoCliente;
    novaVendaCriada.dia = dia;
    novaVendaCriada.mes = mes;
    novaVendaCriada.ano = ano;
    novaVendaCriada.valorTotalDaVenda = valorTotalDoCarrinho;

    inserirVendaNaLista(novaVendaCriada);
    totalDeVendas++;

    /* Cria um registro para cada item do carrinho */
    for (int i = 0; i < quantidadeDeItensNoCarrinho; i++) {
        DadosDoItem item;
        item.numeroDaVenda = novaVendaCriada.numeroDaVenda;
        item.codigoDoProduto = carrinhoDeCompras[i].codigoProduto;
        strcpy(item.descricaoDoProduto, carrinhoDeCompras[i].nomeProduto);
        item.precoDeVenda = carrinhoDeCompras[i].precoDeVenda;
        item.quantidade = carrinhoDeCompras[i].quantidade;
        item.valorTotalDoItem = carrinhoDeCompras[i].precoDeVenda * carrinhoDeCompras[i].quantidade;
        item.statusPagamento = 'a'; /* Comeca em aberto */

        inserirItemNaLista(item);
        totalDeItens++;
    }

    salvarVendas();
    salvarItens();

    registrarPagamento(novaVendaCriada.numeroDaVenda, valorTotalDoCarrinho);

    /* Limpa o carrinho ao final */
    quantidadeDeItensNoCarrinho = 0;
    valorTotalDoCarrinho = 0.0;
}

/* Lista as vendas que ainda nao foram pagas e permite pagar uma delas */
void pagarVendasAbertas(void) {
    printf("\n=== VENDAS EM ABERTO ===\n");

    int encontrouAlguma = 0;
    NoDoItem *noAtual = inicioDaListaDeItens;

    while (noAtual != NULL) {
        if (noAtual->dados.statusPagamento == 'a') {
            printf("Venda %d | %-30s | R$ %.2f\n",
                   noAtual->dados.numeroDaVenda,
                   noAtual->dados.descricaoDoProduto,
                   noAtual->dados.valorTotalDoItem);
            encontrouAlguma = 1;
        }
        noAtual = noAtual->proximo;
    }

    if (!encontrouAlguma) {
        printf("Nenhuma venda em aberto.\n");
        return;
    }

    int numeroDaVenda = lerInt("Numero da venda para pagar: ");

    /* Procura o valor total dessa venda */
    float valorTotal = 0;
    NoDaVenda *noVenda = inicioDaListaDeVendas;

    while (noVenda != NULL) {
        if (noVenda->dados.numeroDaVenda == numeroDaVenda) {
            valorTotal = noVenda->dados.valorTotalDaVenda;
            break;
        }
        noVenda = noVenda->proximo;
    }

    if (valorTotal == 0) {
        printf("Venda nao encontrada!\n");
        return;
    }

    registrarPagamento(numeroDaVenda, valorTotal);
}


/* ============================================================
   OPERACOES DE CAIXA
   ============================================================ */

/* Retirada de dinheiro do caixa (sangria) */
void sangria(void) {
    if (!validarAdministrador()) {
        return;
    }

    if (!caixaDoDia.caixaEstaAberto) {
        printf("Caixa fechado!\n");
        return;
    }

    float saldoEmDinheiro = caixaDoDia.valorDeAbertura
                          + caixaDoDia.totalPagoEmDinheiro
                          + caixaDoDia.totalMistoParteDinheiro;

    printf("\n=== RETIRADA DE CAIXA (SANGRIA) ===\n");
    printf("Saldo em dinheiro no caixa: R$ %.2f\n", saldoEmDinheiro);
    printf("(Minimo obrigatorio de R$ 50,00 para troco deve permanecer)\n");

    float valorParaRetirar = lerFloat("Valor a retirar: R$ ");

    if (valorParaRetirar <= 0) {
        printf("Valor invalido!\n");

    } else if (valorParaRetirar >= (saldoEmDinheiro - 50.0f)) {
        float maximoPermitido = (saldoEmDinheiro - 50.0f) > 0 ? (saldoEmDinheiro - 50.0f) : 0.0f;

        printf("ATENCAO: Nao e permitido retirar este valor.\n");
        printf("Deve sobrar ao menos R$ 50,00 para troco.\n");
        printf("Maximo permitido para retirada: R$ %.2f\n", maximoPermitido);

    } else {
        caixaDoDia.valorDeAbertura -= valorParaRetirar;
        salvarCaixa();
        printf("Sangria de R$ %.2f realizada com sucesso!\n", valorParaRetirar);
        printf("Saldo restante em dinheiro: R$ %.2f\n", saldoEmDinheiro - valorParaRetirar);
    }
}

/* Abre o caixa do dia informando o troco inicial */
void abrirCaixa(void) {
    if (!validarAdministrador()) {
        return;
    }

    if (caixaDoDia.caixaEstaAberto) {
        printf("Caixa ja esta aberto! Saldo: R$ %.2f\n",
               caixaDoDia.valorDeAbertura + caixaDoDia.faturamentoTotal);
        return;
    }

    float trocoInicial;
    do {
        trocoInicial = lerFloat("Informe o troco inicial: R$ ");
        if (trocoInicial < 0) {
            printf("Valor invalido!\n");
        }
    } while (trocoInicial < 0);

    caixaDoDia = (DadosDoCaixa){0};
    caixaDoDia.valorDeAbertura = trocoInicial;
    caixaDoDia.caixaEstaAberto = 1;
    salvarCaixa();

    printf("Caixa aberto com R$ %.2f.\n", trocoInicial);
}

/* Fecha o caixa do dia e mostra o resumo financeiro */
void fecharCaixa(void) {
    if (!validarAdministrador()) {
        return;
    }

    if (!caixaDoDia.caixaEstaAberto) {
        printf("Caixa ja esta fechado!\n");
        return;
    }

    if (quantidadeDeItensNoCarrinho > 0) {
        printf("Ha itens no carrinho! Finalize o pagamento antes de fechar.\n");
        return;
    }

    printf("\n=== FECHAMENTO DE CAIXA ===\n");
    printf("-------------------------------\n");
    printf("Qtd de vendas realizadas : %d\n", totalDeVendas);
    printf("Total faturado : R$ %10.2f\n", caixaDoDia.faturamentoTotal);
    printf("Valor de abertura : R$ %10.2f\n", caixaDoDia.valorDeAbertura);
    printf("Pagamentos em dinheiro : R$ %10.2f\n", caixaDoDia.totalPagoEmDinheiro);
    printf("Pagamentos em cartao : R$ %10.2f\n", caixaDoDia.totalPagoEmCartao);
    printf("Misto (parte dinheiro) : R$ %10.2f\n", caixaDoDia.totalMistoParteDinheiro);
    printf("Misto (parte cartao) : R$ %10.2f\n", caixaDoDia.totalMistoParteCartao);
    printf("-------------------------------\n");

    float resultado = caixaDoDia.faturamentoTotal
                    - caixaDoDia.totalPagoEmDinheiro
                    - caixaDoDia.totalPagoEmCartao
                    - caixaDoDia.totalMistoParteDinheiro
                    - caixaDoDia.totalMistoParteCartao;

    caixaDoDia.valorDeAjuste = resultado;

    if (resultado != 0.0f) {
        printf("\nATENCAO: Divergencia de valores.\n");

        if (resultado > 0) {
            printf("Sobra de R$ %.2f.\n", resultado);
        } else {
            printf("Falta de R$ %.2f.\n", -resultado);
        }

        printf("Ajuste necessario antes de fechar.\n");

    } else {
        caixaDoDia.caixaEstaAberto = 0;
        salvarCaixa();
        printf("\nCaixa fechado com sucesso!\n");
    }
}


/* ============================================================
   RELATORIOS
   ============================================================ */

/* Verifica se uma data (dia/mes/ano) esta dentro de um periodo */
int dataEstaDentroDoPeriodo(int dia, int mes, int ano,
                            int diaInicial, int mesInicial, int anoInicial,
                            int diaFinal, int mesFinal, int anoFinal) {

    int depoisDoInicio =
        (ano > anoInicial) ||
        (ano == anoInicial && mes > mesInicial) ||
        (ano == anoInicial && mes == mesInicial && dia >= diaInicial);

    int antesDoFim =
        (ano < anoFinal) ||
        (ano == anoFinal && mes < mesFinal) ||
        (ano == anoFinal && mes == mesFinal && dia <= diaFinal);

    return depoisDoInicio && antesDoFim;
}

/* Relatorio: clientes em ordem alfabetica */
void relatorioClientesAlfabetica(void) {
    if (quantidadeDeClientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    /* Faz uma copia para ordenar sem baguncar a lista original */
    Cliente *copiaClientes = malloc(quantidadeDeClientes * sizeof(Cliente));
    memcpy(copiaClientes, listaDeClientes, quantidadeDeClientes * sizeof(Cliente));

    /* Ordena por nome usando bubble sort */
    int houveTroca;
    do {
        houveTroca = 0;
        for (int i = 0; i < quantidadeDeClientes - 1; i++) {
            if (strcmp(copiaClientes[i].nomeCompleto, copiaClientes[i + 1].nomeCompleto) > 0) {
                Cliente auxiliar = copiaClientes[i];
                copiaClientes[i] = copiaClientes[i + 1];
                copiaClientes[i + 1] = auxiliar;
                houveTroca = 1;
            }
        }
    } while (houveTroca);

    printf("\n=== CLIENTES - ORDEM ALFABETICA ===\n");
    printf("%-5s %-30s %-15s %-20s\n", "COD", "NOME", "CPF", "CELULAR");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < quantidadeDeClientes; i++) {
        printf("%-5d %-30s %-15s %-20s\n",
               copiaClientes[i].codigoCliente,
               copiaClientes[i].nomeCompleto,
               copiaClientes[i].cpf,
               copiaClientes[i].telefone);
    }

    free(copiaClientes);
}

/* Relatorio: clientes que compraram em um periodo */
void relatorioClientesQueCompraram(void) {
    int diaInicial, mesInicial, anoInicial;
    int diaFinal, mesFinal, anoFinal;

    printf("\n=== CLIENTES QUE COMPRARAM ===\n");

    printf("Periodo inicial (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &diaInicial, &mesInicial, &anoInicial);
    while (getchar() != '\n');

    printf("Periodo final (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &diaFinal, &mesFinal, &anoFinal);
    while (getchar() != '\n');

    printf("\n%-5s %-30s %-12s\n", "COD", "CLIENTE", "DATA");
    printf("--------------------------------------------------\n");

    NoDaVenda *noVenda = inicioDaListaDeVendas;
    while (noVenda != NULL) {

        int dentroDoPeriodo = dataEstaDentroDoPeriodo(
            noVenda->dados.dia, noVenda->dados.mes, noVenda->dados.ano,
            diaInicial, mesInicial, anoInicial,
            diaFinal, mesFinal, anoFinal);

        if (dentroDoPeriodo) {
            for (int i = 0; i < quantidadeDeClientes; i++) {
                if (listaDeClientes[i].codigoCliente == noVenda->dados.codigoDoCliente) {
                    printf("%-5d %-30s %02d/%02d/%04d\n",
                           listaDeClientes[i].codigoCliente,
                           listaDeClientes[i].nomeCompleto,
                           noVenda->dados.dia,
                           noVenda->dados.mes,
                           noVenda->dados.ano);
                    break;
                }
            }
        }

        noVenda = noVenda->proximo;
    }
}

/* Relatorio: produtos em ordem alfabetica */
void relatorioProdutosAlfabetica(void) {
    if (quantidadeDeProdutos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    /* Faz uma copia para ordenar sem baguncar a lista original */
    Produto *copiaProdutos = malloc(quantidadeDeProdutos * sizeof(Produto));
    memcpy(copiaProdutos, listaDeProdutos, quantidadeDeProdutos * sizeof(Produto));

    /* Ordena por nome usando bubble sort */
    int houveTroca;
    do {
        houveTroca = 0;
        for (int i = 0; i < quantidadeDeProdutos - 1; i++) {
            if (strcmp(copiaProdutos[i].nomeProduto, copiaProdutos[i + 1].nomeProduto) > 0) {
                Produto auxiliar = copiaProdutos[i];
                copiaProdutos[i] = copiaProdutos[i + 1];
                copiaProdutos[i + 1] = auxiliar;
                houveTroca = 1;
            }
        }
    } while (houveTroca);

    printf("\n=== PRODUTOS - ORDEM ALFABETICA ===\n");
    printf("%-6s %-30s %-15s %10s %6s\n", "COD", "DESCRICAO", "CATEGORIA", "VL.VENDA", "ESTQ");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < quantidadeDeProdutos; i++) {
        char *nomeCategoria;

        if (copiaProdutos[i].categoria == 1) {
            nomeCategoria = "Alimentos";
        } else if (copiaProdutos[i].categoria == 2) {
            nomeCategoria = "Limpeza";
        } else {
            nomeCategoria = "Panificadora";
        }

        printf("%-6d %-30s %-15s R$%8.2f %6d\n",
               copiaProdutos[i].codigoProduto,
               copiaProdutos[i].nomeProduto,
               nomeCategoria,
               copiaProdutos[i].precoDeVenda,
               copiaProdutos[i].quantidadeEmEstoque);
    }

    free(copiaProdutos);
}

/* Relatorio: produtos com estoque critico (no minimo ou abaixo) */
void relatorioEstoqueMinimo(void) {
    printf("\n=== PRODUTOS COM ESTOQUE CRITICO ===\n");
    printf("%-6s %-30s %6s %6s\n", "COD", "DESCRICAO", "ESTQ", "MIN");
    printf("--------------------------------------------------\n");

    int achouAlgum = 0;
    for (int i = 0; i < quantidadeDeProdutos; i++) {
        if (listaDeProdutos[i].quantidadeEmEstoque <= listaDeProdutos[i].estoqueMinimo) {
            printf("%-6d %-30s %6d %6d\n",
                   listaDeProdutos[i].codigoProduto,
                   listaDeProdutos[i].nomeProduto,
                   listaDeProdutos[i].quantidadeEmEstoque,
                   listaDeProdutos[i].estoqueMinimo);
            achouAlgum = 1;
        }
    }

    if (!achouAlgum) {
        printf("Nenhum produto com estoque critico.\n");
    }
}

/* Relatorio: vendas realizadas em um periodo */
void relatorioVendasPeriodo(void) {
    int diaInicial, mesInicial, anoInicial;
    int diaFinal, mesFinal, anoFinal;

    printf("\n=== VENDAS POR PERIODO ===\n");

    printf("Periodo inicial (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &diaInicial, &mesInicial, &anoInicial);
    while (getchar() != '\n');

    printf("Periodo final (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &diaFinal, &mesFinal, &anoFinal);
    while (getchar() != '\n');

    printf("\n%-8s %-12s %12s\n", "N.VENDA", "DATA", "TOTAL");
    printf("----------------------------------\n");

    NoDaVenda *noVenda = inicioDaListaDeVendas;
    while (noVenda != NULL) {

        int dentroDoPeriodo = dataEstaDentroDoPeriodo(
            noVenda->dados.dia, noVenda->dados.mes, noVenda->dados.ano,
            diaInicial, mesInicial, anoInicial,
            diaFinal, mesFinal, anoFinal);

        if (dentroDoPeriodo) {
            printf("%-8d %02d/%02d/%04d R$%10.2f\n",
                   noVenda->dados.numeroDaVenda,
                   noVenda->dados.dia,
                   noVenda->dados.mes,
                   noVenda->dados.ano,
                   noVenda->dados.valorTotalDaVenda);
        }

        noVenda = noVenda->proximo;
    }
}

/* Relatorio: faturamento total separado por dinheiro e cartao */
void relatorioFaturamentoConsolidado(void) {
    int diaInicial, mesInicial, anoInicial;
    int diaFinal, mesFinal, anoFinal;
    float totalEmDinheiro = 0;
    float totalEmCartao = 0;
    float totalGeral = 0;

    printf("\n=== FATURAMENTO CONSOLIDADO ===\n");

    printf("Periodo inicial (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &diaInicial, &mesInicial, &anoInicial);
    while (getchar() != '\n');

    printf("Periodo final (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &diaFinal, &mesFinal, &anoFinal);
    while (getchar() != '\n');

    NoDaVenda *noVenda = inicioDaListaDeVendas;
    while (noVenda != NULL) {

        int dentroDoPeriodo = dataEstaDentroDoPeriodo(
            noVenda->dados.dia, noVenda->dados.mes, noVenda->dados.ano,
            diaInicial, mesInicial, anoInicial,
            diaFinal, mesFinal, anoFinal);

        if (dentroDoPeriodo) {
            totalGeral += noVenda->dados.valorTotalDaVenda;

            /* Soma os pagamentos dessa venda separando dinheiro e cartao */
            NoDoPagamento *noPagamento = inicioDaListaDePagamentos;
            while (noPagamento != NULL) {

                if (noPagamento->dados.numeroDaVenda == noVenda->dados.numeroDaVenda) {
                    int ehDinheiro = (noPagamento->dados.formaDePagamento[0] == 'd') ||
                                     (strcmp(noPagamento->dados.formaDePagamento, "md") == 0);

                    if (ehDinheiro) {
                        totalEmDinheiro += noPagamento->dados.valorPago;
                    } else {
                        totalEmCartao += noPagamento->dados.valorPago;
                    }
                }

                noPagamento = noPagamento->proximo;
            }
        }

        noVenda = noVenda->proximo;
    }

    printf("-------------------------------\n");
    printf("Total em dinheiro : R$ %.2f\n", totalEmDinheiro);
    printf("Total em cartao : R$ %.2f\n", totalEmCartao);
    printf("Total geral : R$ %.2f\n", totalGeral);
}

void relatorioProdutosMaisVendidos(void) {
    int diaInicial, mesInicial, anoInicial;
    int diaFinal, mesFinal, anoFinal;

    printf("\n=== PRODUTOS MAIS VENDIDOS POR PERIODO ===\n");

    printf("Periodo inicial (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &diaInicial, &mesInicial, &anoInicial);
    while (getchar() != '\n');

    printf("Periodo final (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &diaFinal, &mesFinal, &anoFinal);
    while (getchar() != '\n');

    if (quantidadeDeProdutos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    int *quantidadeVendidaPorProduto = calloc(quantidadeDeProdutos, sizeof(int));
    float *valorVendidoPorProduto = calloc(quantidadeDeProdutos, sizeof(float));
    int *ordemDosProdutos = malloc(quantidadeDeProdutos * sizeof(int));

    if (quantidadeVendidaPorProduto == NULL ||
        valorVendidoPorProduto == NULL ||
        ordemDosProdutos == NULL) {
        printf("Erro de memoria!\n");
        free(quantidadeVendidaPorProduto);
        free(valorVendidoPorProduto);
        free(ordemDosProdutos);
        return;
    }

    for (int i = 0; i < quantidadeDeProdutos; i++) {
        ordemDosProdutos[i] = i;
    }

    NoDaVenda *noVenda = inicioDaListaDeVendas;
    while (noVenda != NULL) {

        int dentroDoPeriodo = dataEstaDentroDoPeriodo(
            noVenda->dados.dia, noVenda->dados.mes, noVenda->dados.ano,
            diaInicial, mesInicial, anoInicial,
            diaFinal, mesFinal, anoFinal);

        if (dentroDoPeriodo) {
            NoDoItem *noItem = inicioDaListaDeItens;
            while (noItem != NULL) {

                if (noItem->dados.numeroDaVenda == noVenda->dados.numeroDaVenda) {
                    for (int i = 0; i < quantidadeDeProdutos; i++) {
                        if (listaDeProdutos[i].codigoProduto == noItem->dados.codigoDoProduto) {
                            quantidadeVendidaPorProduto[i] += noItem->dados.quantidade;
                            valorVendidoPorProduto[i] += noItem->dados.valorTotalDoItem;
                            break;
                        }
                    }
                }

                noItem = noItem->proximo;
            }
        }

        noVenda = noVenda->proximo;
    }

    /* Ordena os produtos do mais vendido para o menos vendido */
    int houveTroca;
    do {
        houveTroca = 0;
        for (int i = 0; i < quantidadeDeProdutos - 1; i++) {
            if (quantidadeVendidaPorProduto[ordemDosProdutos[i]] <
                quantidadeVendidaPorProduto[ordemDosProdutos[i + 1]]) {

                int temporario = ordemDosProdutos[i];
                ordemDosProdutos[i] = ordemDosProdutos[i + 1];
                ordemDosProdutos[i + 1] = temporario;
                houveTroca = 1;
            }
        }
    } while (houveTroca);

    printf("\n%-4s %-30s %-15s %8s %12s\n",
           "POS", "PRODUTO", "CATEGORIA", "QTD.VND", "TOTAL.VND");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < quantidadeDeProdutos; i++) {
        int indiceProduto = ordemDosProdutos[i];

        /* Pula produtos que nao venderam nada */
        if (quantidadeVendidaPorProduto[indiceProduto] == 0) {
            continue;
        }

        char *nomeCategoria;
        if (listaDeProdutos[indiceProduto].categoria == 1) {
            nomeCategoria = "Alimentos";
        } else if (listaDeProdutos[indiceProduto].categoria == 2) {
            nomeCategoria = "Limpeza";
        } else {
            nomeCategoria = "Panificadora";
        }

        printf("%-4d %-30s %-15s %8d R$%10.2f\n",
               i + 1,
               listaDeProdutos[indiceProduto].nomeProduto,
               nomeCategoria,
               quantidadeVendidaPorProduto[indiceProduto],
               valorVendidoPorProduto[indiceProduto]);
    }

    free(quantidadeVendidaPorProduto);
    free(valorVendidoPorProduto);
    free(ordemDosProdutos);
}

/* ============================================================
   MENUS
   ============================================================ */

/* Menu de relatorios */
void menuRelatorios(void) {
    int opcao;

    printf("\n===== Relatorios =====\n");
    printf("1 - Clientes - Ordem Alfabetica\n");
    printf("2 - Clientes que Compraram (periodo)\n");
    printf("3 - Produtos - Ordem Alfabetica\n");
    printf("4 - Produtos com Estoque Critico\n");
    printf("5 - Produtos mais Vendidos (periodo)\n");
    printf("6 - Vendas por Periodo\n");
    printf("7 - Faturamento Consolidado\n");
    printf("0 - Voltar\n");
    opcao = lerInt("Escolha: ");

    switch (opcao) {
        case 1: relatorioClientesAlfabetica(); break;
        case 2: relatorioClientesQueCompraram(); break;
        case 3: relatorioProdutosAlfabetica(); break;
        case 4: relatorioEstoqueMinimo(); break;
        case 5: relatorioProdutosMaisVendidos(); break;
        case 6: relatorioVendasPeriodo(); break;
        case 7: relatorioFaturamentoConsolidado(); break;
        case 0: break;
        default: printf("Opcao invalida!\n");
    }
}

/* Menu de cadastros */
void menuCadastro(void) {
    int opcao;

    printf("\n===== Cadastros =====\n");
    printf("1 - Cadastrar usuario\n");
    printf("2 - Cadastrar cliente\n");
    printf("3 - Cadastrar produto\n");
    printf("4 - Listar clientes\n");
    printf("5 - Listar produtos\n");
    printf("6 - Voltar\n");
    opcao = lerInt("Escolha: ");

    switch (opcao) {
        case 1:
            if (validarAdministrador()) {
                cadastrarUsuario();
            }
            break;
        case 2: cadastrarCliente(); break;
        case 3: cadastrarProduto(); break;
        case 4: listarClientes(); break;
        case 5: listarProdutos(); break;
        case 6: break;
        default: printf("Opcao invalida!\n");
    }
}

/* Menu de vendas */
void menuVendas(void) {
    int opcao;

    printf("\n===== Menu de Vendas =====\n");
    printf("1 - Nova venda\n");
    printf("2 - Retirada de caixa (SANGRIA)\n");
    printf("3 - Pagar vendas em aberto\n");
    printf("4 - Voltar\n");
    opcao = lerInt("Escolha: ");

    switch (opcao) {
        case 1: novaVenda(); break;
        case 2: sangria(); break;
        case 3: pagarVendasAbertas(); break;
        case 4: break;
        default: printf("Opcao invalida!\n");
    }
}

/* ============================================================
   INICIALIZACAO E PROGRAMA PRINCIPAL
   ============================================================ */

/* Carrega todos os dados dos arquivos ao iniciar o programa */
void inicializar(void) {
    carregarUsuarios();
    carregarClientes();
    carregarProdutos();
    carregarVendas();
    carregarItens();
    carregarPagamentos();
    carregarCaixa();
}

int main(void) {
    int opcao;

    inicializar();
    tipoDoUsuarioLogado = 2; /* Comeca como usuario comum */

    do {
        printf("\n===== Mercearia BERE =====\n");
        printf("1 - Cadastros\n");
        printf("2 - Vendas\n");
        printf("3 - Abertura de caixa\n");
        printf("4 - Fechamento de caixa\n");
        printf("5 - Relatorios\n");
        printf("6 - Sair\n");
        opcao = lerInt("Escolha: ");

        switch (opcao) {
            case 1: menuCadastro(); break;
            case 2: menuVendas(); break;
            case 3: abrirCaixa(); break;
            case 4: fecharCaixa(); break;
            case 5: menuRelatorios(); break;
            case 6: printf("Encerrando o sistema. Ate logo!\n"); break;
            default: printf("Opcao invalida!\n");
        }

    } while (opcao != 6);

    /* Libera a memoria dos vetores */
    free(listaDeClientes);
    free(listaDeProdutos);
    free(carrinhoDeCompras);

    /* Libera a memoria das listas encadeadas */
    NoDaVenda *vendaParaLiberar;
    while (inicioDaListaDeVendas != NULL) {
        vendaParaLiberar = inicioDaListaDeVendas->proximo;
        free(inicioDaListaDeVendas);
        inicioDaListaDeVendas = vendaParaLiberar;
    }

    NoDoItem *itemParaLiberar;
    while (inicioDaListaDeItens != NULL) {
        itemParaLiberar = inicioDaListaDeItens->proximo;
        free(inicioDaListaDeItens);
        inicioDaListaDeItens = itemParaLiberar;
    }

    NoDoPagamento *pagamentoParaLiberar;
    while (inicioDaListaDePagamentos != NULL) {
        pagamentoParaLiberar = inicioDaListaDePagamentos->proximo;
        free(inicioDaListaDePagamentos);
        inicioDaListaDePagamentos = pagamentoParaLiberar;
    }

    NoDoUsuario *usuarioParaLiberar;
    while (inicioDaListaDeUsuarios != NULL) {
        usuarioParaLiberar = inicioDaListaDeUsuarios->proximo;
        free(inicioDaListaDeUsuarios);
        inicioDaListaDeUsuarios = usuarioParaLiberar;
    }

    return 0;
}