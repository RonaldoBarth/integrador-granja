#include <stdio.h>   // biblioteca para entrada e saida (printf, scanf, etc)
#include <stdlib.h>  // biblioteca para alocacao de memoria (malloc, realloc, free)
#include <string.h>  // biblioteca para manipulacao de strings (strcpy, strcmp, etc)

// ===================== STRUCTS =====================
// Structs sao estruturas que agrupam varios dados diferentes em um so lugar

// Estrutura que guarda os dados de um cliente
typedef struct {
    int codigo;           // numero unico que identifica o cliente
    char nome[100];       // nome completo do cliente (ate 100 caracteres)
    char nomeSocial[100]; // nome social do cliente (respeito a diversidade)
    char cpf[15];         // CPF do cliente (ex: 000.000.000-00)
    char rua[100];        // endereco: rua e numero
    char bairro[60];      // bairro onde mora
    char celular[20];     // numero de celular/whatsapp
} Cliente;

// Estrutura que guarda os dados de um produto
typedef struct {
    int codigo;          // numero unico que identifica o produto
    char descricao[100]; // nome/descricao do produto
    int categoria;       // 1=Alimento, 2=Material de Limpeza, 3=Panificacao
    float precoCompra;   // quanto custou para comprar o produto
    float margem;        // percentual de lucro sobre o preco de compra
    int estoque;         // quantas unidades tem disponivel
    int estoqueMin;      // quantidade minima antes de emitir alerta
} Produto;

// Estrutura que guarda os dados de um item de venda
typedef struct {
    int numVenda;        // numero da venda a qual esse item pertence
    int codProduto;      // codigo do produto vendido
    char descricao[100]; // nome do produto (copiado do cadastro)
    float precoVenda;    // preco calculado com margem de lucro
    int quantidade;      // quantas unidades foram vendidas
    float totalItem;     // preco de venda multiplicado pela quantidade
    char pagamento;      // 'a' = em aberto (nao pago), 'p' = pago
} Venda;

// Estrutura que guarda os dados de um pagamento realizado
typedef struct {
    int numVenda;    // numero da venda que foi paga
    float valorPago; // valor total que foi pago
    char tipo[3];    // "d"=dinheiro, "c"=cartao, "dc"=dinheiro e cartao
} Pagamento;

// ===================== VARIAVEIS GLOBAIS =====================
// Variaveis globais sao acessiveis em qualquer funcao do programa

Cliente *clientes = NULL;   // ponteiro para o vetor dinamico de clientes (comeca vazio)
int totalClientes = 0;      // quantos clientes foram cadastrados ate agora

Produto *produtos = NULL;   // ponteiro para o vetor dinamico de produtos (comeca vazio)
int totalProdutos = 0;      // quantos produtos foram cadastrados ate agora

Venda *vendas = NULL;       // ponteiro para o vetor dinamico de vendas (comeca vazio)
int totalVendas = 0;        // quantos itens de venda foram registrados ate agora

Pagamento *pagamentos = NULL; // ponteiro para o vetor dinamico de pagamentos (comeca vazio)
int totalPagamentos = 0;      // quantos pagamentos foram registrados ate agora

float valorAbertura = -1;  // valor inicial do caixa; -1 significa que o caixa esta fechado
int proximoNumVenda = 1;   // numero sequencial para identificar cada venda

// ===================== FUNCOES DE LEITURA =====================

/* Le um numero inteiro do teclado com validacao de entrada */
int lerInt(const char *msg) {
    int val;              // variavel que vai guardar o numero lido
    printf("%s", msg);   // exibe a mensagem pedindo o valor
    while (scanf("%d", &val) != 1) {   // tenta ler um inteiro; se falhar entra no loop
        printf("Entrada invalida! Digite um numero: "); // avisa que o dado e invalido
        while (getchar() != '\n');  // limpa o que sobrou no buffer do teclado
    }
    while (getchar() != '\n');  // limpa o enter que sobrou apos a leitura valida
    return val;  // retorna o numero lido
}

/* Le um numero decimal (float) do teclado com validacao de entrada */
float lerFloat(const char *msg) {
    float val;            // variavel que vai guardar o numero decimal lido
    printf("%s", msg);   // exibe a mensagem pedindo o valor
    while (scanf("%f", &val) != 1) {   // tenta ler um float; se falhar entra no loop
        printf("Entrada invalida! Digite um numero: "); // avisa que o dado e invalido
        while (getchar() != '\n');  // limpa o buffer do teclado
    }
    while (getchar() != '\n');  // limpa o enter que sobrou
    return val;  // retorna o numero decimal lido
}

/* Le um texto (string) do teclado */
void lerString(const char *msg, char *dest, int tam) {
    printf("%s", msg);        // exibe a mensagem pedindo o texto
    fgets(dest, tam, stdin);  // le o texto incluindo espacos, limitando pelo tamanho tam
    dest[strcspn(dest, "\n")] = '\0'; // remove o '\n' que o fgets deixa no final da string
}

// ===================== FUNCOES DE CADASTRO =====================

/* Cadastra um novo cliente na lista dinamica */
void cadastrarCliente() {
    // realloc aumenta o espaco do vetor para caber mais um cliente
    clientes = realloc(clientes, (totalClientes + 1) * sizeof(Cliente));

    Cliente *c = &clientes[totalClientes]; // ponteiro apontando para o novo cliente (ultimo da lista)

    c->codigo = totalClientes + 1; // gera o codigo automaticamente (1, 2, 3...)
    printf("\n--- Cadastro de Cliente (Codigo: %d) ---\n", c->codigo); // mostra o codigo gerado

    lerString("Nome completo: ", c->nome, 100);         // le o nome completo
    lerString("Nome social: ", c->nomeSocial, 100);     // le o nome social
    lerString("CPF: ", c->cpf, 15);                     // le o CPF
    lerString("Rua/Numero: ", c->rua, 100);             // le o endereco
    lerString("Bairro: ", c->bairro, 60);               // le o bairro
    lerString("Celular/WhatsApp: ", c->celular, 20);    // le o celular

    totalClientes++; // incrementa o contador de clientes cadastrados
    printf("Cliente cadastrado com sucesso!\n"); // confirma o cadastro
}

/* Cadastra um novo produto na lista dinamica */
void cadastrarProduto() {
    // realloc aumenta o espaco do vetor para caber mais um produto
    produtos = realloc(produtos, (totalProdutos + 1) * sizeof(Produto));

    Produto *p = &produtos[totalProdutos]; // ponteiro apontando para o novo produto

    p->codigo = 1000 + totalProdutos; // codigos de produto comecam em 1000 (1000, 1001, 1002...)
    printf("\n--- Cadastro de Produto (Codigo: %d) ---\n", p->codigo); // mostra o codigo gerado

    lerString("Descricao: ", p->descricao, 100); // le a descricao do produto

    // exibe as opcoes de categoria para o usuario escolher
    printf("Categoria:\n 1 - Alimento\n 2 - Material de Limpeza\n 3 - Panificacao\n");
    p->categoria = lerInt("Escolha: "); // le e guarda a categoria escolhida

    p->precoCompra = lerFloat("Preco de compra: R$ "); // le o preco de compra
    p->margem = lerFloat("Margem de lucro (%%): ");    // le a margem de lucro em percentual
    p->estoque = lerInt("Quantidade em estoque: ");    // le a quantidade disponivel
    p->estoqueMin = lerInt("Estoque minimo: ");        // le o limite minimo de alerta

    totalProdutos++; // incrementa o contador de produtos cadastrados
    printf("Produto cadastrado com sucesso!\n"); // confirma o cadastro
}

/* Exibe o menu de cadastros e chama as funcoes corretas */
void menuCadastros() {
    int opc; // variavel para guardar a opcao escolhida pelo usuario
    do {
        // exibe as opcoes do submenu de cadastros
        printf("\n---- CADASTROS ----\n");
        printf("1 - Cadastro de Clientes\n");
        printf("2 - Cadastro de Produtos\n");
        printf("3 - Retornar ao Menu Principal\n");
        opc = lerInt("Escolha: "); // le a opcao

        // executa a funcao correspondente a opcao escolhida
        switch (opc) {
            case 1: cadastrarCliente(); break; // chama o cadastro de cliente
            case 2: cadastrarProduto(); break; // chama o cadastro de produto
            case 3: printf("Voltando ao menu principal...\n"); break; // sai do submenu
            default: printf("Opcao invalida!\n"); break; // opcao nao existe
        }
    } while (opc != 3); // repete ate o usuario escolher voltar
}

// ===================== FUNCOES DE VENDA =====================

/* Busca um produto pelo seu codigo e retorna um ponteiro para ele */
Produto *buscarProduto(int cod) {
    int i; // variavel de controle do loop
    for (i = 0; i < totalProdutos; i++) {          // percorre todos os produtos cadastrados
        if (produtos[i].codigo == cod)              // compara o codigo de cada produto com o buscado
            return &produtos[i];                   // se encontrou, retorna o endereco do produto
    }
    return NULL; // se nao encontrou nenhum produto com esse codigo, retorna NULL
}

/* Calcula o preco de venda de um produto aplicando a margem de lucro */
float calcularPrecoVenda(Produto *p) {
    // formula: preco de compra + (preco de compra * margem / 100)
    return p->precoCompra * (1.0 + p->margem / 100.0);
}

/* Exibe a tabela de todos os produtos disponiveis para venda */
void listarProdutosVenda() {
    int i; // variavel de controle do loop
    // cabecalho da tabela com formatacao de colunas
    printf("\n%-6s %-30s %-15s %-12s %-8s\n", "Cod", "Descricao", "Categoria", "Preco Venda", "Estoque");
    printf("---------------------------------------------------------------------\n");

    for (i = 0; i < totalProdutos; i++) { // percorre todos os produtos
        char cat[20]; // variavel local para guardar o nome da categoria

        // converte o numero da categoria para texto
        if (produtos[i].categoria == 1) strcpy(cat, "Alimento");
        else if (produtos[i].categoria == 2) strcpy(cat, "Mat. Limpeza");
        else strcpy(cat, "Panificacao");

        // exibe uma linha da tabela com os dados do produto
        printf("%-6d %-30s %-15s R$%-10.2f %-8d\n",
            produtos[i].codigo,
            produtos[i].descricao,
            cat,
            calcularPrecoVenda(&produtos[i]), // calcula o preco de venda na hora
            produtos[i].estoque);
    }
}

/* Realiza uma nova venda: monta o carrinho, aplica desconto e registra o pagamento */
void novaVenda() {
    if (totalProdutos == 0) { // verifica se ha produtos cadastrados antes de vender
        printf("Nenhum produto cadastrado!\n");
        return; // encerra a funcao se nao houver produtos
    }

    float totalCarrinho = 0; // acumula o valor total dos itens do carrinho
    char continuar = 's';    // controla se o usuario quer adicionar mais itens
    int numVenda = proximoNumVenda; // numero desta venda

    printf("\n--- Nova Venda (Num: %d) ---\n", numVenda);

    // loop que continua adicionando itens enquanto o usuario quiser
    while (continuar == 's' || continuar == 'S') {
        listarProdutosVenda(); // mostra a tabela de produtos disponíveis

        int cod = lerInt("\nInforme o codigo do produto: "); // le o codigo do produto desejado
        Produto *p = buscarProduto(cod); // busca o produto pelo codigo

        if (p == NULL) { // se o produto nao foi encontrado
            printf("Produto nao encontrado!\n");
            continue; // volta para o inicio do loop sem adicionar nada
        }

        if (p->estoque == 0) { // verifica se o produto esta sem estoque
            printf("ALERTA: Produto sem estoque! Nao e possivel vender.\n");
            continue; // volta para o inicio do loop
        }

        int qtd = lerInt("Informe a quantidade: "); // le a quantidade desejada pelo cliente

        if (qtd > p->estoque) { // verifica se a quantidade pedida e maior que o estoque
            // avisa que nao tem estoque suficiente e pergunta se continua mesmo assim
            printf("ALERTA: Voce deseja vender %d produto(s). Ha somente %d em estoque! Continuar (s/n)? ", qtd, p->estoque);
            char resp;
            scanf(" %c", &resp); // le a resposta s ou n
            while (getchar() != '\n'); // limpa o buffer
            if (resp != 's' && resp != 'S') continue; // se nao quiser continuar, volta ao loop
            qtd = p->estoque; // ajusta a quantidade para o maximo disponivel em estoque
        }

        float preco = calcularPrecoVenda(p); // calcula o preco de venda com a margem
        float totalItem = preco * qtd;       // calcula o valor total desse item

        // aumenta o vetor de vendas para caber mais um item
        vendas = realloc(vendas, (totalVendas + 1) * sizeof(Venda));
        Venda *v = &vendas[totalVendas]; // ponteiro para o novo item de venda

        v->numVenda = numVenda;          // registra o numero da venda
        v->codProduto = p->codigo;       // registra o codigo do produto
        strcpy(v->descricao, p->descricao); // copia a descricao do produto para a venda
        v->precoVenda = preco;           // registra o preco de venda calculado
        v->quantidade = qtd;             // registra a quantidade vendida
        v->totalItem = totalItem;        // registra o valor total do item
        v->pagamento = 'a';             // marca como 'a' = aberto (ainda nao pago)

        totalVendas++; // incrementa o contador de itens de venda
        totalCarrinho += totalItem; // soma o valor do item ao total do carrinho

        p->estoque -= qtd; // desconta a quantidade vendida do estoque do produto

        // verifica se o estoque ficou no minimo ou abaixo dele
        if (p->estoque <= p->estoqueMin)
            printf("ALERTA: Produto '%s' atingiu o estoque minimo! (%d restantes)\n", p->descricao, p->estoque);

        printf("Item adicionado! Total atual: R$ %.2f\n", totalCarrinho); // mostra o total parcial

        printf("Novo item no carrinho de compra (s/n)? ");
        scanf(" %c", &continuar); // le se o usuario quer adicionar mais itens
        while (getchar() != '\n'); // limpa o buffer
    }

    if (totalCarrinho == 0) { // se nenhum item foi adicionado, cancela a venda
        printf("Nenhum item adicionado. Venda cancelada.\n");
        return;
    }

    // exibe o resumo do carrinho com todos os itens adicionados
    printf("\n---- CARRINHO ----\n");
    printf("%-6s %-30s %-12s %-6s %-10s\n", "Cod", "Descricao", "Preco", "Qtd", "Total");
    int i;
    for (i = 0; i < totalVendas; i++) {                 // percorre todos os itens de venda
        if (vendas[i].numVenda == numVenda)              // filtra apenas os itens desta venda
            printf("%-6d %-30s R$%-10.2f %-6d R$%.2f\n",
                vendas[i].codProduto, vendas[i].descricao,
                vendas[i].precoVenda, vendas[i].quantidade, vendas[i].totalItem);
    }
    printf("Total carrinho: R$ %.2f\n", totalCarrinho); // exibe o total geral do carrinho

    // le o percentual de desconto; se for 0, nao aplica desconto
    float desconto = lerFloat("Ha desconto (informe 0 para nao ou % concedido): ");
    float totalFinal = totalCarrinho - (totalCarrinho * desconto / 100.0); // calcula o total com desconto
    printf("Total final: R$ %.2f\n", totalFinal); // exibe o total apos desconto

    // exibe as formas de pagamento disponiveis
    printf("\n1 - Pagamento no cartao\n2 - Pagamento em dinheiro\n3 - Retornar ao Menu Principal\n");
    int formaPag = lerInt("Informe a opcao: "); // le a forma de pagamento escolhida

    Pagamento pg;          // cria uma variavel local para guardar os dados do pagamento
    pg.numVenda = numVenda; // associa o pagamento ao numero da venda
    pg.valorPago = totalFinal; // guarda o valor total a ser pago

    if (formaPag == 1) { // pagamento somente no cartao
        int ok;
        do {
            printf("(1) Pagamento na maquinha OK  (0) Nao OK: ");
            ok = lerInt(""); // le o resultado do pagamento na maquinha
            if (ok == 0) printf("Erro, tente novamente.\n"); // pagamento recusado, tenta de novo
        } while (ok == 0); // repete enquanto o pagamento nao for aprovado
        strcpy(pg.tipo, "c"); // registra o tipo como 'c' = cartao
        printf("Pagamento no cartao efetuado!\n");

    } else if (formaPag == 2) { // pagamento em dinheiro
        float dinheiro = lerFloat("Valor recebido: R$ "); // le o valor entregue pelo cliente

        if (dinheiro < totalFinal) { // se o dinheiro nao for suficiente para pagar tudo
            printf("Valor insuficiente! Deseja pagar o restante no cartao (s/n)? ");
            char resp;
            scanf(" %c", &resp); // le se vai completar com cartao
            while (getchar() != '\n'); // limpa o buffer

            if (resp == 's' || resp == 'S') { // se sim, cobra o restante no cartao
                float resto = totalFinal - dinheiro; // calcula quanto falta
                int ok;
                printf("Valor no cartao: R$ %.2f\n", resto); // informa o valor que vai no cartao
                do {
                    printf("(1) Pagamento na maquinha OK  (0) Nao OK: ");
                    ok = lerInt(""); // le o resultado do cartao
                    if (ok == 0) printf("Erro, tente novamente.\n");
                } while (ok == 0); // repete enquanto o cartao nao for aprovado
                strcpy(pg.tipo, "dc"); // registra como 'dc' = dinheiro e cartao
                printf("Pagamento em dinheiro e cartao efetuado!\n");
            } else { // se nao quiser pagar o restante no cartao, cancela
                printf("Pagamento cancelado.\n");
                return; // encerra a funcao sem registrar o pagamento
            }
        } else { // dinheiro suficiente para pagar tudo
            if (dinheiro > totalFinal) // se pagou a mais, calcula e exibe o troco
                printf("Troco: R$ %.2f\n", dinheiro - totalFinal);
            strcpy(pg.tipo, "d"); // registra o tipo como 'd' = dinheiro
            printf("Pagamento em dinheiro efetuado!\n");
        }

    } else { // usuario escolheu voltar ao menu
        printf("Voltando ao menu...\n");
        return; // encerra a funcao sem registrar pagamento
    }

    // marca todos os itens desta venda como pagos
    for (i = 0; i < totalVendas; i++) {
        if (vendas[i].numVenda == numVenda) // filtra os itens desta venda
            vendas[i].pagamento = 'p';      // muda de 'a'(aberto) para 'p'(pago)
    }

    // adiciona o pagamento no vetor dinamico de pagamentos
    pagamentos = realloc(pagamentos, (totalPagamentos + 1) * sizeof(Pagamento));
    pagamentos[totalPagamentos] = pg; // copia os dados do pagamento para o vetor
    totalPagamentos++; // incrementa o contador de pagamentos

    proximoNumVenda++; // incrementa para que a proxima venda tenha um numero diferente
    printf("Venda finalizada com sucesso!\n");
}

/* Realiza a retirada de dinheiro do caixa (sangria) */
void sangria() {
    float totalDinheiro = valorAbertura; // começa com o valor de abertura do caixa
    int i;

    // soma todos os pagamentos feitos em dinheiro ou em dinheiro+cartao
    for (i = 0; i < totalPagamentos; i++) {
        if (strcmp(pagamentos[i].tipo, "d") == 0 || strcmp(pagamentos[i].tipo, "dc") == 0)
            totalDinheiro += pagamentos[i].valorPago; // acumula o valor em dinheiro
    }

    printf("\nTotal em dinheiro no caixa: R$ %.2f\n", totalDinheiro); // mostra o total disponivel
    printf("(Deve sobrar pelo menos R$ 50,00 de troco)\n"); // lembra a regra do troco minimo

    float retirada = lerFloat("Informe o valor da retirada: R$ "); // le o valor a retirar

    if (retirada >= totalDinheiro - 50) { // verifica se a retirada respeitaria o saldo minimo de R$50
        // informa o valor maximo permitido para retirada
        printf("Erro! Nao e permitido retirar esse valor. Maximo permitido: R$ %.2f\n", totalDinheiro - 50);
    } else {
        valorAbertura -= retirada; // desconta a retirada do valor de abertura do caixa
        printf("Retirada de R$ %.2f realizada!\n", retirada); // confirma a retirada
    }
}

/* Busca e exibe vendas que nao tiveram o pagamento registrado (pagamento em aberto) */
void pagamentoEmAberto() {
    int i, encontrou = 0; // 'encontrou' serve para saber se achou alguma venda em aberto

    for (i = 0; i < totalVendas; i++) {          // percorre todos os itens de venda
        if (vendas[i].pagamento == 'a') {         // verifica se o item esta em aberto
            encontrou = 1;                        // marca que encontrou pelo menos uma venda aberta
            // exibe as informacoes da venda em aberto
            printf("Venda em aberto encontrada! Num. Venda: %d | Produto: %s | Total: R$ %.2f\n",
                vendas[i].numVenda, vendas[i].descricao, vendas[i].totalItem);
        }
    }
    if (!encontrou) // se nao encontrou nenhuma venda aberta
        printf("Nenhuma venda em aberto encontrada.\n");
}

/* Exibe o menu de vendas e chama as funcoes corretas */
void menuVendas() {
    int opc; // variavel para guardar a opcao escolhida
    do {
        // exibe as opcoes do submenu de vendas
        printf("\n---- VENDAS ----\n");
        printf("1 - Nova Venda\n");
        printf("2 - Retirada de Caixa (Sangria)\n");
        printf("3 - Pagamento (vendas em aberto)\n");
        printf("4 - Retornar ao Menu Principal\n");
        opc = lerInt("Escolha: "); // le a opcao

        switch (opc) {
            case 1: novaVenda(); break;           // chama o processo de nova venda
            case 2: sangria(); break;             // chama a retirada de caixa
            case 3: pagamentoEmAberto(); break;   // chama a verificacao de vendas abertas
            case 4: printf("Voltando...\n"); break; // sai do submenu
            default: printf("Opcao invalida!\n"); break;
        }
    } while (opc != 4); // repete ate o usuario escolher voltar
}

// ===================== ABERTURA E FECHAMENTO DE CAIXA =====================

/* Registra a abertura do caixa com o valor inicial */
void abrirCaixa() {
    if (valorAbertura >= 0) { // verifica se o caixa ja esta aberto (valor >= 0)
        printf("Erro! O caixa ja esta aberto.\n");
        return; // encerra sem fazer nada
    }
    valorAbertura = lerFloat("Digite o valor inicial do caixa: R$ "); // le o valor inicial
    if (valorAbertura < 0) { // verifica se o valor digitado e valido (nao pode ser negativo)
        printf("Valor invalido! Caixa nao aberto.\n");
        valorAbertura = -1; // volta ao estado de caixa fechado
    } else {
        printf("Caixa aberto com R$ %.2f\n", valorAbertura); // confirma a abertura
    }
}

/* Gera o resumo do fechamento do caixa e verifica divergencias */
void fecharCaixa() {
    float faturamento = 0; // total de tudo que foi vendido
    float vlPagoD = 0;     // total pago somente em dinheiro
    float vlPagoC = 0;     // total pago somente em cartao
    float vlPagoDC = 0;    // total pago em dinheiro e cartao juntos
    int qtdVendas = 0;     // quantidade de vendas distintas realizadas
    int i;

    int vendaContada[1000];  // vetor para guardar os numeros de vendas ja contadas
    int totalContadas = 0;   // quantas vendas distintas ja foram contadas

    // percorre todos os itens de venda para somar o faturamento e contar vendas unicas
    for (i = 0; i < totalVendas; i++) {
        faturamento += vendas[i].totalItem; // soma o valor de cada item ao faturamento

        // verifica se o numero desta venda ja foi contado antes
        int j, achou = 0;
        for (j = 0; j < totalContadas; j++) {
            if (vendaContada[j] == vendas[i].numVenda) { achou = 1; break; } // ja foi contada
        }
        if (!achou) { // se ainda nao foi contada
            vendaContada[totalContadas++] = vendas[i].numVenda; // registra o numero da venda
            qtdVendas++; // incrementa o contador de vendas distintas
        }
    }

    // percorre os pagamentos e classifica por tipo
    for (i = 0; i < totalPagamentos; i++) {
        if (strcmp(pagamentos[i].tipo, "d") == 0)  vlPagoD  += pagamentos[i].valorPago; // so dinheiro
        if (strcmp(pagamentos[i].tipo, "c") == 0)  vlPagoC  += pagamentos[i].valorPago; // so cartao
        if (strcmp(pagamentos[i].tipo, "dc") == 0) vlPagoDC += pagamentos[i].valorPago; // dinheiro+cartao
    }

    // calcula o ajuste: deve ser zero se tudo estiver correto
    // formula: faturamento - abertura - pagamentos em dinheiro - pagamentos em cartao - pagamentos mistos
    float ajuste = faturamento - valorAbertura - vlPagoD - vlPagoC - vlPagoDC;

    // exibe o resumo completo do fechamento
    printf("\n---- Fechamento de Caixa - Resumo ----\n");
    printf("Quantidade de vendas realizadas: %d\n", qtdVendas);
    printf("Total do faturamento no dia: R$ %.2f\n", faturamento);
    printf("Valor da abertura de caixa: R$ %.2f\n", valorAbertura);
    printf("Valor pago em dinheiro: R$ %.2f\n", vlPagoD);
    printf("Valor pago em cartao: R$ %.2f\n", vlPagoC);
    printf("Valor pago em dinheiro/cartao: R$ %.2f\n", vlPagoDC);

    if (ajuste == 0) { // se o ajuste for zero, o caixa fecha corretamente
        printf("Fechamento de Caixa: OK (sem divergencias)\n");
    } else { // se o ajuste nao for zero, ha divergencia
        printf("ATENCAO: O caixa nao podera ser fechado porque ha divergencia de valores!\n");
        if (ajuste > 0) // sobra de dinheiro
            printf("Sobra de: R$ %.2f\n", ajuste);
        else            // falta de dinheiro
            printf("Falta de: R$ %.2f\n", -ajuste); // exibe sem o sinal negativo
    }
}

/* Exibe o menu de abertura de caixa */
void menuAberturaCaixa() {
    int opc;
    do {
        printf("\n---- ABERTURA DE CAIXA ----\n");
        printf("1 - Abrir o Caixa\n");
        printf("2 - Retornar ao Menu Principal\n");
        opc = lerInt("Escolha: ");
        if (opc == 1) abrirCaixa();        // chama a funcao de abrir o caixa
        else if (opc != 2) printf("Opcao invalida!\n");
    } while (opc != 2); // repete ate o usuario voltar
}

/* Exibe o menu de fechamento de caixa */
void menuFechamentoCaixa() {
    int opc;
    do {
        printf("\n---- FECHAMENTO DE CAIXA ----\n");
        printf("1 - Fechar o Caixa\n");
        printf("2 - Retornar ao Menu Principal\n");
        opc = lerInt("Escolha: ");
        if (opc == 1) fecharCaixa();       // chama a funcao de fechar o caixa
        else if (opc != 2) printf("Opcao invalida!\n");
    } while (opc != 2); // repete ate o usuario voltar
}

// ===================== RELATORIOS =====================

/* Exibe a listagem completa de todos os clientes cadastrados */
void listarClientes() {
    int i;
    if (totalClientes == 0) { printf("Nenhum cliente cadastrado.\n"); return; } // sem clientes, encerra

    printf("\n---- Listagem de Clientes ----\n");
    for (i = 0; i < totalClientes; i++) { // percorre todos os clientes
        // exibe as informacoes principais de cada cliente
        printf("Cod: %d | Nome: %s | CPF: %s | Cel: %s\n",
            clientes[i].codigo, clientes[i].nome, clientes[i].cpf, clientes[i].celular);
    }
}

/* Exibe a listagem completa de todos os produtos cadastrados */
void listarProdutos() {
    int i;
    if (totalProdutos == 0) { printf("Nenhum produto cadastrado.\n"); return; } // sem produtos, encerra

    printf("\n---- Listagem de Produtos ----\n");
    // cabecalho da tabela
    printf("%-6s %-30s %-15s %-10s %-10s %-8s %-8s\n",
        "Cod", "Descricao", "Categoria", "Pr.Compra", "Pr.Venda", "Estoque", "Est.Min");

    for (i = 0; i < totalProdutos; i++) { // percorre todos os produtos
        char cat[20]; // variavel local para o nome da categoria
        if (produtos[i].categoria == 1) strcpy(cat, "Alimento");
        else if (produtos[i].categoria == 2) strcpy(cat, "Mat.Limpeza");
        else strcpy(cat, "Panificacao");

        // exibe uma linha com todos os dados do produto
        printf("%-6d %-30s %-15s R$%-8.2f R$%-8.2f %-8d %-8d\n",
            produtos[i].codigo, produtos[i].descricao, cat,
            produtos[i].precoCompra,
            calcularPrecoVenda(&produtos[i]), // preco de venda calculado com margem
            produtos[i].estoque, produtos[i].estoqueMin);
    }
}

/* Exibe a listagem de todas as vendas registradas */
void listarVendas() {
    int i;
    if (totalVendas == 0) { printf("Nenhuma venda registrada.\n"); return; } // sem vendas, encerra

    printf("\n---- Listagem de Vendas ----\n");
    // cabecalho da tabela
    printf("%-8s %-30s %-12s %-6s %-10s %-6s\n", "Num.Venda", "Produto", "Preco", "Qtd", "Total", "Pag.");

    for (i = 0; i < totalVendas; i++) { // percorre todos os itens de venda
        // exibe uma linha com os dados do item de venda
        printf("%-8d %-30s R$%-10.2f %-6d R$%-8.2f %-6c\n",
            vendas[i].numVenda, vendas[i].descricao,
            vendas[i].precoVenda, vendas[i].quantidade,
            vendas[i].totalItem,
            vendas[i].pagamento); // 'a'=aberto, 'p'=pago
    }
}

/* Exibe o menu de relatorios e chama as funcoes corretas */
void menuRelatorios() {
    int opc;
    do {
        printf("\n---- RELATORIOS ----\n");
        printf("1 - Listagem de Clientes\n");
        printf("2 - Listagem de Produtos\n");
        printf("3 - Listagem de Vendas\n");
        printf("4 - Retornar ao Menu Principal\n");
        opc = lerInt("Escolha: "); // le a opcao escolhida

        switch (opc) {
            case 1: listarClientes(); break; // exibe lista de clientes
            case 2: listarProdutos(); break; // exibe lista de produtos
            case 3: listarVendas(); break;   // exibe lista de vendas
            case 4: printf("Voltando...\n"); break; // sai do submenu
            default: printf("Opcao invalida!\n"); break;
        }
    } while (opc != 4); // repete ate o usuario voltar
}

// ===================== FUNCAO PRINCIPAL =====================

int main() {
    int opc; // variavel que guarda a opcao do menu principal

    do {
        // exibe o menu principal do sistema
        printf("\n==============================\n");
        printf("     MERCEARIA DONA BERE\n");
        printf("==============================\n");
        printf("1 - Cadastros\n");
        printf("2 - Vendas\n");
        printf("3 - Abertura de Caixa\n");
        printf("4 - Fechamento de Caixa\n");
        printf("5 - Relatorios\n");
        printf("6 - Sair\n");
        printf("==============================\n");
        opc = lerInt("Escolha: "); // le a opcao do usuario

        // executa o submenu correspondente a opcao escolhida
        switch (opc) {
            case 1: menuCadastros(); break;       // vai para o menu de cadastros
            case 2: menuVendas(); break;          // vai para o menu de vendas
            case 3: menuAberturaCaixa(); break;   // vai para a abertura de caixa
            case 4: menuFechamentoCaixa(); break; // vai para o fechamento de caixa
            case 5: menuRelatorios(); break;      // vai para os relatorios
            case 6: printf("Saindo... Ate logo!\n"); break; // encerra o programa
            default: printf("Opcao invalida!\n"); break;
        }

    } while (opc != 6); // repete o menu principal ate o usuario escolher sair

    // libera toda a memoria alocada dinamicamente antes de encerrar o programa
    free(clientes);   // libera a memoria do vetor de clientes
    free(produtos);   // libera a memoria do vetor de produtos
    free(vendas);     // libera a memoria do vetor de vendas
    free(pagamentos); // libera a memoria do vetor de pagamentos

    return 0; // encerra o programa com sucesso
}